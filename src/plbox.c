/* $Id$
   $Log$
   Revision 1.3  1992/09/30 18:25:42  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.2  1992/09/29  04:45:45  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:14  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plbox.c

	Draws a box around the current viewport.
*/

#include "plplot.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

static PLFLT xlog[8] =
{0.301030, 0.477121, 0.602060, 0.698970,
0.778151, 0.845098, 0.903090, 0.954243};

/*----------------------------------------------------------------------*\
* void plbox()
*
* This draws a box around the current viewport. XOPT and YOPT are
* character strings which define the box as follows:
*
* A: Draw axis (X is horizontal line Y=0, Y is vertical line X=0)
* B: Draw bottom (X) or left (Y) edge of frame
* C: Draw top (X) or right (Y) edge of frame
* G: Draws a grid at the major tick interval
* I: Inverts tick marks
* L: Logarithmic axes, major ticks at decades, minor ticks at units
* N: Write numeric label at conventional location
* M: Write numeric label at unconventional location
* T: Draw major tick marks
* S: Draw minor tick marks
* V: (for Y only) Label vertically
*
* xtick, ytick are the major tick intervals required, zero for
* automatic selection
*
* nxsub, nysub are the number of subtick intervals in a major tick
* interval
\*----------------------------------------------------------------------*/

