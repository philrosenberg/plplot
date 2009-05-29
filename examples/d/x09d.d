/* $Id: $

	Contour plot demo.

   This file is part of PLplot.

   PLplot is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Library Public License as published
   by the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   PLplot is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with PLplot; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

*/

import std.string;
import plplot;

#define XPTS    35		/* Data points in x */
#define YPTS    46		/* Data points in y */

#define XSPA    2./(XPTS-1)
#define YSPA    2./(YPTS-1)

/* polar plot data */
#define PERIMETERPTS 100
#define RPTS 40
#define THETAPTS 40

/* potential plot data */
#define PPERIMETERPTS 100
#define PRPTS 40
#define PTHETAPTS 64
#define PNLEVEL 20

static PLFLT clevel[11] =
{-1., -.8, -.6, -.4, -.2, 0, .2, .4, .6, .8, 1.};

/* Transformation function */

extern (C) {
  PLFLT[] tr = {XSPA, 0.0, -1.0, 0.0, YSPA, -1.0};

  void mypltr(PLFLT x, PLFLT y, PLFLT* tx, PLFLT* ty, void* pltr_data)
  {
      *tx = tr[0]*x + tr[1]*y + tr[2];
      *ty = tr[3]*x + tr[4]*y + tr[5];
  }
}

static void polar()
/*polar contour plot example.*/
{
   int i,j;
   PLcGrid2 cgrid2;
   PLFLT **z;
   PLFLT px[PERIMETERPTS], py[PERIMETERPTS];
   PLFLT t, r, theta;
   PLFLT lev[10];

   plenv(-1., 1., -1., 1., 0, -2);
   plcol0(1);
       
/*Perimeter*/
   for (i = 0; i < PERIMETERPTS; i++) {
      t = (2.*M_PI/(PERIMETERPTS-1))*(double)i;
      px[i] = cos(t);
      py[i] = sin(t);
   }
   plline(PERIMETERPTS, px, py);
	       
/*create data to be contoured.*/
   plAlloc2dGrid(&cgrid2.xg, RPTS, THETAPTS);
   plAlloc2dGrid(&cgrid2.yg, RPTS, THETAPTS);
   plAlloc2dGrid(&z, RPTS, THETAPTS);
   cgrid2.nx = RPTS;
   cgrid2.ny = THETAPTS;
   
   for (i = 0; i < RPTS; i++) {
      r = i/(double)(RPTS-1);
      for (j = 0; j < THETAPTS; j++) {
	 theta = (2.*M_PI/(double)(THETAPTS-1))*(double)j;
	 cgrid2.xg[i][j] = r*cos(theta);
	 cgrid2.yg[i][j] = r*sin(theta);
	 z[i][j] = r;
      }
   }

   for (i = 0; i < 10; i++) {
      lev[i] = 0.05 + 0.10*(double) i;
   }

   plcol0(2);
   plcont(z, RPTS, THETAPTS, 1, RPTS, 1, THETAPTS, lev, 10,
	              pltr2, (void *) &cgrid2);
   plcol0(1);
   pllab("", "", "Polar Contour Plot");
   plFree2dGrid(z, RPTS, THETAPTS);
   plFree2dGrid(cgrid2.xg, RPTS, THETAPTS);
   plFree2dGrid(cgrid2.yg, RPTS, THETAPTS);
}

/*--------------------------------------------------------------------------*\
 * f2mnmx
 *
 * Returns min & max of input 2d array.
\*--------------------------------------------------------------------------*/

static void
f2mnmx(PLFLT **f, PLINT nx, PLINT ny, PLFLT *fmin, PLFLT *fmax)
{
    int i, j;

    *fmax = f[0][0];
    *fmin = *fmax;

    for (i = 0; i < nx; i++) {
	for (j = 0; j < ny; j++) {
            *fmax = MAX(*fmax, f[i][j]);
            *fmin = MIN(*fmin, f[i][j]);
	}
    }
}

