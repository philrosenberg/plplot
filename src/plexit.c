/* $Id$
   $Log$
   Revision 1.3  1992/09/30 18:25:45  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.2  1992/09/29  04:45:55  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:24  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plexit.c

	Error handlers.
*/

#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>

/* In case of an error this routine is called.  It just prints out the
   error message and trys to clean up as much as possible.
   The user should write his/her own pl_exit() routine, if cleanup needs to
   be done in the user program. */

void 
plexit (char *errormsg)
{
    void pl_exit();

    plend();
    if (*errormsg != '\0')
	fprintf(stderr, "\n%s\n", errormsg);
    fprintf(stderr, "Plplot aborted.\n", errormsg);
    pl_exit();
    exit(1);
}

/* A handy way to issue warnings, if need be. */

void 
plwarn (char *errormsg)
{
    fprintf(stderr, "\nPlplot warning:\n%s\n", errormsg);
}

void 
pl_exit (void)
{
}
