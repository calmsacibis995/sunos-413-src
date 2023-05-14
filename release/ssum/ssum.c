/* @(#) ssum.c 1.1@(#) 7/30/92 */

/* Copyright 1986 Sun Microsystems, Inc. */

#include <stdio.h>
#include <sys/param.h>
#include <ctype.h>
#include "ssum.h"

#ifdef DEBUG
int	dumpser = 0,
	mflag = 0,
	debug = 0;
FILE	*dumpfile;
#endif DEBUG

/*
 * abortflag is set to zero if a calling routine wants to handle errors.
 */
int	abortflag = 1;	/* abort() on error */

char	*calloc(),
	*sprintf(),
	*sccsname();

/*
 * myname must be set to the name of the main program for error messages
 */
extern char	*myname;

static int	absline;	/* absolute line number in SCCS file */

/*
 * sidstr - format a SCCS ID for printing
 */
char *
sidstr(sid)
	sid_t	*sid;
{
	static char	buf[30];

	return(sprintf(buf, sid->s_br ? "%d.%d.%d.%d" : "%d.%d",
		sid->s_rel, sid->s_lev, sid->s_br, sid->s_seq));
}

/*
 * fmterr - Print a SCCS file format error and abort if abortflag != 0
 */
static void
fmterr(fname, explain)
	char	*fname,
		*explain;
{
	(void)fprintf(stderr, "%s: format error in %s%s%s\n",
		myname, fname, explain? ", ":"", explain?explain:"");
	if( abortflag )
		abort();
}

/*
 * freedelta - free all storage associated with a delta_t structure
 */
void
freedelta(dptr)
	register delta_t	*dptr;
{
	if( !dptr )
		return;

	freeslist((slist_t *)dptr->q);	/* !! SHOULD BE NIL !! */
	freeslist((slist_t *)dptr->ixglist);
	freeslist((slist_t *)dptr->includes_me);
	freeslist((slist_t *)dptr->excludes_me);
	freeslist((slist_t *)dptr->ignores_me);
	free((char *)dptr);
}

/*
 * freearray - free all storage associated with an SCCS file
 */
void
freearray(array)
	delta_t	**array;
{
	register int	i,
		maxdelta;

	if( array == (delta_t **)NULL )
		return;
	maxdelta = (long)array[0];
	for( i = 1; i <= maxdelta; ++i)
		freedelta(array[i]);
	free((char *)array);
}

/*
 * freeslist - free all the elements of a singly linked list
 */
void
freeslist(slist)
	slist_t	*slist;
{
	struct slist	*next;

	while( slist )
	{
		next = slist->next;
		free((char *)slist);
		slist = next;
	}
}

/*
 * buildtree - the second stage of processing an SCCS file.
 *
 *	The arguments for buildtree are generated by readhead();
 *
 *	Buildtree takes a list of delta_t's linked through the child pointer.
 *	The first element in the list must be the hightest numbered SCCS delta.
 *
 *	Buildtree returns a pointer to an array of pointers to delta_t's.
 *	The array is indexed by delta serial number and the first element
 *	is actually a count of the number of elements in the array.
 *
 *	Buildtree links the parent, sibling, and child pointers in the delta_t
 *	structures to reflect the SCCS delta relationships.
 *
 *	The pointer to the list if deltas is passed by reference. Buildtree
 *	changes the pointer to point to the head of the tree.
 *
 *	(N.B. SCCS delta serial numbers begin at 1.)
 *
 */