static void potential()
/*shielded potential contour plot example.*/
{
  PLcGrid2 cgrid2;
  PLFLT rmax, xmin, xmax, x0, ymin, ymax, y0, zmin, zmax;
  PLFLT peps, xpmin, xpmax, ypmin, ypmax;
  PLFLT eps, q1, d1, q1i, d1i, q2, d2, q2i, d2i;
  PLFLT div1, div1i, div2, div2i;
  PLFLT **z;
  PLINT nlevelneg, nlevelpos;
  PLFLT dz, clevel, clevelneg[PNLEVEL], clevelpos[PNLEVEL];
  PLINT ncollin, ncolbox, ncollab;
  PLFLT px[PPERIMETERPTS], py[PPERIMETERPTS];
  PLFLT t, r, theta;

  /*create data to be contoured.*/
  plAlloc2dGrid(&cgrid2.xg, PRPTS, PTHETAPTS);
  plAlloc2dGrid(&cgrid2.yg, PRPTS, PTHETAPTS);
  plAlloc2dGrid(&z, PRPTS, PTHETAPTS);
  cgrid2.nx = PRPTS;
  cgrid2.ny = PTHETAPTS;

  for (i = 0; i < PRPTS; i++) {
    r = 0.5 + (double) i;
    for (j = 0; j < PTHETAPTS; j++) {
  theta = (2.*M_PI/(double)(PTHETAPTS-1))*(0.5 + (double) j);
  cgrid2.xg[i][j] = r*cos(theta);
  cgrid2.yg[i][j] = r*sin(theta);
    }
  }

  rmax = r;
  f2mnmx(cgrid2.xg, PRPTS, PTHETAPTS, &xmin, &xmax);
  f2mnmx(cgrid2.yg, PRPTS, PTHETAPTS, &ymin, &ymax);
  x0 = (xmin + xmax)/2.;
  y0 = (ymin + ymax)/2.;

  /* Expanded limits */
  peps = 0.05;
  xpmin = xmin - fabs(xmin)*peps;
  xpmax = xmax + fabs(xmax)*peps;
  ypmin = ymin - fabs(ymin)*peps;
  ypmax = ymax + fabs(ymax)*peps;

  /* Potential inside a conducting cylinder (or sphere) by method of images.
    Charge 1 is placed at (d1, d1), with image charge at (d2, d2).
    Charge 2 is placed at (d1, -d1), with image charge at (d2, -d2).
    Also put in smoothing term at small distances.
  */

  eps = 2.;

  q1 = 1.;
  d1 = rmax/4.;

  q1i = - q1*rmax/d1;
  d1i = pow(rmax, 2.)/d1;

  q2 = -1.;
  d2 = rmax/4.;

  q2i = - q2*rmax/d2;
  d2i = pow(rmax, 2.)/d2;

  for(int i=0; i<PRPTS; i++) {
    for(int j=0; j<PTHETAPTS; j++) {
      div1 = sqrt(pow(cgrid2.xg[i][j]-d1, 2.) + pow(cgrid2.yg[i][j]-d1, 2.) + pow(eps, 2.));
      div1i = sqrt(pow(cgrid2.xg[i][j]-d1i, 2.) + pow(cgrid2.yg[i][j]-d1i, 2.) + pow(eps, 2.));
      div2 = sqrt(pow(cgrid2.xg[i][j]-d2, 2.) + pow(cgrid2.yg[i][j]+d2, 2.) + pow(eps, 2.));
      div2i = sqrt(pow(cgrid2.xg[i][j]-d2i, 2.) + pow(cgrid2.yg[i][j]+d2i, 2.) + pow(eps, 2.));
      z[i][j] = q1/div1 + q1i/div1i + q2/div2 + q2i/div2i;
    }
  }
  f2mnmx(z, zmin, zmax);

  /* Positive and negative contour levels.*/
  dz = (zmax-zmin)/PNLEVEL;
  nlevelneg = 0;
  nlevelpos = 0;
  for(int i=0; i<PNLEVEL; i++) {
    clevel = zmin + (i+0.5)*dz;
    if(clevel<=0.)
      clevelneg[nlevelneg++] = clevel;
    else
      clevelpos[nlevelpos++] = clevel;
  }
  /* Colours! */
  ncollin = 11;
  ncolbox = 1;
  ncollab = 2;

  /* Finally start plotting this page! */
  pladv(0);
  plcol0(ncolbox);

  plvpas(0.1, 0.9, 0.1, 0.9, 1.0);
  plwind(xpmin, xpmax, ypmin, ypmax);
  plbox("", 0., 0, "", 0., 0);

  plcol0(ncollin);
  if(nlevelneg >0) {
    /* Negative contours */
    pllsty(2);
    plcont(z, 1, PRPTS, 1, PTHETAPTS, clevelneg, pltr2, (void*)&cgrid2);
  }

  if(nlevelpos >0) {
    /* Positive contours  */
    pllsty(1);
    plcont(z, 1, PRPTS, 1, PTHETAPTS, clevelpos, pltr2, (void*)&cgrid2);
  }
   
  /* Draw outer boundary  */
  for(int i=0; i<PPERIMETERPTS; i++) {
    t = (2.*PI/(PPERIMETERPTS-1))*i;
    px[i] = x0 + rmax*cos(t);
    py[i] = y0 + rmax*sin(t);
  }

  plcol0(ncolbox);
  plline(px, py);
       
  plcol0(ncollab);
  pllab("", "", "Shielded potential of charges in a conducting sphere");
}
  

