
#ifndef lint
static	char sccsid[] = "@(#)d_asinpi_.c 1.1 92/07/30 SMI";
#endif

/*
 * Copyright (c) 1988 by Sun Microsystems, Inc.
 */

#include <math.h>

double d_asinpi_(x)
double *x;
{
	return asinpi(*x);
}