delta_t **
buildtree(thetree, fname)
	delta_t	**thetree;	/* A linked list of delta_t's linked through
				 * the child pointer.
				 */
	char	*fname;		/* Name of the SCCS file for error messages */
{
	int	i,		/* misc variable */
		maxdelta;	/* highest delta serial number in SCCS file */
	register delta_t
		*tree = *thetree,
		**array,	/* Pointer to the returned array of delta_t's */
		*dptr;		/* Current delta */

	maxdelta = tree->sid.s_serial;	/* 1st element is highest delta */

	/* build a tree */
	if((array=(delta_t **)calloc((unsigned)maxdelta+1,sizeof(delta_t *)))==NULL)
		nomem();

	(long)array[0] = (long)maxdelta;	/* save # of elements */

	/*
	 * Create an array so that deltas can be indexed by serial number
	 */
	for( dptr = tree; dptr; dptr = dptr->child )
		array[dptr->sid.s_serial] = dptr;

	/*
	 * Zero all child pointers to avoid any confusion during debugging
	 */
	for( i = 1; i <= maxdelta; ++i )
		if( (dptr = array[i]) != NULL )
			dptr->child = NULL;

	tree = NULL;

	/*
	 * Link the deltas into a tree structure.
	 */
	for( i = 1; i <= maxdelta; ++i)
	{
		delta_t	*parent;
		int	j;

		/*
		 * Link to the parent of each non-null delta
		 */
		if( (dptr = array[i]) == NULL )
			continue;
		if( (j = dptr->sid.s_pred) == 0 )
			continue;
		if( (parent = array[j]) == NULL )
		{
			fmterr(fname, "bad tree structure in header");
			freearray(array);
			return((delta_t **)0);
		}
		dptr->parent = parent;

		/*
		 * Create child and sibling links
		 */
		if( parent->child )
		{
			register delta_t	*temp;

			temp = parent->child;

			while( temp->sibling )
				temp = temp->sibling;

			temp->sibling = dptr;
		}
		else
			parent->child = dptr;

		/*
		 * For each delta that the current delta
		 * ignores/inserts/excludes, create a structure in the
		 * ignored/inserted/exclueded delta to aid in processing the
		 * edit commands.
		 */
		if( dptr->ixglist )
		{
			register delta_t	*temp;
			ixglist_t	*ixg,
					*new;

			for( ixg = dptr->ixglist; ixg ; ixg = ixg->next )
			{
				if( (new = (ixglist_t *)calloc(1, sizeof(*ixg))) == (ixglist_t *)NULL)
					nomem();
				*new = *ixg;
				new->deltap = dptr;
				new->delta = dptr->sid.s_serial;

				temp = array[ixg->delta];
				ixg->deltap = temp;

				switch( ixg->action )
				{
				case INCLUDE:
					new->next = temp->includes_me;
					temp->includes_me = new;
					break;
				case EXCLUDE:
					new->next = temp->excludes_me;
					temp->excludes_me = new;
					break;
				case IGNORE:
					new->next = temp->ignores_me;
					temp->ignores_me = new;
					break;
				default:
					fmterr(fname, "bad [ixg] command in header");
					freearray(array);
					return((delta_t **)0);
					break;
				}
			}
		}
	}

	/*
	 * Find the head of the tree. The first (lowest numbered) non-null
	 * delta in the file is the head of the tree.
	 */
	for( i = 1; i <= maxdelta; ++i )
		if( array[i] )
		{
			*thetree = array[i];
			break;
		}
	return(array);
}

/*
 * readdeltainfo - collect the information about a single delta from SCCS
 *	delta header records.
 */
