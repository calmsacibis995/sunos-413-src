/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef lint
static	char sccsid[] = "@(#)_mvwinchstr.c 1.1 92/07/30 SMI"; /* from S5R3.1 1.1 */
#endif

#define	NOMACROS

#include	"curses_inc.h"

mvwinchstr(win, y, x, str)
WINDOW *win;
int	y, x;
chtype	*str;
{
    return (wmove(win, y, x)==ERR?ERR:winchstr(win, str));
}
