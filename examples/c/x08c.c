//      3-d plot demo.
//
// Copyright (C) 2004  Alan W. Irwin
// Copyright (C) 2004  Rafael Laboissiere
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published
// by the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//
//

#include "plcdemos.h"

// plexit not declared in public header!
PLDLLIMPEXP void
plexit( const char *errormsg );

#define XPTS    35              // Data points in x
#define YPTS    46              // Data points in y

static PLFLT alt[] = { 60.0, 20.0 };
static PLFLT az[] = { 30.0, 60.0 };
static void cmap1_init( int );

static const char *title[] =
{
    "#frPLplot Example 8 - Alt=60, Az=30",
    "#frPLplot Example 8 - Alt=20, Az=60",
};

//--------------------------------------------------------------------------
// cmap1_init1
//
// Initializes color map 1 in HLS space.
// Basic grayscale variation from half-dark (which makes more interesting
// looking plot compared to dark) to light.
// An interesting variation on this:
//	s[1] = 1.0
//--------------------------------------------------------------------------

static void
cmap1_init( int gray )
{
    PLFLT i[2], h[2], l[2], s[2];

    i[0] = 0.0;         // left boundary
    i[1] = 1.0;         // right boundary

    if ( gray )
    {
        h[0] = 0.0;     // hue -- low: red (arbitrary if s=0)
        h[1] = 0.0;     // hue -- high: red (arbitrary if s=0)

        l[0] = 0.5;     // lightness -- low: half-dark
        l[1] = 1.0;     // lightness -- high: light

        s[0] = 0.0;     // minimum saturation
        s[1] = 0.0;     // minimum saturation
    }
    else
    {
        h[0] = 240; // blue -> green -> yellow ->
        h[1] = 0;   // -> red

        l[0] = 0.6;
        l[1] = 0.6;

        s[0] = 0.8;
        s[1] = 0.8;
    }

    plscmap1n( 256 );
    c_plscmap1l( 0, 2, i, h, l, s, NULL );
}

//--------------------------------------------------------------------------
// main
//
// Does a series of 3-d plots for a given data set, with different
// viewing options in each plot.
//--------------------------------------------------------------------------


static int           sombrero;

static PLOptionTable options[] = {
    {
        "sombrero",             // Turns on use of Sombrero function
        NULL,
        NULL,
        &sombrero,
        PL_OPT_BOOL,
        "-sombrero",
        "Use the \"sombrero\" function."
    },
    {
        NULL,                   // option
        NULL,                   // handler
        NULL,                   // client data
        NULL,                   // address of variable to set
        0,                      // mode flag
        NULL,                   // short syntax
        NULL
    }                           // long syntax
};

#define LEVELS    10