static delta_t *
readdeltainfo(fname, file, ptr)
	char	*fname;		/* SCCS file name for error messages */
	register char	*ptr;	/* pointer to SCCS STATS record */
	FILE	*file;		/* SCCS file to read more records */
{
	register delta_t	*dptr;	/* pointer to new delta_t */
	char	line[512];		/* input buffer */

	if( (dptr = (delta_t *)calloc(1,sizeof(delta_t))) == NULL)
	{
		nomem();
		return((delta_t *)0);
	}

	(void)sscanf(ptr, " %05d/%05d/%05d",
		&dptr->sid.s_added, &dptr->sid.s_deleted, &dptr->sid.s_same);

	dptr->maxlines = dptr->sid.s_added + dptr->sid.s_same;

	while( (ptr = fgets(line, sizeof(line), file)) != NULL )
	{
		if( *ptr++ != CTLCHAR )
			goto badfmt;	/* should see end of delta header 1st */

		switch(*ptr++)
		{
		case BDELTAB:	/* create a new delta record */
			if(	*ptr++ != ' '
				|| ((*ptr != 'D' ) && (*ptr != 'R')) )
				goto badfmt;

			if( *ptr++ == 'R' )	/* removed */
				break;		/* ignore removed deltas */

			if( *ptr++ != ' ' )
			{
				fmterr(fname, "bad BDELTAB in header");
				goto badfmt;
			}

			(void)sscanf(ptr, "%d.%d.%d.%d",
				&dptr->sid.s_rel, &dptr->sid.s_lev,
				&dptr->sid.s_br, &dptr->sid.s_seq);

			while(*ptr++ != ' ')
				continue;

			(void)sscanf(ptr, "%*2d/%*2d/%*2d %*2d:%*2d:%*2d %*s %d %d",
				&dptr->sid.s_serial, &dptr->sid.s_pred);

			if( dptr->sid.s_serial <= 0 )
				goto badfmt;
			break;

		case IGNORE:
		case EXCLUDE:
		case INCLUDE:
			{
				register ixglist_t	*ixg;
				char	action = ptr[-1];

				if( dptr == (delta_t *)NULL )
					goto badfmt;
				for(;;)
				{
					int	i;

					while( *ptr && isspace(*ptr) )
						++ptr;
					if( isdigit(*ptr) )
						i = atoi(ptr);
					else
						break;
					while( *ptr && isdigit(*ptr) )
						++ptr;

					if( (ixg = (ixglist_t *)calloc(1, sizeof(ixglist_t))) == (ixglist_t *)NULL )
						nomem();
					ixg->next = dptr->ixglist;
					dptr->ixglist = ixg;
					ixg->action = action;
					ixg->delta = i;
				}
			}
			break;

		case EDELTAB:
			if( dptr == (delta_t *)NULL )
				goto badfmt;

			if( dptr->sid.s_serial == 0 )	/* Removed delta? */
			{
				free((char *)dptr);
				dptr = (delta_t *)NULL;
				return((delta_t *) 0);
			}
			else
				return(dptr);

		/* These routines may be more usefull to people if MR numbers
		 * and comments are stored.
		 */
		case MRNUM:
		case COMMENTS:
			break;

		default:
			fmterr(fname, "illegal command in header");
			goto badfmt;
		}
	}
	fmterr(fname, "EOF in header");
badfmt:
	free((char *)dptr);
	return(BADDELTA);
}

/*
 * readhead - Process the header records in an SCCS file. Readhead returns a
 *	linked list of delta_t's linked through the child pointer. The 1st
 *	element in the array is the highest numbered delta.
 */
delta_t *
readhead(file, fname)
	FILE	*file;	/* SCCS file */
	char	*fname;	/* Name of file for error messages */
{
	register char	*ptr;
	delta_t	*dptr = (delta_t *)NULL,
		*tree = (delta_t *)NULL,
		*last = (delta_t *)NULL;
	char	line[1024];	/* input buffer */


	/* Read the first line of the SCCS file. The first line is always
	 * the HEAD record containing the checksum for the SCCS file.
	 */
	if(	fgets(line,sizeof(line),file) == NULL
		|| line[0] != CTLCHAR
		|| line[1] != HEAD )
	{
		fmterr(fname, "not an SCCS file");
		return((delta_t *)0);
	}

	absline = 1;	/* reset absolute line counter */

	/*
	 * Process each delta record in the header
	 */
	while( (ptr = fgets(line, sizeof(line), file)) != NULL )
	{
		++absline;

		if( *ptr++ != CTLCHAR )
			goto badfmt;

		if( *ptr == STATS )
		{
			dptr = readdeltainfo(fname,file,++ptr);

			if( dptr == BADDELTA )
				goto badfmt;
			else if( dptr != (delta_t *)0 )
			{
				/* keep the highest numbered delta
				 * 1st on the list
				 */
				if( last == (delta_t *)NULL )
					tree = dptr;
				else
					last->child = dptr;
				last = dptr;
			}
			/* else it is a removed delta and we don't care */
		}
		else
			switch( *ptr )
			{
			case EUSERTXT:	/* proper end of header record */
				return(tree);
				/* NOTREACHED */

			/* The following header records are not used by
			 * these routines
			 */
			case BUSERNAM:
			case EUSERNAM:
			case FLAG:
			case BUSERTXT:
				break;

			/*
			 * Edit, or unknown, commands cannot appear
			 * in the header.
			 */
			case INSERT:
			case DELETE:
			case END:
			default:
				fmterr(fname, "illegal command in header");
				goto badfmt;
				/* NOTREACHED */
			}
	}
	fmterr(fname, "eof before eoh");
badfmt:
	/* use "last" as a temp pointer to free the list */
	for(dptr = tree, last = (tree ? tree->child : (delta_t *)0);
	    dptr;
	    dptr = last, last = (dptr ? dptr->child : (delta_t *)0)
	    )
		freedelta(dptr);
	return((delta_t *)0);
	/*NOTREACHED*/
}