void
c_plbox(char *xopt, PLFLT xtick, PLINT nxsub,
	char *yopt, PLFLT ytick, PLINT nysub)
{
    static char string[40];
    PLINT lax, lbx, lcx, lgx, lix, llx, lmx, lnx, lsx, ltx;
    PLINT lay, lby, lcy, lgy, liy, lly, lmy, lny, lsy, lty, lvy;
    PLINT xmajor, xminor, ymajor, yminor;
    PLINT xmode, xprec, xdigmax, xdigits, xscale;
    PLINT ymode, yprec, ydigmax, ydigits, yscale;
    PLINT i, i1x, i2x, i3x, i4x, i1y, i2y, i3y, i4y, it0;
    PLINT nxsub1, nysub1;
    PLINT lxmin, lxmax, lymin, lymax;
    PLINT pxmin, pxmax, pymin, pymax;
    PLINT vppxmi, vppxma, vppymi, vppyma;
    PLINT level;
    PLFLT xpmm, ypmm, defmaj, defmin, htmaj, htmin;
    PLFLT xtick1, ytick1, vpwxmi, vpwxma, vpwymi, vpwyma;
    PLFLT pos, tn, tp, temp, offset, height;

    glev(&level);
    if (level < 3)
	plexit("Please set up window before calling plbox.");

    /* Open  the clip limits to the subpage limits */
    gclp(&lxmin, &lxmax, &lymin, &lymax);
    gphy(&pxmin, &pxmax, &pymin, &pymax);
    sclp(pxmin, pxmax, pymin, pymax);

    gvpp(&vppxmi, &vppxma, &vppymi, &vppyma);

    /* Tick and subtick sizes in device coords */
    gpixmm(&xpmm, &ypmm);
    gmaj(&defmaj, &htmaj);
    gmin(&defmin, &htmin);

    xmajor = MAX(ROUND(htmaj * ypmm), 1);
    ymajor = MAX(ROUND(htmaj * xpmm), 1);
    xminor = MAX(ROUND(htmin * ypmm), 1);
    yminor = MAX(ROUND(htmin * xpmm), 1);

    xtick1 = xtick;
    nxsub1 = nxsub;
    ytick1 = ytick;
    nysub1 = nysub;

    lax = stsearch(xopt, 'a');
    lbx = stsearch(xopt, 'b');
    lcx = stsearch(xopt, 'c');
    lgx = stsearch(xopt, 'g');
    lix = stsearch(xopt, 'i');
    llx = stsearch(xopt, 'l');
    lmx = stsearch(xopt, 'm');
    lnx = stsearch(xopt, 'n');
    lsx = stsearch(xopt, 's');
    ltx = stsearch(xopt, 't');

    lay = stsearch(yopt, 'a');
    lby = stsearch(yopt, 'b');
    lcy = stsearch(yopt, 'c');
    lgy = stsearch(yopt, 'g');
    liy = stsearch(yopt, 'i');
    lly = stsearch(yopt, 'l');
    lmy = stsearch(yopt, 'm');
    lny = stsearch(yopt, 'n');
    lsy = stsearch(yopt, 's');
    lty = stsearch(yopt, 't');
    lvy = stsearch(yopt, 'v');

    gvpw(&vpwxmi, &vpwxma, &vpwymi, &vpwyma);

    lax = lax && (vpwymi * vpwyma < 0.0) && !llx;
    lay = lay && (vpwxmi * vpwxma < 0.0) && !lly;

    plgxax(&xdigmax, &xdigits);
    plgyax(&ydigmax, &ydigits);

    if (llx) xtick1 = 1.0;
    if (lly) ytick1 = 1.0;

    /* Calculate tick spacing */

    if (ltx || lgx)
	pldtik(vpwxmi, vpwxma, &xtick1, &nxsub1, &xmode, &xprec, xdigmax,
	       &xscale);

    if (lty || lgy)
	pldtik(vpwymi, vpwyma, &ytick1, &nysub1, &ymode, &yprec, ydigmax,
	       &yscale);

    /* Set up tick variables */

    if (lix) {
	i1x = xminor;
	i2x = 0;
	i3x = xmajor;
	i4x = 0;
    }
    else {
	i1x = 0;
	i2x = xminor;
	i3x = 0;
	i4x = xmajor;
    }

    if (liy) {
	i1y = yminor;
	i2y = 0;
	i3y = ymajor;
	i4y = 0;
    }
    else {
	i1y = 0;
	i2y = yminor;
	i3y = 0;
	i4y = ymajor;
    }

    /* Draw the bottom edge of the box */

    if (lbx) {
	movphy(vppxmi, vppymi);
	if (ltx) {
	    tp = xtick1 * floor(vpwxmi / xtick1);
	    for (;;) {
		tn = tp + xtick1;
		if (lsx) {
		    if (llx) {
			for (i = 0; i <= 7; i++) {
			    temp = tp + xlog[i];
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(wcpcx(temp), vppymi, i1x, i2x);
			}
		    }
		    else {
			for (i = 1; i <= nxsub1 - 1; i++) {
			    temp = tp + i * (tn - tp) / nxsub1;
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(wcpcx(temp), vppymi, i1x, i2x);
			}
		    }
		}
		if (!BETW(tn, vpwxmi, vpwxma))
		    break;
		plxtik(wcpcx(tn), vppymi, i3x, i4x);
		tp = tn;
	    }
	}
	draphy(vppxma, vppymi);
    }

    /* Draw right-hand edge of box */

    if (lcy) {
	movphy(vppxma, vppymi);
	if (lty) {
	    tp = ytick1 * floor(vpwymi / ytick1);
	    for (;;) {
		tn = tp + ytick1;
		if (lsy) {
		    if (lly) {
			for (i = 0; i <= 7; i++) {
			    temp = tp + xlog[i];
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(vppxma, wcpcy(temp), i2y, i1y);
			}
		    }
		    else {
			for (i = 1; i <= nysub1 - 1; i++) {
			    temp = tp + i * (tn - tp) / nysub1;
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(vppxma, wcpcy(temp), i2y, i1y);
			}
		    }
		}
		if (!BETW(tn, vpwymi, vpwyma))
		    break;
		plytik(vppxma, wcpcy(tn), i4y, i3y);
		tp = tn;
	    }
	}
	draphy(vppxma, vppyma);
    }

    /* Draw the top edge of the box */

    if (lcx) {
	movphy(vppxma, vppyma);
	if (ltx) {
	    tp = xtick1 * (floor(vpwxma / xtick1) + 1);
	    for (;;) {
		tn = tp - xtick1;
		if (lsx) {
		    if (llx) {
			for (i = 7; i >= 0; i--) {
			    temp = tn + xlog[i];
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(wcpcx(temp), vppyma, i2x, i1x);
			}
		    }
		    else {
			for (i = nxsub1 - 1; i >= 1; i--) {
			    temp = tn + i * (tp - tn) / nxsub1;
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(wcpcx(temp), vppyma, i2x, i1x);
			}
		    }
		}
		if (!BETW(tn, vpwxmi, vpwxma))
		    break;
		plxtik(wcpcx(tn), vppyma, i4x, i3x);
		tp = tn;
	    }
	}
	draphy(vppxmi, vppyma);
    }

    /* Draw left-hand edge of box */

    if (lby) {
	movphy(vppxmi, vppyma);
	if (lty) {
	    tp = ytick1 * (floor(vpwyma / ytick1) + 1);
	    for (;;) {
		tn = tp - ytick1;
		if (lsy) {
		    if (lly) {
			for (i = 7; i >= 0; i--) {
			    temp = tn + xlog[i];
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(vppxmi, wcpcy(temp), i1y, i2y);
			}
		    }
		    else {
			for (i = nysub1 - 1; i >= 1; i--) {
			    temp = tn + i * (tp - tn) / nysub1;
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(vppxmi, wcpcy(temp), i1y, i2y);
			}
		    }
		}
		if (!BETW(tn, vpwymi, vpwyma))
		    break;
		plytik(vppxmi, wcpcy(tn), i3y, i4y);
		tp = tn;
	    }
	}
	draphy(vppxmi, vppymi);
    }

    /* Draw the horizontal axis */

    if (lax) {
	it0 = wcpcy((PLFLT) 0.0);
	movphy(vppxmi, it0);
	if (ltx) {
	    tp = xtick1 * floor(vpwxmi / xtick1);
	    for (;;) {
		tn = tp + xtick1;
		if (lsx) {
		    if (llx) {
			for (i = 0; i <= 7; i++) {
			    temp = tp + xlog[i];
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(wcpcx(temp), it0, xminor, xminor);
			}
		    }
		    else {
			for (i = 1; i <= nxsub1 - 1; i++) {
			    temp = tp + i * (tn - tp) / nxsub1;
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(wcpcx(temp), it0, xminor, xminor);
			}
		    }
		}
		if (!BETW(tn, vpwxmi, vpwxma))
		    break;
		plxtik(wcpcx(tn), it0, xmajor, xmajor);
		tp = tn;
	    }
	}
	draphy(vppxma, it0);
    }

    /* Draw the vertical axis */

    if (lay) {
	it0 = wcpcx((PLFLT) 0.0);
	movphy(it0, vppymi);
	if (lty) {
	    tp = ytick1 * floor(vpwymi / ytick1);
	    for (;;) {
		tn = tp + ytick1;
		if (lsy) {
		    if (lly) {
			for (i = 0; i <= 7; i++) {
			    temp = tp + xlog[i];
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(it0, wcpcy(temp), yminor, yminor);
			}
		    }
		    else {
			for (i = 1; i <= nysub1 - 1; i++) {
			    temp = tp + i * (tn - tp) / nysub1;
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(it0, wcpcy(temp), yminor, yminor);
			}
		    }
		}
		if (!BETW(tn, vpwymi, vpwyma))
		    break;
		plytik(it0, wcpcy(tn), ymajor, ymajor);
		tp = tn;
	    }
	}
	draphy(it0, vppyma);
    }

    /* Draw grid in x direction */

    if (lgx) {
	tp = xtick1 * (1. + floor(vpwxmi / xtick1 + .5));
	for (tn = tp; BETW(tn + xtick1 / 2., vpwxmi, vpwxma); tn += xtick1)
	    pljoin(tn, vpwymi, tn, vpwyma);
    }

    /* Draw grid in y direction */

    if (lgy) {
	tp = ytick1 * (1. + floor(vpwymi / ytick1 + .5));
	for (tn = tp; BETW(tn + ytick1 / 2., vpwymi, vpwyma); tn += ytick1)
	    pljoin(vpwxmi, tn, vpwxma, tn);
    }

    /* Write horizontal label(s) */

    if ((lmx || lnx) && ltx) {
	tp = xtick1 * (1. + floor(vpwxmi / xtick1));
	for (tn = tp; BETW(tn, vpwxmi, vpwxma); tn += xtick1) {
	    if (!llx)
		plform(tn, xscale, xprec, string);
	    else 
		sprintf(string,"10#u%d",ROUND(tn));

	    pos = (tn - vpwxmi) / (vpwxma - vpwxmi);
	    if (lnx)
		plmtex("b", (PLFLT) 1.5, pos, (PLFLT) 0.5, string);
	    if (lmx)
		plmtex("t", (PLFLT) 1.5, pos, (PLFLT) 0.5, string);
	}
	xdigits = 2;
	plsxax(xdigmax, xdigits);

/* Write separate exponential label if mode = 1. */

	if (!llx && xmode) {
	    pos = 1.0;
/*	    sprintf(string, "E%+d", xscale);	*/
	    sprintf(string, "(x10#u%d#d)", xscale);
	    if (lnx) plmtex("b", (PLFLT) 2.5, pos, (PLFLT) 0.5, string);
	    if (lmx) plmtex("t", (PLFLT) 2.5, pos, (PLFLT) 0.5, string);
	}
    }

    /* Write vertical label(s) */

    if ((lmy || lny) && lty) {
	ydigits = 0;
	tp = ytick1 * (1. + floor(vpwymi / ytick1));
	for (tn = tp; BETW(tn, vpwymi, vpwyma); tn += ytick1) {
	    if (!lly)
		plform(tn, yscale, yprec, string);
	    else 
		sprintf(string, "10#u%d", ROUND(tn));

	    pos = (tn - vpwymi) / (vpwyma - vpwymi);
	    if (lny) {
		if (lvy)
		    plmtex("lv", (PLFLT) 0.5, pos, (PLFLT) 1.0, string);
		else
		    plmtex("l", (PLFLT) 1.5, pos, (PLFLT) 0.5, string);
	    }
	    if (lmy) {
		if (lvy)
		    plmtex("rv", (PLFLT) 0.5, pos, (PLFLT) 0.0, string);
		else
		    plmtex("r", (PLFLT) 1.5, pos, (PLFLT) 0.5, string);
	    }
	    ydigits = MAX(ydigits, strlen(string));
	}
	if (!lvy)
	    ydigits = 2;
	plsyax(ydigmax, ydigits);

/* Write separate exponential label if mode = 1. */

	if (!lly && ymode) {
/*	    sprintf(string, "E%+d", yscale);	*/
	    sprintf(string, "(x10#u%d#d)", yscale);
	    offset = 0.02;
	    height = 1.6;
	    if (lny) {
		pos = 0.0 - offset;
		plmtex("t", (PLFLT) height, pos, (PLFLT) 1.0, string);
	    }
	    if (lmy) {
		pos = 1.0 + offset;
		plmtex("t", (PLFLT) height, pos, (PLFLT) 0.0, string);
	    }
	}
    }
    /* Restore the clip limits to viewport edge */

    sclp(lxmin, lxmax, lymin, lymax);
}
