//--------------------------------------------------------------------------
// Copyright (C) 2004  Andrew Ross
// Copyright (C) 2004  Alan W. Irwin
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
// Implementation of PLplot example 15 in C++.
//--------------------------------------------------------------------------

#include "plc++demos.h"

#ifdef PL_USE_NAMESPACE
using namespace std;
#endif

class x15 {
public:
    x15( int, const char ** );
    void plot1( PLFLT * *, PLFLT, PLFLT );
    void plot2( PLFLT * *, PLFLT, PLFLT );
    void plot3();

private:
    // Class data
    plstream         *pls;

    const static int XPTS;
    const static int YPTS;
};

const int x15::  XPTS = 35;
const int x15::  YPTS = 46;


x15::x15( int argc, const char ** argv )
{
    int   i, j;

    PLFLT xx;
    PLFLT yy;
    PLFLT **z;
    PLFLT zmin, zmax;


    // plplot initialization

    pls = new plstream();

    // Parse and process command line arguments.
    pls->parseopts( &argc, argv, PL_PARSE_FULL );

    // Initialize PLplot.
    pls->init();
    // no need to fiddle with color maps since example uses default cmap0
    // and the cmap1 fiddling that x15c.c does is completely irrelevant
    // (although interesting).

    pls->Alloc2dGrid( &z, XPTS, YPTS );

    for ( i = 0; i < XPTS; i++ )
    {
        xx = (PLFLT) ( i - ( XPTS / 2 ) ) / (PLFLT) ( XPTS / 2 );
        for ( j = 0; j < YPTS; j++ )
        {
            yy      = ( (PLFLT) ( j - ( YPTS / 2 ) ) / (PLFLT) ( YPTS / 2 ) ) - 1.;
            z[i][j] = xx * xx - yy * yy + ( xx - yy ) / ( xx * xx + yy * yy + 0.1 );
        }
    }

    pls->MinMax2dGrid( z, XPTS, YPTS, &zmax, &zmin );

    plot1( z, zmin, zmax );
    plot2( z, zmin, zmax );
    plot3();

    //pls->end();

    // Tidy up allocated grids
    pls->Free2dGrid( z, XPTS, YPTS );

    delete pls;
}

// Illustrates a single shaded region.

void x15::plot1( PLFLT **z, PLFLT zmin, PLFLT zmax )
{
    PLFLT shade_min, shade_max, sh_color;
    int   sh_cmap = 0;
    PLFLT sh_width;
    int   min_color = 0, max_color = 0;
    PLFLT min_width = 0.0, max_width = 0.0;

    pls->adv( 0 );
    pls->vpor( 0.1, 0.9, 0.1, 0.9 );
    pls->wind( -1.0, 1.0, -1.0, 1.0 );

    // Plot using identity transform

    shade_min = zmin + ( zmax - zmin ) * 0.4;
    shade_max = zmin + ( zmax - zmin ) * 0.6;
    sh_color  = 7;
    sh_width  = 2.0;
    min_color = 9;
    max_color = 2;
    min_width = 2.0;
    max_width = 2.0;

    pls->psty( 8 );

    pls->shade( z, XPTS, YPTS, NULL, -1., 1., -1., 1.,
        shade_min, shade_max,
        sh_cmap, sh_color, sh_width,
        min_color, min_width, max_color, max_width,
        plstream::fill, true, NULL, NULL );

    pls->col0( 1 );
    pls->box( "bcnst", 0.0, 0, "bcnstv", 0.0, 0 );
    pls->col0( 2 );
    pls->lab( "distance", "altitude", "Bogon flux" );
}

// Illustrates multiple adjacent shaded regions, using different fill
// patterns for each region.

void x15::plot2( PLFLT **z, PLFLT zmin, PLFLT zmax )
{
    PLFLT        shade_min, shade_max, sh_color;
    int          sh_cmap = 0;
    PLFLT        sh_width;
    int          min_color = 0, max_color = 0;
    PLFLT        min_width = 0.0, max_width = 0.0;
    int          i;
    static PLINT nlin[10]   = { 1, 1, 1, 1, 1, 2, 2, 2, 2, 2 };
    static PLINT inc[10][2] = { { 450,    0 }, { -450,    0 }, { 0,   0 }, { 900,   0 },
                                { 300,    0 }, {  450, -450 }, { 0, 900 }, {   0, 450 },
                                { 450, -450 }, {    0,  900 } };
    static PLINT del[10][2] = { { 2000, 2000 }, { 2000, 2000 }, { 2000, 2000 },
                                { 2000, 2000 }, { 2000, 2000 }, { 2000, 2000 },
                                { 2000, 2000 }, { 2000, 2000 }, { 4000, 4000 },
                                { 4000, 2000 } };
    sh_width = 2.0;

    pls->adv( 0 );
    pls->vpor( 0.1, 0.9, 0.1, 0.9 );
    pls->wind( -1.0, 1.0, -1.0, 1.0 );

    // Plot using identity transform

    for ( i = 0; i < 10; i++ )
    {
        shade_min = zmin + ( zmax - zmin ) * i / 10.0;
        shade_max = zmin + ( zmax - zmin ) * ( i + 1 ) / 10.0;
        sh_color  = i + 6;
        pls->pat( nlin[i], inc[i], del[i] );

        pls->shade( z, XPTS, YPTS, NULL, -1., 1., -1., 1.,
            shade_min, shade_max,
            sh_cmap, sh_color, sh_width,
            min_color, min_width, max_color, max_width,
            plstream::fill, true, NULL, NULL );
    }

    pls->col0( 1 );
    pls->box( "bcnst", 0.0, 0, "bcnstv", 0.0, 0 );
    pls->col0( 2 );
    pls->lab( "distance", "altitude", "Bogon flux" );
}

// Illustrates shaded regions in 3d, using a different fill pattern for
// each region.

void x15::plot3()
{
    static PLFLT xx[2][5] = { { -1.0, 1.0, 1.0, -1.0, -1.0 },
                              { -1.0, 1.0, 1.0, -1.0, -1.0 } };
    static PLFLT yy[2][5] = { {  1.0,  1.0, 0.0, 0.0,  1.0 },
                              { -1.0, -1.0, 0.0, 0.0, -1.0 } };
    static PLFLT zz[2][5] = { { 0.0, 0.0, 1.0, 1.0, 0.0 },
                              { 0.0, 0.0, 1.0, 1.0, 0.0 } };

    pls->adv( 0 );
    pls->vpor( 0.1, 0.9, 0.1, 0.9 );
    pls->wind( -1.0, 1.0, -1.0, 1.0 );
    pls->w3d( 1., 1., 1., -1.0, 1.0, -1.0, 1.0, 0.0, 1.5, 30, -40 );

// Plot using identity transform

    pls->col0( 1 );
    pls->box3( "bntu", "X", 0.0, 0, "bntu", "Y", 0.0, 0, "bcdfntu", "Z", 0.5, 0 );
    pls->col0( 2 );
    pls->lab( "", "", "3-d polygon filling" );

    pls->col0( 3 );
    pls->psty( 1 );
    pls->line3( 5, xx[0], yy[0], zz[0] );
    pls->fill3( 4, xx[0], yy[0], zz[0] );
    pls->psty( 2 );
    pls->line3( 5, xx[1], yy[1], zz[1] );
    pls->fill3( 4, xx[1], yy[1], zz[1] );
}


int main( int argc, const char ** argv )
{
    x15 *x = new x15( argc, argv );

    delete x;
}


//--------------------------------------------------------------------------
//                              End of x15.cc
//--------------------------------------------------------------------------