int
main( int argc, const char *argv[] )
{
    int      i, j, k;
    PLFLT    *x, *y, **z, *z_row_major, *z_col_major;
    PLfGrid2 grid_c, grid_row_major, grid_col_major;
    PLFLT    xx, yy, r;
    PLINT    ifshade;
    PLFLT    zmin, zmax, step;
    PLFLT    clevel[LEVELS];
    PLINT    nlevel = LEVELS;
    int      rosen  = 1;

    PLINT    indexxmin = 0;
    PLINT    indexxmax = XPTS;
    PLINT    *indexymin;
    PLINT    *indexymax;
    PLFLT    **zlimited;
    // parameters of ellipse that limits the data.
    PLFLT    x0 = 0.5 * (PLFLT) ( XPTS - 1 );
    PLFLT    a  = x0;
    PLFLT    y0 = 0.5 * (PLFLT) ( YPTS - 1 );
    PLFLT    b  = y0;
    PLFLT    square_root;

    // Parse and process command line arguments
    plMergeOpts( options, "x08c options", NULL );
    (void) plparseopts( &argc, argv, PL_PARSE_FULL );
    if ( sombrero )
        rosen = 0;

    // Initialize plplot

    plinit();

// Allocate data structures

    x = (PLFLT *) calloc( XPTS, sizeof ( PLFLT ) );
    y = (PLFLT *) calloc( YPTS, sizeof ( PLFLT ) );

    plAlloc2dGrid( &z, XPTS, YPTS );
    z_row_major = (PLFLT *) malloc( XPTS * YPTS * sizeof ( PLFLT ) );
    z_col_major = (PLFLT *) malloc( XPTS * YPTS * sizeof ( PLFLT ) );
    if ( !z_row_major || !z_col_major )
        plexit( "Memory allocation error" );

    grid_c.f         = z;
    grid_row_major.f = (PLFLT **) z_row_major;
    grid_col_major.f = (PLFLT **) z_col_major;
    grid_c.nx        = grid_row_major.nx = grid_col_major.nx = XPTS;
    grid_c.ny        = grid_row_major.ny = grid_col_major.ny = YPTS;

    for ( i = 0; i < XPTS; i++ )
    {
        x[i] = ( (double) ( i - ( XPTS / 2 ) ) / (double) ( XPTS / 2 ) );
        if ( rosen )
            x[i] *= 1.5;
    }

    for ( i = 0; i < YPTS; i++ )
    {
        y[i] = (double) ( i - ( YPTS / 2 ) ) / (double) ( YPTS / 2 );
        if ( rosen )
            y[i] += 0.5;
    }

    for ( i = 0; i < XPTS; i++ )
    {
        xx = x[i];
        for ( j = 0; j < YPTS; j++ )
        {
            yy = y[j];
            if ( rosen )
            {
                z[i][j] = pow( 1. - xx, 2. ) + 100. * pow( yy - pow( xx, 2. ), 2. );

                // The log argument may be zero for just the right grid.
                if ( z[i][j] > 0. )
                    z[i][j] = log( z[i][j] );
                else
                    z[i][j] = -5.; // -MAXFLOAT would mess-up up the scale
            }
            else
            {
                r       = sqrt( xx * xx + yy * yy );
                z[i][j] = exp( -r * r ) * cos( 2.0 * M_PI * r );
            }

            z_row_major[i * YPTS + j] = z[i][j];
            z_col_major[i + XPTS * j] = z[i][j];
        }
    }

    //  Allocate and alculate y index ranges and corresponding zlimited.
    plAlloc2dGrid( &zlimited, XPTS, YPTS );
    indexymin = (PLINT *) malloc( XPTS * sizeof ( PLINT ) );
    indexymax = (PLINT *) malloc( XPTS * sizeof ( PLINT ) );
    if ( !indexymin || !indexymax )
        plexit( "Memory allocation error" );

    for ( i = indexxmin; i < indexxmax; i++ )
    {
        square_root  = sqrt( 1. - MIN( 1., pow( ( (PLFLT) i - x0 ) / a, 2. ) ) );
        indexymin[i] = MAX( 0, (PLINT) ( y0 - b * square_root ) );
        indexymax[i] = MIN( YPTS, (PLINT) ( y0 + b * square_root ) );
        for ( j = indexymin[i]; j < indexymax[i]; j++ )
        {
            zlimited[i][j] = z[i][j];
        }
    }


    plMinMax2dGrid( (const PLFLT * const *) z, XPTS, YPTS, &zmax, &zmin );
    step = ( zmax - zmin ) / ( nlevel + 1 );
    for ( i = 0; i < nlevel; i++ )
        clevel[i] = zmin + step + step * i;

    pllightsource( 1., 1., 1. );

    for ( k = 0; k < 2; k++ )
    {
        for ( ifshade = 0; ifshade < 5; ifshade++ )
        {
            pladv( 0 );
            plvpor( 0.0, 1.0, 0.0, 0.9 );
            plwind( -1.0, 1.0, -0.9, 1.1 );
            plcol0( 3 );
            plmtex( "t", 1.0, 0.5, 0.5, title[k] );
            plcol0( 1 );
            if ( rosen )
                plw3d( 1.0, 1.0, 1.0, -1.5, 1.5, -0.5, 1.5, zmin, zmax, alt[k], az[k] );
            else
                plw3d( 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, zmin, zmax, alt[k], az[k] );

            plbox3( "bnstu", "x axis", 0.0, 0,
                "bnstu", "y axis", 0.0, 0,
                "bcdmnstuv", "z axis", 0.0, 0 );
            plcol0( 2 );

            if ( ifshade == 0 ) // diffuse light surface plot
            {
                cmap1_init( 1 );
                plfsurf3d( x, y, plf2ops_c(), (PLPointer) z, XPTS, YPTS, 0, NULL, 0 );
            }
            else if ( ifshade == 1 ) // magnitude colored plot
            {
                cmap1_init( 0 );
                plfsurf3d( x, y, plf2ops_grid_c(), ( PLPointer ) & grid_c, XPTS, YPTS, MAG_COLOR, NULL, 0 );
            }
            else if ( ifshade == 2 ) //  magnitude colored plot with faceted squares
            {
                cmap1_init( 0 );
                plfsurf3d( x, y, plf2ops_grid_row_major(), ( PLPointer ) & grid_row_major, XPTS, YPTS, MAG_COLOR | FACETED, NULL, 0 );
            }
            else if ( ifshade == 3 ) // magnitude colored plot with contours
            {
                cmap1_init( 0 );
                plfsurf3d( x, y, plf2ops_grid_col_major(), ( PLPointer ) & grid_col_major, XPTS, YPTS, MAG_COLOR | SURF_CONT | BASE_CONT, clevel, nlevel );
            }
            else // magnitude colored plot with contours and index limits.
            {
                cmap1_init( 0 );
                plsurf3dl( x, y, (const PLFLT * const *) zlimited, XPTS, YPTS, MAG_COLOR | SURF_CONT | BASE_CONT, clevel, nlevel, indexxmin, indexxmax, indexymin, indexymax );
            }
        }
    }

// Clean up

    free( (void *) x );
    free( (void *) y );
    plFree2dGrid( z, XPTS, YPTS );
    free( (void *) z_row_major );
    free( (void *) z_col_major );

    plFree2dGrid( zlimited, XPTS, YPTS );
    free( (void *) indexymin );
    free( (void *) indexymax );

    plend();

    exit( 0 );
}