/*
 * addsum - add a line of text into the checksum for a particular delta and
 *	all of the deltas that include it.
 *
 *	This recursive routine is the performance bottleneck of the program.
 */
static void
addsum(dptr, text, delta, array)
	register delta_t
		*dptr;	/* current delta */
	char	*text;	/* line of text */
	int	delta;	/* originating delta */
	delta_t	**array;	/* array of deltas indexed by serial number */
{
	register ixglist_t	*ixg;	/* list of deltas that include/ex... */
	register delta_t	*temp;	/* used to traverse lists */

	/*
	 * If the delta has already "seen" the line of text, do not reprocess.
	 */
	if( dptr->absline >= absline )	/* redundant? */
		return;			/*  yes */
	else
		dptr->absline = absline;

	/*
	 * If the delta explicitly excludes or ignores text from the orginating
	 * delta, then do not process the text.
	 */
	for( ixg = dptr->ixglist; ixg; ixg = ixg->next )
		if(ixg->delta==delta && ixg->action != INCLUDE)
			return;

	/*
	 * If the insert state is ON then process the text.
	 */
	if( dptr->q && dptr->q->insert == ON )
	{
		register char		*ptr = text;
		register unsigned	sum = dptr->sum;
		register		c;
		register long		bytes = 0;

		++dptr->lines;			/* count this line */
		dptr->absline = absline;	/* save line number for
						 * redundancy check.
						 */

#ifdef DEBUG
		if( dumpser && dptr->sid.s_serial == dumpser )
		{
			if( debug > 1 )
				(void)fprintf(dumpfile, "%s %d\t",
					sidstr(&array[delta]->sid), absline);
			if( mflag )
				(void)fprintf(dumpfile, "%s\t",
					sidstr(&array[delta]->sid));
			(void)fprintf(dumpfile, "%s", ptr);
		}

		if(debug)
			(void)fprintf(stderr,"%s\t%d\n",
				sidstr(&array[dptr->sid.s_serial]->sid),
				dptr->lines);
#endif DEBUG

		/*
		 * The checksum algorithm is the same as the one in sum(1)
		 */
		while (c = *ptr++)
		{
			++bytes;
			if (sum & 01)
				sum = (sum>>1) + 0x8000;
			else
				sum >>= 1;
			sum += c;
			sum &= 0xFFFF;
		}
		dptr->sum = sum;
		dptr->bytes += bytes;


		/*
		 * Let each child delta see the line of text.
		 */
		for( temp = dptr->child; temp; temp = temp->sibling )
			addsum(temp, text, delta, array);

		/*
		 * Let each delta that include the current delta see the line
		 */
		ixg = dptr->includes_me;
	}
	else
	{
		/*
		 * The deltas that exclude or ignore this delta need to see
		 * the text.
		 */
		ixg = dptr->excludes_me;
	}

	/* Let interested deltas see the text. */
	while( ixg )
	{
		addsum(ixg->deltap, text, delta, array);
		ixg = ixg->next;
	}
}

/*
 * changestate - recursively change the edit state of a delta and all deltas
 *	affected buy that delta. changestate maintains a queue of edit
 *	states ordered by the delta number of the edit command for each delta.
 */