/*--------------------------------------------------------------------------*\
 * main
 *
 * Does several contour plots using different coordinate mappings.
\*--------------------------------------------------------------------------*/
int main(char[][] args)
{
  PLINT[] mark = [ 1500 ], space = [ 1500 ];

  /* Parse and process command line arguments */
  plparseopts(args, PL_PARSE_FULL);

  /* Initialize plplot */
  plinit();

  /* Set up function arrays */
  PLFLT xx, yy;
  PLFLT[][] z = new PLFLT[][XPTS];
  for(int i=0; i<XPTS; i++)
    z[i] = new PLFLT[YPTS];

  PLFLT[][] w = new PLFLT[][XPTS];
  for(int i=0; i<XPTS; i++)
    w[i] = new PLFLT[YPTS];

  for(int i=0; i<XPTS; i++) {
    xx = cast(double)(i-(XPTS/2))/(XPTS/2);
    for(int j=0; j<YPTS; j++) {
	    yy = cast(double)(j-(YPTS/2))/(YPTS / 2)-1.0;
	    z[i][j] = xx*xx - yy*yy;
	    w[i][j] = 2*xx*yy;
    }
  }

  /* Set up grids */
  cgrid1.xg = (new PLFLT[XPTS]).ptr;
  cgrid1.yg = (new PLFLT[XPTS]).ptr;

  PLFLT[][] cgrid2.xg = new PLFLT[][XPTS];
  for(int i=0; i<XPTS; i++)
    cgrid2.xg[i] = new PLFLT[YPTS];

  PLFLT[][] cgrid2.yg = new PLFLT[][XPTS];
  for(int i=0; i<XPTS; i++)
    cgrid2.yg[i] = new PLFLT[YPTS];

  PLFLT argx, argy, distort;
  for(int i=0; i<XPTS; i++) {
    for(int j=0; j<YPTS; j++) {
      mypltr(cast(PLFLT)i, cast(PLFLT)j, &xx, &yy, null);

      argx = xx * M_PI/2;
      argy = yy * M_PI/2;
      distort = 0.4;

      cgrid1.xg[i] = xx + distort * cos(argx);
      cgrid1.yg[j] = yy - distort * cos(argy);

      cgrid2.xg[i][j] = xx + distort * cos(argx) * cos(argy);
      cgrid2.yg[i][j] = yy - distort * cos(argx) * cos(argy);
    }
  }

  /* Plot using identity transform */
  pl_setcontlabelparam(0.006, 0.3, 0.1, 1);
  plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
  plcol0(2);
  plcont(z, 1, XPTS, 1, YPTS, clevel, mypltr);
  
  plstyl(mark, space);
  plcol0(3);
  plcont(w, 1, XPTS, 1, YPTS, clevel, mypltr);
  plstyl(null, null);
  plcol0(1);
  pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");
  pl_setcontlabelparam(0.006, 0.3, 0.1, 0);
    
  /* Plot using 1d coordinate transform */
  plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
  plcol0(2);
  plcont(z, 1, XPTS, 1, YPTS, clevel, pltr1, cast(void*)&cgrid1);

  plstyl(1, mark, space);
  plcol0(3);
  plcont(w, 1, XPTS, 1, YPTS, clevel, pltr1, cast(void*)&cgrid1);
  plstyl(null, null);
  plcol0(1);
  pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

  /* Plot using 2d coordinate transform */
  plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
  plcol0(2);
  plcont(z, 1, XPTS, 1, YPTS, clevel, pltr2, (void *)&cgrid2);

  plstyl(mark, space);
  plcol0(3);
  plcont(w, 1, XPTS, 1, YPTS, clevel, pltr2, (void *)&cgrid2);
  plstyl(null, null);
  plcol0(1);
  pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

  pl_setcontlabelparam(0.006, 0.3, 0.1, 0);
  polar();

  pl_setcontlabelparam(0.006, 0.3, 0.1, 0);
  potential();

  plend();

  return 0;
}