void
changestate(dptr, state, delta)
	register delta_t
		*dptr;	/* perform the requested state change on this delta */
	int	state,	/* requested new state */
		delta;	/* delta that generated this command */
{
	register ixglist_t	*ixg;
	register delta_t	*temp;

	if( dptr->absline >= absline )	/* redundant? */
		return;			/*  yes */
	else
		dptr->absline = absline;

	/*
	 * If the current delta excludes or ignore edits from the orginating
	 * delta, then return.
	 */
	for( ixg = dptr->ixglist; ixg; ixg = ixg->next )
		if(ixg->delta == delta && ixg->action != INCLUDE)
		{
			/*
			 * Take care of those who exclude or ignore
			 * the current delta
			 */
			if( delta != dptr->sid.s_serial )
			{
				for( ixg = dptr->excludes_me; ixg; ixg = ixg->next )
					changestate(ixg->deltap, state, delta);
				for( ixg = dptr->ignores_me; ixg; ixg = ixg->next )
					changestate(ixg->deltap, state, delta);
			}
			return;
		}

	/*
	 * Delete the old edit state in response to an END edit command.
	 */
	if( state == POP )
	{
		queue_t	*cur,
			*prev = (queue_t *)&dptr->q;

		for( cur = dptr->q; cur; cur = (prev = cur)->next )
			if( delta == cur->delta )
				break;
		if( cur )
		{
			prev->next = cur->next;
			free((char *)cur);
		}
	}
	else	/* Insert edit state into the list */
	{
		queue_t	*cur,
			*prev = (queue_t *)&dptr->q;

		for( cur = dptr->q; cur; cur = (prev = cur)->next )
			if( delta >= cur->delta )
				break;
		if( cur == (queue_t *)NULL || delta != cur->delta )
		{
			if((cur=(queue_t *)calloc(1, sizeof(queue_t))) == (queue_t *)NULL)
				nomem();
			cur->next = prev->next;
			cur->delta = delta;
			cur->insert = state;
			prev->next = cur;
		}
	}

	/*
	 * Let deltas that include this delta see the edit command.
	 */
	for( ixg = dptr->includes_me; ixg; ixg = ixg->next )
		changestate(ixg->deltap, state, delta);

	/*
	 * Let child deltas see the edit command.
	 */
	for( temp = dptr->child; temp; temp = temp->sibling )
		changestate(temp, state, delta);
}

/*
 * readtext - Process the text portion of an SCCS file and update the
 *	checksums of each delta_t.
 */
int
readtext(file, tree, fname, wholine, array)
	FILE	*file;
	register delta_t	*tree;
	char	*fname;
	int	wholine;	/* who owns the text */
	delta_t	**array;
{
	register char	*ptr;
	int	r;
	char	line[512];

	/*
	 * Process the next line of text
	 */
	while( (ptr = fgets(line, sizeof(line), file )) != NULL )
	{
		char	c;
		int	delta;
		register delta_t	*dptr;

		++absline;	/* keep track of the SCCS file line number */

		/*
		 * Text lines do not begin with CTLCHR
		 */
		if( *ptr != CTLCHAR )
		{
#ifdef DEBUG
			if( debug )
				(void)fprintf(stderr, "%s", ptr);
#endif DEBUG
			addsum(tree, ptr, wholine, array);
			continue;
		}

		/* Parse an edit command */
		(void)sscanf( ++ptr, "%c %d", &c, &delta);

		dptr = array[delta];	/* index the affected delta */

		/*
		 * Apply the edit command.
		 */
		switch(c)
		{
		case INSERT:
			if( dptr->command )	/* already have a command? */
			{
				  fmterr(fname, "two commands for one delta");
				  return(ERROR);
			}
			changestate(dptr, ON, delta);
			dptr->command = CMD_INS;
			if( (r=readtext(file, dptr, fname, delta, array)) != 0 )
				return(r);
			break;
		case DELETE:
			if( dptr->command )	/* already have a command? */
			{
				fmterr(fname, "two commands for one delta");
				return(ERROR);
			}
			changestate(dptr, OFF, delta);
			dptr->command = CMD_DEL;
			break;
		case END:
			if( dptr->command == CMD_NIL )
			{
				fmterr(fname,"END without command");
				return(ERROR);
			}
			else
			{
				int	savecmd = dptr->command;

				dptr->command = CMD_NIL;
				changestate(dptr, POP, delta);
				if( savecmd == CMD_INS )
					return(NOERROR);
			}
			break;
		default:
			fmterr(fname, "bad control line");
			return(ERROR);
			break;
		}
	}
	return(EOF);
}