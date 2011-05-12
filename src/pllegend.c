// $Id$
// pllegend(...) (which plots a discrete plot legend) and the static
// routines which support it.
//
// Copyright (C) 2010  Hezekiah M. Carty
// Copyright (C) 2010  Alan W. Irwin
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

//! @file
//!
//! Legend routines.
//!
#include "plplotP.h"

//--------------------------------------------------------------------------
//! Determine arguments of the last call to plvpor.  This gives a different
//! result than plgvpd if there is more than one subpage per page.
//!
//! @param p_xmin Pointer to a location that contains (after the call)
//! the normalized subpage coordinate of the left-hand edge of the
//! viewport.
//! @param p_xmax Pointer to a location that contains (after the call)
//! the normalized subpage coordinate of the right-hand edge of the
//! viewport.
//! @param p_ymin Pointer to a location that contains (after the call)
//! the normalized subpage coordinate of the bottom edge of the
//! viewport.
//! @param p_ymax Pointer to a location that contains (after the call)
//! the normalized subpage coordinate of the top edge of the viewport.

static void plgvpsp( PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax )
{
    if ( plsc->level < 1 )
    {
        plabort( "plgvpsp: Please call plinit first" );
        return;
    }
    if ( ( plsc->cursub <= 0 ) || ( plsc->cursub > ( plsc->nsubx * plsc->nsuby ) ) )
    {
        plabort( "plgvpsp: Please call pladv or plenv to go to a subpage" );
        return;
    }
    *p_xmin = ( plsc->vpdxmi - plsc->spdxmi ) / ( plsc->spdxma - plsc->spdxmi );
    *p_xmax = ( plsc->vpdxma - plsc->spdxmi ) / ( plsc->spdxma - plsc->spdxmi );
    *p_ymin = ( plsc->vpdymi - plsc->spdymi ) / ( plsc->spdyma - plsc->spdymi );
    *p_ymax = ( plsc->vpdyma - plsc->spdymi ) / ( plsc->spdyma - plsc->spdymi );
}

//--------------------------------------------------------------------------
//! Calculate parameters that help determine the position of the top
//! left of the legend in adopted coordinates.  See pllegend
//! documentation for definition of adopted coordinates.
//!
//! @param position Control variable containing valid combinations of the
//! following control bits that specify the 16 standard positions of
//! the legend: PL_POSITION_LEFT, PL_POSITION_RIGHT, PL_POSITION_TOP,
//! PL_POSITION_BOTTOM, PL_POSITION_INSIDE, and PL_POSITION_OUTSIDE.
//! @param legend_width Total legend width in adopted coordinates.
//! @param legend_height Total legend height in adopted coordinates.
//! @param x_legend_position Pointer to a location that contains
//! (after the call) the X value in adopted coordinates of
//! one of the 16 standard legend positions specified by position.
//! @param y_legend_position Pointer to a location that contains
//! (after the call) the Y equivalent of x_legend_position.
//! @param xsign Pointer to a location that contains (after the call)
//! the sign of the X offset relative to the standard legend position
//! specified by position.
//! @param ysign Pointer to a location that contains (after the call)
//! the Y equivalent to xsign.
//!

static void legend_position( PLINT position, PLFLT legend_width, PLFLT legend_height,
                             PLFLT *x_legend_position, PLFLT *y_legend_position,
                             PLFLT *xsign, PLFLT *ysign )
{
    // xorigin, yorigin, xlegend, and ylegend are all calculated for
    // one of the 16 standard positions specified by position and are
    // expressed in adopted coordinates.  xorigin is the X value of
    // the reference point of the adopted coordinates.  yorigin is the
    // Y value of the reference point of the adopted coordinates.
    // xlegend is the X coordinate of the top-left of the legend box
    // relative to the legend box reference point.  ylegend is the y
    // coordinate of the top-left of the legend box relative to the
    // legend box reference point.

    PLFLT xorigin, yorigin, xlegend, ylegend;
    // By default the sign of the x and y offsets is positive.
    *xsign = 1.;
    *ysign = 1.;
    if ( position & PL_POSITION_RIGHT )
    {
        xorigin = 1.;
        if ( position & PL_POSITION_TOP )
        {
            yorigin = 1.;
            if ( position & PL_POSITION_INSIDE )
            {
                xlegend = -legend_width;
                ylegend = 0.;
                *xsign  = -1.;
                *ysign  = -1.;
            }
            else if ( position & PL_POSITION_OUTSIDE )
            {
                xlegend = 0.;
                ylegend = legend_height;
            }
            else
            {
                plexit( "legend_position: internal logic error 1" );
            }
        }
        else if ( !( position & PL_POSITION_TOP ) && !( position & PL_POSITION_BOTTOM ) )
        {
            yorigin = 0.5;
            ylegend = 0.5 * legend_height;
            if ( position & PL_POSITION_INSIDE )
            {
                xlegend = -legend_width;
                *xsign  = -1.;
            }
            else if ( position & PL_POSITION_OUTSIDE )
            {
                xlegend = 0.;
            }
            else
            {
                plexit( "legend_position: internal logic error 2" );
            }
        }
        else if ( position & PL_POSITION_BOTTOM )
        {
            yorigin = 0.;
            if ( position & PL_POSITION_INSIDE )
            {
                xlegend = -legend_width;
                ylegend = legend_height;
                *xsign  = -1.;
            }
            else if ( position & PL_POSITION_OUTSIDE )
            {
                xlegend = 0.;
                ylegend = 0.;
                *ysign  = -1.;
            }
            else
            {
                plexit( "legend_position: internal logic error 3" );
            }
        }
        else
        {
            plexit( "legend_position: internal logic error 4" );
        }
    }
    else if ( !( position & PL_POSITION_RIGHT ) && !( position & PL_POSITION_LEFT ) )
    {
        xorigin = 0.5;
        xlegend = -0.5 * legend_width;
        if ( position & PL_POSITION_TOP )
        {
            yorigin = 1.;
            if ( position & PL_POSITION_INSIDE )
            {
                ylegend = 0.;
                *ysign  = -1.;
            }
            else if ( position & PL_POSITION_OUTSIDE )
            {
                ylegend = legend_height;
            }
            else
            {
                plexit( "legend_position: internal logic error 5" );
            }
        }
        else if ( position & PL_POSITION_BOTTOM )
        {
            yorigin = 0.;
            if ( position & PL_POSITION_INSIDE )
            {
                ylegend = legend_height;
            }
            else if ( position & PL_POSITION_OUTSIDE )
            {
                ylegend = 0.;
                *ysign  = -1.;
            }
            else
            {
                plexit( "legend_position: internal logic error 6" );
            }
        }
        else
        {
            plexit( "legend_position: internal logic error 7" );
        }
    }
    else if ( position & PL_POSITION_LEFT )
    {
        xorigin = 0.;
        if ( position & PL_POSITION_TOP )
        {
            yorigin = 1.;
            if ( position & PL_POSITION_INSIDE )
            {
                xlegend = 0.;
                ylegend = 0.;
                *ysign  = -1.;
            }
            else if ( position & PL_POSITION_OUTSIDE )
            {
                xlegend = -legend_width;
                ylegend = legend_height;
                *xsign  = -1.;
            }
            else
            {
                plexit( "legend_position: internal logic error 8" );
            }
        }
        else if ( !( position & PL_POSITION_TOP ) && !( position & PL_POSITION_BOTTOM ) )
        {
            yorigin = 0.5;
            ylegend = 0.5 * legend_height;
            if ( position & PL_POSITION_INSIDE )
            {
                xlegend = 0.;
            }
            else if ( position & PL_POSITION_OUTSIDE )
            {
                xlegend = -legend_width;
                *xsign  = -1.;
            }
            else
            {
                plexit( "legend_position: internal logic error 9" );
            }
        }
        else if ( position & PL_POSITION_BOTTOM )
        {
            yorigin = 0.;
            if ( position & PL_POSITION_INSIDE )
            {
                ylegend = legend_height;
                xlegend = 0.;
            }
            else if ( position & PL_POSITION_OUTSIDE )
            {
                xlegend = -legend_width;
                ylegend = 0.;
                *xsign  = -1.;
                *ysign  = -1.;
            }
            else
            {
                plexit( "legend_position: internal logic error 10" );
            }
        }
        else
        {
            plexit( "legend_position: internal logic error 11" );
        }
    }
    else
    {
        plexit( "legend_position: internal logic error 12" );
    }
    *x_legend_position = xorigin + xlegend;
    *y_legend_position = yorigin + ylegend;
}

//--------------------------------------------------------------------------
//! Obtain ratio of normalized subpage to mm coordinates in both x and y.
//!
//! @param x_subpage_per_mm Pointer to a location that contains (after the call) the x ratio.
//! @param y_subpage_per_mm Pointer to a location that contains (after the call) the y ratio.
//!

static void get_subpage_per_mm( PLFLT *x_subpage_per_mm, PLFLT *y_subpage_per_mm )
{
    // Normalized viewport limits
    PLFLT vxmin, vxmax, vymin, vymax;
    // Size of subpage in mm
    PLFLT mxmin, mxmax, mymin, mymax;
    // Viewport limits in world coordinates
    PLFLT wxmin, wxmax, wymin, wymax;
    plgvpsp( &vxmin, &vxmax, &vymin, &vymax );
    plgspa( &mxmin, &mxmax, &mymin, &mymax );
    plgvpw( &wxmin, &wxmax, &wymin, &wymax );
    *x_subpage_per_mm = ( wxmax - wxmin ) / ( ( vxmax - vxmin ) * ( mxmax - mxmin ) );
    *y_subpage_per_mm = ( wymax - wymin ) / ( ( vymax - vymin ) * ( mymax - mymin ) );
}

//--------------------------------------------------------------------------
//! Obtain character or symbol height in (y) subpage coordinates.
//!
//! @param ifcharacter TRUE obtain character height, FALSE obtain symbol
//! height.
//! @return character or symbol height.
//!

static PLFLT get_character_or_symbol_height( PLBOOL ifcharacter )
{
    // Character height in mm
    PLFLT default_mm, char_height_mm;
    PLFLT x_subpage_per_mm, y_subpage_per_mm;

    if ( ifcharacter )
    {
        plgchr( &default_mm, &char_height_mm );
    }
    else
    {
        default_mm     = plsc->symdef;
        char_height_mm = plsc->symht;
    }
    get_subpage_per_mm( &x_subpage_per_mm, &y_subpage_per_mm );
    return ( char_height_mm * y_subpage_per_mm );
}

//--------------------------------------------------------------------------
//! Convert from adopted X coordinate to normalized subpage X
//! coordinate.  See pllegend documentation for definition of adopted
//! coordinates.
//!
//! @param nx Adopted X coordinate.
//!

#define adopted_to_subpage_x( nx )    ( ( xdmin_adopted ) + ( nx ) * ( ( xdmax_adopted ) - ( xdmin_adopted ) ) )

//--------------------------------------------------------------------------
//! Convert from normalized subpage X coordinate to adopted X
//! coordinate.  See pllegend documentation for definition of adopted
//! coordinates.
//!
//! @param nx Normalized subpage X coordinate.
//!

#define subpage_to_adopted_x( nx )    ( ( nx - xdmin_adopted ) / ( ( xdmax_adopted ) - ( xdmin_adopted ) ) )

//--------------------------------------------------------------------------
//! Convert from adopted Y coordinate to normalized subpage Y
//! coordinate.  See pllegend documentation for definition of adopted
//! coordinates.
//!
//! @param ny Adopted Y coordinate.
//!

#define adopted_to_subpage_y( ny )    ( ( ydmin_adopted ) + ( ny ) * ( ( ydmax_adopted ) - ( ydmin_adopted ) ) )

//--------------------------------------------------------------------------
//! Convert from normalized subpage Y coordinate to adopted Y
//! coordinate.  See pllegend documentation for definition of adopted
//! coordinates.
//!
//! @param ny Normalized subpage Y coordinate.
//!

#define subpage_to_adopted_y( ny )    ( ( ny - ydmin_adopted ) / ( ( ydmax_adopted ) - ( ydmin_adopted ) ) )

//--------------------------------------------------------------------------
//! Plot legend using discretely annotated filled boxes, lines, and/or lines
//! of symbols.
//!
//! (N.B. the adopted coordinate system used for some of the parameters
//! is defined in the documentation of the position parameter.)
//!
//! @param p_legend_width Pointer to a location which contains (after
//! the call) the legend width in adopted coordinates.  This quantity
//! is calculated from the plot_width and text_offset arguments, the
//! ncolumn argument (possibly modified inside the routine depending
//! on the nlegend and nrow arguments), and the length (calculated
//! internally) of the longest text string.
//! @param p_legend_height Pointer to a location which contains (after
//! the call) the legend height in adopted coordinates.  This
//! quantity is calculated from the text_scale and text_spacing
//! arguments and the nrow argument (possibly modified inside the
//! routine depending on the nlegend and ncolumn arguments).
//! @param opt This variable contains bits which control the overall
//! legend.  If the PL_LEGEND_TEXT_LEFT bit is set, put the text area
//! on the left of the legend and the plotted area on the right.
//! Otherwise, put the text area on the right of the legend and the
//! plotted area on the left.  If the PL_LEGEND_BACKGROUND bit is set,
//! plot a (semi-transparent) background for the legend.  If the
//! PL_LEGEND_BOUNDING_BOX bit is set, plot a bounding box for the
//! legend.  If the PL_LEGEND_ROW_MAJOR bit is set and both (of the
//! possibly internally transformed) nrow > 1 and ncolumn > 1, then
//! plot the resulting array of legend entries in row-major order.
//! Otherwise, plot the legend entries in column-major order.
//! @param position This variable contains bits which control the
//! overall position of the legend and the definition of the adopted
//! coordinates used for positions.  The combination of the
//! PL_POSITION_LEFT, PL_POSITION_RIGHT, PL_POSITION_TOP,
//! PL_POSITION_BOTTOM, PL_POSITION_INSIDE, and PL_POSITION_OUTSIDE
//! bits specifies one of the 16 possible standard positions (the 4
//! corners and 4 side centers for both the inside and outside cases)
//! of the legend relative to the adopted coordinate system.  The
//! adopted coordinates are normalized viewport coordinates if the
//! PL_POSITION_VIEWPORT bit is set or normalized subpage coordinates
//! if the PL_POSITION_SUBPAGE bit is set.  Default position bits: If
//! none of PL_POSITION_LEFT, PL_POSITION_RIGHT, PL_POSITION_TOP, or
//! PL_POSITION_BOTTOM are set, then use the combination of
//! PL_POSITION_RIGHT and PL_POSITION_TOP.  If neither of
//! PL_POSITION_INSIDE or PL_POSITION_OUTSIDE is set, use
//! PL_POSITION_INSIDE.  If neither of PL_POSITION_VIEWPORT or
//! PL_POSITION_SUBPAGE is set, use PL_POSITION_VIEWPORT.
//! @param x X offset of the legend position in adopted coordinates
//! from the specified standard position of the legend.  For positive
//! x, the direction of motion away from the standard position is
//! inward/outward from the standard corner positions or standard left
//! or right positions if the PL_POSITION_INSIDE/PL_POSITION_OUTSIDE
//! bit is set in position.  For the standard top or bottom positions,
//! the direction of motion for positive x is toward positive X.
//! @param y Y offset of the legend position in adopted coordinates
//! from the specified standard position of the legend.  For positive
//! y, the direction of motion away from the standard position is
//! inward/outward from the standard corner positions or standard top
//! or bottom positions if the PL_POSITION_INSIDE/PL_POSITION_OUTSIDE
//! bit is set in position.  For the standard left or right positions,
//! the direction of motion for positive y is toward positive Y.
//! @param plot_width Horizontal width in adopted coordinates of the
//! plot area (where colored boxes, lines, and/or symbols are drawn in
//! the legend).
//! @param bg_color The cmap0 index of the background color for the legend
//! (PL_LEGEND_BACKGROUND).
//! @param bb_color The cmap0 index of the color of the bounding-box
//! line for the legend (PL_LEGEND_BOUNDING_BOX).
//! @param bb_style The pllsty style number for the bounding-box line
//! for the legend (PL_LEGEND_BOUNDING_BOX).
//! @param nrow Number of rows in the matrix used to render the
//! nlegend legend entries.  For internal transformations of nrow, see
//! further remarks under nlegend.
//! @param ncolumn Number of columns in the matrix used to render the
//! nlegend legend entries.  For internal transformations of ncolumn,
//! see further remarks under nlegend.
//! @param nlegend Number of legend entries.  The above nrow and
//! ncolumn values are transformed internally to be consistent with
//! nlegend.  If either nrow or ncolumn is non-positive it is replaced
//! by 1.  If the resulting product of nrow and ncolumn is less than
//! nlegend, the smaller of the two (or nrow, if nrow == ncolumn) is
//! increased so the product is >= nlegend.  Thus, for example, the
//! common nrow = 0, ncolumn = 0 case is transformed internally to
//! nrow = nlegend, ncolumn = 1; i.e., the usual case of a legend
//! rendered as a single column.
//! @param opt_array Array of nlegend values of options to control
//! each individual plotted area corresponding to a legend entry.  If
//! the PL_LEGEND_NONE bit is set, then nothing is plotted in the
//! plotted area.  If the PL_LEGEND_COLOR_BOX, PL_LEGEND_LINE, and/or
//! PL_LEGEND_SYMBOL bits are set, the plotted area corresponding to a
//! legend entry is specified with a colored box; a line; and/or a
//! line of symbols.
//! @param text_offset Offset of the text area from the plot area in
//! units of character width.
//! @param text_scale Character height scale for text annotations.
//! @param text_spacing Vertical spacing in units of the character
//! height from one legend entry to the next.
//! @param text_justification Justification parameter used for text
//! justification.  The most common values of text_justification are
//! 0., 0.5, or 1. corresponding to a text that is left justified,
//! centred, or right justified within the text area, but other values
//! are allowed as well.
//! @param text_colors Array of nlegend text colors (cmap0 indices).
//! @param text Array of nlegend pointers to null-terminated text
//! annotation strings.  Like other PLplot strings specified by the
//! user, the string must be UTF-8 (including its ascii subset) and
//! may include any of the PLplot text escapes.
//! @param box_colors Array of nlegend colors (cmap0 indices) for
//! the discrete colored boxes (PL_LEGEND_COLOR_BOX).
//! @param box_patterns Array of nlegend patterns (plpsty indices)
//! for the discrete colored boxes (PL_LEGEND_COLOR_BOX).
//! @param box_scales Array of nlegend scales (units of fraction of
//! character height) for the height of the discrete colored boxes
//! (PL_LEGEND_COLOR_BOX).
//! @param box_line_widths Array of nlegend line widths for the
//! patterns specified by box_patterns (PL_LEGEND_COLOR_BOX).
//! @param line_colors Array of nlegend line colors (cmap0 indices)
//! (PL_LEGEND_LINE).
//! @param line_styles Array of nlegend line styles (pllsty indices)
//! (PL_LEGEND_LINE).
//! @param line_widths Array of nlegend line widths (PL_LEGEND_LINE).
//! @param symbol_colors Array of nlegend symbol colors (cmap0
//! indices) (PL_LEGEND_SYMBOL).
//! @param symbol_scales Array of nlegend scale values for the
//! symbol height (PL_LEGEND_SYMBOL).
//! @param symbol_numbers Array of nlegend numbers of symbols to be
//! drawn across the width of the plotted area (PL_LEGEND_SYMBOL).
//! @param symbols Array of nlegend pointers to null-terminated
//! strings which represent the glyph for the symbol to be plotted
//! (PL_LEGEND_SYMBOL).  Like other PLplot strings specified by the
//! user, the string must be UTF-8 (including its ascii subset) and
//! may include any of the PLplot text escapes.
//!

void
c_pllegend( PLFLT *p_legend_width, PLFLT *p_legend_height,
            PLINT opt, PLINT position, PLFLT x, PLFLT y, PLFLT plot_width,
            PLINT bg_color, PLINT bb_color, PLINT bb_style,
            PLINT nrow, PLINT ncolumn,
            PLINT nlegend, const PLINT *opt_array,
            PLFLT text_offset, PLFLT text_scale, PLFLT text_spacing,
            PLFLT text_justification,
            const PLINT *text_colors, const char **text,
            const PLINT *box_colors, const PLINT *box_patterns,
            const PLFLT *box_scales, const PLINT *box_line_widths,
            const PLINT *line_colors, const PLINT *line_styles,
            const PLINT *line_widths,
            const PLINT *symbol_colors, const PLFLT *symbol_scales,
            const PLINT *symbol_numbers, const char **symbols )

{
    // Legend position
    PLFLT plot_x, plot_x_end, plot_x_subpage, plot_x_end_subpage;
    PLFLT plot_y, plot_y_subpage;
    PLFLT text_x, text_y, text_x_subpage, text_y_subpage;
    // Character height (normalized subpage coordinates)
    PLFLT character_height, character_width, symbol_width;
    // x, y-position of the current legend entry
    PLFLT ty, xshift, drow, dcolumn;
    // Positions of the legend entries
    PLFLT dxs, *xs, *ys, xl[2], yl[2], xbox[4], ybox[4];
    PLINT i, j;
    // Active attributes to be saved and restored afterward.
    PLINT col0_save       = plsc->icol0,
          line_style_save = plsc->line_style,
          line_width_save = plsc->width,
          pattern_save    = plsc->patt;
    PLFLT text_scale_save = plsc->chrht / plsc->chrdef;
    // Saved external world coordinates of viewport.
    PLFLT xwmin_save, xwmax_save, ywmin_save, ywmax_save;
    // Saved external normalized coordinates of viewport.
    // (These are actual values used only for the restore.)
    PLFLT xdmin_save, xdmax_save, ydmin_save, ydmax_save;
    // Limits of adopted coordinates used to calculate all coordinate
    // transformations.
    PLFLT xdmin_adopted, xdmax_adopted, ydmin_adopted, ydmax_adopted;

    PLFLT x_subpage_per_mm, y_subpage_per_mm, text_width0 = 0., text_width;
    PLFLT width_border, column_separation,
          legend_width, legend_height, legend_width_vc, legend_height_vc;
    PLFLT x_legend_position, y_legend_position, xsign, ysign;

    PLINT some_boxes         = 0, some_lines = 0, some_symbols = 0;
    PLINT max_symbol_numbers = 0;
    PLINT irow = 0, icolumn = 0;

    // Default nrow, ncolumn.
    nrow    = MAX( nrow, 1 );
    ncolumn = MAX( ncolumn, 1 );
    if ( nrow * ncolumn < nlegend )
    {
        // Make smaller one large enough to accomodate nlegend.
        if ( ncolumn < nrow )
            ncolumn = ( nlegend % nrow ) ? ( nlegend / nrow ) + 1 : nlegend / nrow;
        else
            nrow = ( nlegend % ncolumn ) ? ( nlegend / ncolumn ) + 1 : nlegend / ncolumn;
    }
    // fprintf(stdout, "nrow, ncolumn = %d, %d\n", nrow, ncolumn);

    // Default position flags and sanity checks for position flags.
    if ( !( position & PL_POSITION_RIGHT ) && !( position & PL_POSITION_LEFT ) && !( position & PL_POSITION_TOP ) && !( position & PL_POSITION_BOTTOM ) )
    {
        position = position | PL_POSITION_RIGHT | PL_POSITION_TOP;
    }
    else if ( ( position & PL_POSITION_RIGHT ) && ( position & PL_POSITION_LEFT ) )
    {
        plabort( "pllegend: PL_POSITION_RIGHT and PL_POSITION_LEFT cannot be simultaneously set." );
        return;
    }

    else if ( ( position & PL_POSITION_TOP ) && ( position & PL_POSITION_BOTTOM ) )
    {
        plabort( "pllegend: PL_POSITION_TOP and PL_POSITION_BOTTOM cannot be simultaneously set." );
        return;
    }

    if ( !( position & PL_POSITION_INSIDE ) && !( position & PL_POSITION_OUTSIDE ) )
    {
        position = position | PL_POSITION_INSIDE;
    }
    else if ( ( position & PL_POSITION_INSIDE ) && ( position & PL_POSITION_OUTSIDE ) )
    {
        plabort( "pllegend: PL_POSITION_INSIDE and PL_POSITION_OUTSIDE cannot be simultaneously set." );
        return;
    }

    if ( !( position & PL_POSITION_VIEWPORT ) && !( position & PL_POSITION_SUBPAGE ) )
    {
        position = position | PL_POSITION_VIEWPORT;
    }
    else if ( ( position & PL_POSITION_VIEWPORT ) && ( position & PL_POSITION_SUBPAGE ) )
    {
        plabort( "pllegend: PL_POSITION_VIEWPORT and PL_POSITION_SUBPAGE cannot be simultaneously set." );
        return;
    }

    // xdmin_save, etc., are the actual external relative viewport
    // coordinates within the current sub-page used only for
    // restoration at the end.
    plgvpsp( &xdmin_save, &xdmax_save, &ydmin_save, &ydmax_save );

    // Choose adopted coordinates.
    if ( position & PL_POSITION_SUBPAGE )
        plvpor( 0., 1., 0., 1. );

    // xdmin_adopted, etc., are the adopted coordinates within the
    // current sub-page used for all coordinate transformations.
    // If position & PL_POSITION_VIEWPORT is true, these coordinates
    // are the external relative viewport coordinates.
    // If position & PL_POSITION_SUBPAGE is true, these
    // coordinates are the relative subpage coordinates.

    plgvpsp( &xdmin_adopted, &xdmax_adopted, &ydmin_adopted, &ydmax_adopted );

    // xwmin_save, etc., are the external world coordinates corresponding
    // to the external viewport boundaries.
    plgvpw( &xwmin_save, &xwmax_save, &ywmin_save, &ywmax_save );

    // Internal viewport corresponds to sub-page so that all legends will
    // be clipped at sub-page boundaries.
    plvpor( 0., 1., 0., 1. );

    // Internal window coordinates are the same as normalized internal
    // viewport coordinates which are the same as normalized subpage coordinates.
    plwind( 0., 1., 0., 1. );

    for ( i = 0; i < nlegend; i++ )
    {
        if ( opt_array[i] & PL_LEGEND_COLOR_BOX )
            some_boxes = 1;
        if ( opt_array[i] & PL_LEGEND_LINE )
            some_lines = 1;
        if ( opt_array[i] & PL_LEGEND_SYMBOL )
        {
            max_symbol_numbers = MAX( max_symbol_numbers, symbol_numbers[i] );
            some_symbols       = 1;
        }
    }

    // Get character height and width in normalized subpage coordinates.
    character_height = get_character_or_symbol_height( TRUE );
    character_width  = character_height;

    // Calculate maximum width of text area.
    plschr( 0., text_scale );
    for ( i = 0; i < nlegend; i++ )
    {
        // units are mm.
        text_width0 = MAX( text_width0, plstrl( text[i] ) );
    }
    get_subpage_per_mm( &x_subpage_per_mm, &y_subpage_per_mm );

    // units are normalized subpage coordinates.
    text_width0 = x_subpage_per_mm * text_width0;

    // Allow gap on end closest to legend plot.
    text_width = text_width0 + text_offset * character_width;

    // Allow small border area where only the background is plotted
    // for left and right of legend.  0.4 seems to be a reasonable factor
    // that gives a good-looking result.
    width_border = 0.4 * character_width;
    // Separate columns (if any) by 2.0 * character_width.
    column_separation = 2.0 * character_width;

    // Total width and height of legend area in normalized subpage coordinates.
    legend_width = 2. * width_border + ( ncolumn - 1 ) * column_separation +
                   ncolumn * ( text_width +
                               adopted_to_subpage_x( plot_width ) - adopted_to_subpage_x( 0. ) );
    legend_height = nrow * text_spacing * character_height;

    // Total width and height of legend area in adopted coordinates.

    legend_width_vc  = subpage_to_adopted_x( legend_width ) - subpage_to_adopted_x( 0. );
    legend_height_vc = subpage_to_adopted_y( legend_height ) - subpage_to_adopted_y( 0. );
    *p_legend_width  = legend_width_vc;
    *p_legend_height = legend_height_vc;

    // dcolumn is the spacing from one column to the next and
    // drow is the spacing from one row to the next.
    dcolumn = column_separation + text_width +
              adopted_to_subpage_x( plot_width ) - adopted_to_subpage_x( 0. );
    drow = text_spacing * character_height;

    legend_position( position, legend_width_vc, legend_height_vc, &x_legend_position, &y_legend_position, &xsign, &ysign );
    plot_x     = x * xsign + x_legend_position;
    plot_y     = y * ysign + y_legend_position;
    plot_x_end = plot_x + plot_width;
    // Normalized subpage coordinates for legend plots
    plot_x_subpage     = adopted_to_subpage_x( plot_x );
    plot_y_subpage     = adopted_to_subpage_y( plot_y );
    plot_x_end_subpage = adopted_to_subpage_x( plot_x_end );

    // Get normalized subpage positions of the start of the legend text
    text_x         = plot_x_end;
    text_y         = plot_y;
    text_x_subpage = adopted_to_subpage_x( text_x ) +
                     text_offset * character_width;
    text_y_subpage = adopted_to_subpage_y( text_y );

    if ( opt & PL_LEGEND_BACKGROUND )
    {
        PLFLT xbg[4] = {
            plot_x_subpage,
            plot_x_subpage,
            plot_x_subpage + legend_width,
            plot_x_subpage + legend_width,
        };
        PLFLT ybg[4] = {
            plot_y_subpage,
            plot_y_subpage - legend_height,
            plot_y_subpage - legend_height,
            plot_y_subpage,
        };
        plpsty( 0 );
        plcol0( bg_color );
        plfill( 4, xbg, ybg );
        plcol0( col0_save );
    }

    if ( opt & PL_LEGEND_BOUNDING_BOX )
    {
        PLFLT xbb[5] = {
            plot_x_subpage,
            plot_x_subpage,
            plot_x_subpage + legend_width,
            plot_x_subpage + legend_width,
            plot_x_subpage,
        };
        PLFLT ybb[5] = {
            plot_y_subpage,
            plot_y_subpage - legend_height,
            plot_y_subpage - legend_height,
            plot_y_subpage,
            plot_y_subpage,
        };
        pllsty( bb_style );
        plcol0( bb_color );
        plline( 5, xbb, ybb );
        plcol0( col0_save );
    }

    if ( opt & PL_LEGEND_TEXT_LEFT )
    {
        // text area on left, plot area on right.
        text_x_subpage      = plot_x_subpage;
        plot_x_subpage     += text_width;
        plot_x_end_subpage += text_width;
    }
    // adjust border after background is drawn.
    plot_x_subpage     += width_border;
    plot_x_end_subpage += width_border;
    text_x_subpage     += width_border;

    if ( some_symbols )
    {
        max_symbol_numbers = MAX( 2, max_symbol_numbers );
        if ( ( ( xs = (PLFLT *) malloc( max_symbol_numbers * sizeof ( PLFLT ) ) ) == NULL ) ||
             ( ( ys = (PLFLT *) malloc( max_symbol_numbers * sizeof ( PLFLT ) ) ) == NULL ) )
        {
            plexit( "pllegend: Insufficient memory" );
        }

        // Get symbol width in normalized subpage coordinates if symbols are plotted to
        // adjust ends of line of symbols.
        // AWI, no idea why must use 0.5 factor to get ends of symbol lines
        // to line up approximately correctly with plotted legend lines.
        // Factor should be unity.
        symbol_width = 0.5 * get_character_or_symbol_height( TRUE );
    }

    // Draw each legend entry
    for ( i = 0; i < nlegend; i++ )
    {
        // y position of text, lines, symbols, and/or centre of cmap0 box.
        ty     = text_y_subpage - ( (double) irow + 0.5 ) * drow;
        xshift = (double) icolumn * dcolumn;
        // Label/name for the legend
        plcol0( text_colors[i] );
        plschr( 0., text_scale );
        plptex( text_x_subpage + xshift + text_justification * text_width0, ty, 0.1, 0.0, text_justification, text[i] );

        if ( !( opt_array[i] & PL_LEGEND_NONE ) )
        {
            if ( opt_array[i] & PL_LEGEND_COLOR_BOX )
            {
                plcol0( box_colors[i] );
                plpsty( box_patterns[i] );
                plwid( box_line_widths[i] );
                xbox[0] = plot_x_subpage + xshift;
                xbox[1] = xbox[0];
                xbox[2] = plot_x_end_subpage + xshift;
                xbox[3] = xbox[2];
                ybox[0] = ty + 0.5 * drow * box_scales[i];
                ybox[1] = ty - 0.5 * drow * box_scales[i];
                ybox[2] = ty - 0.5 * drow * box_scales[i];
                ybox[3] = ty + 0.5 * drow * box_scales[i];
                plfill( 4, xbox, ybox );
                plwid( line_width_save );
            }
            if ( opt_array[i] & PL_LEGEND_LINE )
            {
                plcol0( line_colors[i] );
                pllsty( line_styles[i] );
                plwid( line_widths[i] );
                xl[0] = plot_x_subpage + xshift;
                xl[1] = plot_x_end_subpage + xshift;
                yl[0] = ty;
                yl[1] = ty;
                plline( 2, xl, yl );
                pllsty( line_style_save );
                plwid( line_width_save );
            }

            if ( opt_array[i] & PL_LEGEND_SYMBOL )
            {
                plcol0( symbol_colors[i] );
                plschr( 0., symbol_scales[i] );
                dxs = ( plot_x_end_subpage - plot_x_subpage - symbol_width ) / (double) ( MAX( symbol_numbers[i], 2 ) - 1 );
                for ( j = 0; j < symbol_numbers[i]; j++ )
                {
                    xs[j] = plot_x_subpage + xshift +
                            0.5 * symbol_width + dxs * (double) j;
                    ys[j] = ty;
                }
                plstring( symbol_numbers[i], xs, ys, symbols[i] );
            }
        }

        // Set irow, icolumn for next i value.
        if ( opt & PL_LEGEND_ROW_MAJOR )
        {
            icolumn++;
            if ( icolumn >= ncolumn )
            {
                icolumn = 0;
                irow++;
            }
        }
        else
        {
            irow++;
            if ( irow >= nrow )
            {
                irow = 0;
                icolumn++;
            }
        }
    }
    if ( some_symbols )
    {
        free( xs );
        free( ys );
    }

    // Restore
    plcol0( col0_save );
    plschr( 0., text_scale_save );
    plpsty( pattern_save );
    plvpor( xdmin_save, xdmax_save, ydmin_save, ydmax_save );
    plwind( xwmin_save, xwmax_save, ywmin_save, ywmax_save );

    return;
}

#define PL_COLORBAR_NEW_API
#ifdef PL_COLORBAR_NEW_API
// Code version with reorganized API for plcolorbar

//--------------------------------------------------------------------------
//! Remove specified ascii characters from null-terminated string.
//!
//! @param string Null-terminated string where specified characters
//! (if present) are removed.
//! @param characters Null-terminated string consisting of ascii characters
//! to be removed from string.

void
static remove_characters( char *string, const char *characters )
{
    size_t length = strlen(string);
    size_t prefix_length = strcspn( string, characters );
    if ( prefix_length < length )
    {
        // Remove first matching character by shifting tail of string
        // (including null-terminator) down by one.
        memmove( string+prefix_length, string+prefix_length+1, length - prefix_length );
        // Recurse to remove any remaining specified characters.
        remove_characters( string, characters );
    }
}

//--------------------------------------------------------------------------
//! Draw triangular end-caps for color bars.
//!
//! @param opt If the PL_COLORBAR_NOEDGE/EDGE bit is set, draw (no edge)/(an edge) around
//! the triangle.
//! @param orientation This variable defines the orientation of
//! the triangle.  The triangle points to the right, up, left, or down
//! if orientation contains PL_COLORBAR_ORIENT_RIGHT,
//! PL_COLORBAR_ORIENT_TOP, PL_COLORBAR_ORIENT_LEFT, or
//! PL_COLORBAR_ORIENT_BOTTOM bits.
//! @param xmin Minimum X coordinate of rectangle inscribing the triangle.
//! @param xmax Maximum X coordinate of rectangle inscribing the triangle.
//! @param ymin Minimum Y coordinate of rectangle inscribing the triangle.
//! @param ymax Minimum Y coordinate of rectangle inscribing the triangle.
//! @param color Color (color palette 1) used to fill the end cap.
//!

void
static draw_cap( PLINT opt, PLINT orientation, PLFLT xmin, PLFLT xmax,
          PLFLT ymin,PLFLT ymax, PLFLT color )
{
    // Save current drawing color.
    PLINT col0_save = plsc->icol0;

    // Save window and viewport
    // Saved normalized coordinates of viewport.
    PLFLT xdmin_save, xdmax_save, ydmin_save, ydmax_save;
    // Saved world coordinates of viewport.
    PLFLT xwmin_save, xwmax_save, ywmin_save, ywmax_save;
    // Points for the triangle
    PLFLT xs[3];
    PLFLT ys[3];

    plgvpsp( &xdmin_save, &xdmax_save, &ydmin_save, &ydmax_save );
    plgvpw( &xwmin_save, &xwmax_save, &ywmin_save, &ywmax_save );

    // The viewport is the specified rectangle that inscribes the
    // triangle.  The world coordinates are chosen to make drawing of
    // that triangle convenient.
    plvpor( xmin, xmax, ymin, ymax );
    plwind( 0.0, 1.0, 0.0, 1.0 );

    if ( orientation == PL_COLORBAR_ORIENT_RIGHT )
    {
            xs[0] = 0.;
            ys[0] = 0.;
            xs[1] = 1.;
            ys[1] = 0.5;
            xs[2] = 0.;
            ys[2] = 1.;
    }
    else if ( orientation == PL_COLORBAR_ORIENT_TOP )
    {
            xs[0] = 1.;
            ys[0] = 0.;
            xs[1] = 0.5;
            ys[1] = 1.;
            xs[2] = 0.;
            ys[2] = 0.;
    }
    else if ( orientation == PL_COLORBAR_ORIENT_LEFT )
    {
            xs[0] = 1.;
            ys[0] = 1.;
            xs[1] = 0.;
            ys[1] = 0.5;
            xs[2] = 1.;
            ys[2] = 0.;
    }
    else if ( orientation == PL_COLORBAR_ORIENT_BOTTOM )
    {
            xs[0] = 0.;
            ys[0] = 1.;
            xs[1] = 0.5;
            ys[1] = 0.;
            xs[2] = 1.;
            ys[2] = 1.;
    }
    else
    {
      plexit( "draw_cap: internal error. Incorrect orientation");
    }

    plcol1( color );
    plfill( 3, xs, ys );
    // Restore the drawing color
    plcol0( col0_save );

    // Draw cap outline
    if ( opt & PL_COLORBAR_EDGE)
        plline( 3, xs, ys );

    // Restore window and viewport
    plvpor( xdmin_save, xdmax_save, ydmin_save, ydmax_save );
    plwind( xwmin_save, xwmax_save, ywmin_save, ywmax_save );
}

//--------------------------------------------------------------------------
//! Plot color bar for image, shade or gradient plots.
//!
//! @param opt This variable contains bits which control the overall
//! colorbar.  The orientation (direction of the maximum value) of the
//! colorbar is specified with PL_COLORBAR_ORIENT_(RIGHT, TOP, LEFT,
//! BOTTOM).  If none of those bits are specified, the default
//! orientation is toward the top, i.e., a vertical colorbar.  Whether
//! the edges of the colour box and caps is not drawn or drawn (for
//! the current cmap0 color index) depends on PL_COLORBAR_(NOEDGE,
//! EDGE).  If neither of these bits are specified, then by default
//! PL_COLORBAR_NOEDGE is assumed.  The type of colorbar can be
//! specified with PL_COLORBAR_SHADE, PL_COLORBAR_IMAGE or
//! PL_COLORBAR_GRADIENT.  The position of the (optional) label/title
//! can be specified with PL_COLORBAR_LABEL_(LEFT|RIGHT|TOP|BOTTOM).
//! If no label position is specified then no label will be drawn.
//! End-caps for the colorbar can added with PL_COLORBAR_CAP_LOW and
//! PL_COLORBAR_CAP_HIGH.  If a particular colorbar cap option is not
//! specified then no cap will be drawn for that end.  As a special
//! case for PL_COLORBAR_SHADE, the option PL_COLORBAR_SHADE_LABEL can
//! be specified.  If this option is provided then any tick marks and
//! tick labels will be placed at the breaks between shaded segments.
//! TODO: This should be expanded to support custom placement of tick
//! marks and tick labels at custom value locations for any colorbar
//! type.
//! @param position This variable defines the placement of the colorbar on the
//! subpage.  The position can be one of PL_POSITION_TOP,
//! PL_POSITION_BOTTOM, PL_POSITION_LEFT or PL_POSITION_RIGHT.  The colorbar
//! will be drawn perpendicular to the given side of the subpage.
//! @param x Colorbar displacement distance along/away from the horizonal axis
//! in normalized subpage coordinates.
//! @param y Colorbar displacement distance along/away from the vertical axis
//! in normalized subpage coordinates.
//! @param x_length Length of the colorbar in the X direction in
//! adopted coordinates.
//! @param y_length Length of the colorbar in the Y direction in
//! adopted coordinates.
//! @param low_cap_color Color of the low-end color bar cap, if it is drawn.
//! @param high_cap_color Color of the high-end color bar cap, if it is drawn.
//! @param cont_color Contour color for PL_COLORBAR_SHADE plots.  This is
//! passed directly to plshades, so it will be interpreted according to the
//! design of plshades.
//! @param cont_width Contour width for PL_COLORBAR_SHADE plots.  This is
//! passed directly to plshades, so it will be interpreted according to the
//! design of plshades.
//! @param ticks Spacing of major ticks, as for plbox.
//! @param sub_ticks Number of subticks, as for plbox.
//! @param axis_opts Axis options for the colorbar's major axis, as for plbox.
//! @param label Text label for the colorbar.  No title is drawn if no label
//! position is specified in pos.
//! @param n_values Number of elements in the values array.
//! @param colors Colors (color map 1) used to draw the colorbar.  If this is a
//! PL_COLORBAR_SHADE bar then there should be one entry per break between
//! segments.  If this
//! is a PL_COLORBAR_IMAGE or PL_COLORBAR_GRADIENT bar then there should be two
//! elements - one to specify the high end and one to specify the low end.
//! This should have (n_values - 1) elements.
//! @param values Numeric values for the data range represented by the
//! colorbar.  For PL_COLORBAR_SHADE, this should include one value per break
//! between segments.  For PL_COLORBAR_IMAGE and PL_COLORBAR_GRADIENT this
//! includes two values, one for the maximum value on the scale and one for the
//! minimum value.
//!

void
c_plcolorbar( PLINT opt, PLINT position,
              PLFLT x, PLFLT y, PLFLT x_length, PLFLT y_length,
              PLFLT low_cap_color, PLFLT high_cap_color,
              PLINT cont_color, PLINT cont_width,
              PLFLT ticks, PLINT sub_ticks,
              const char *axis_opts, const char *label,
              PLINT n_values, const PLFLT *values )
{
    // Justification of label text
    PLFLT just;

    // Min and max values
    // Assumes that the values array is sorted from smallest to largest
    // OR from largest to smallest.
    PLFLT min_value, max_value;
    // Height of the cap in normalized coordinates
    // ToDo: Use better value related to size of color bar.
    PLFLT cap_height = 0.05;

    // Min and max colors
    PLFLT min_color, max_color;

    // Saved normalized coordinates of viewport.
    PLFLT xdmin_save, xdmax_save, ydmin_save, ydmax_save;

    // Saved world coordinates of viewport.
    PLFLT xwmin_save, xwmax_save, ywmin_save, ywmax_save;

    // Active attributes to be saved and restored afterward.
    PLINT col0_save       = plsc->icol0;

    // Position of the color bar in normalized viewport (= normalized subpage
    // coordinates).
    PLFLT vx_min, vx_max, vy_min, vy_max;
    PLFLT wx_min, wx_max, wy_min, wy_max;

    // The data to plot
    PLFLT **color_data;

    // Setting up the data for display
    PLINT i, j, ni, nj, n_steps;
    PLFLT step_size;

    // How far away from the axis should the label be drawn?
    PLFLT label_offset;

    // For building axis option string
    PLINT      max_opts = 25;
    char       opt_string[max_opts];
    const char *tick_label_string, *edge_string;
    size_t length_axis_opts = strlen(axis_opts);
    char *local_axis_opts;

    // Draw a title
    char perp;

    // Default orientation.
    if ( !( opt & PL_COLORBAR_ORIENT_RIGHT ||
            opt & PL_COLORBAR_ORIENT_TOP ||
            opt & PL_COLORBAR_ORIENT_LEFT ||
            opt & PL_COLORBAR_ORIENT_BOTTOM ) )
    {
      if ( position & PL_POSITION_LEFT  || position & PL_POSITION_RIGHT )
        opt = opt | PL_COLORBAR_ORIENT_TOP;
      else
        opt = opt | PL_COLORBAR_ORIENT_RIGHT;
    }

    // Do not draw edges of color box and caps by default.
    if ( !( opt & PL_COLORBAR_NOEDGE || opt & PL_COLORBAR_EDGE ) )
        opt = opt | PL_COLORBAR_NOEDGE;

    // local_axis_opts is local version that can be modified from
    // const input version.
    if ( ( local_axis_opts = (char *) malloc( (length_axis_opts + 1) * sizeof ( char ) ) ) == NULL )
    {
        plexit( "plcolorbar: Insufficient memory" );
    }
    strcpy(local_axis_opts, axis_opts);

    
    // Sanity checking on local_axis_opts to remove all control characters
    // that are specified by other means inside this routine.
    remove_characters(local_axis_opts, "BbCcMmNnUuWw");

    min_value = values[0];
    max_value = values[ n_values - 1 ];

    // Assumes that the colors array is sorted from smallest to largest.
    plgcmap1_range( &min_color, &max_color );

    plgvpsp( &xdmin_save, &xdmax_save, &ydmin_save, &ydmax_save );
    plgvpw( &xwmin_save, &xwmax_save, &ywmin_save, &ywmax_save );

    // Specify the proper viewport ranges along the requested side
    // of the subpage
    if ( position & PL_POSITION_LEFT )
    {
        vx_min = x;
        vy_min = y;
    }
    else if ( position & PL_POSITION_RIGHT )
    {
        vx_min = 1.0 - x - x_length;
        vy_min = y;
    }
    else if ( position & PL_POSITION_TOP )
    {
        vx_min = x;
        vy_min = 1.0 - y - y_length;
    }
    else if ( position & PL_POSITION_BOTTOM )
    {
        vx_min = x;
        vy_min = y;
    }
    else
    {
        plabort( "plcolorbar: Invalid PL_POSITION_* bits" );
    }
    vx_max = vx_min + x_length;
    vy_max = vy_min + y_length;

    // Specify the proper window ranges depending on orientation.
    if ( opt & PL_COLORBAR_ORIENT_RIGHT )
    {
        wx_min = min_value;
        wx_max = max_value;
        wy_min = 0.0;
        wy_max = 1.0;
    }
    else if ( opt & PL_COLORBAR_ORIENT_TOP )
    {
        wx_min = 0.0;
        wx_max = 1.0;
        wy_min = min_value;
        wy_max = max_value;
    }
    else if ( opt & PL_COLORBAR_ORIENT_LEFT )
    {
        wx_min = max_value;
        wx_max = min_value;
        wy_min = 0.0;
        wy_max = 1.0;
    }
    else if ( opt & PL_COLORBAR_ORIENT_BOTTOM)
    {
        wx_min = 0.0;
        wx_max = 1.0;
        wy_min = max_value;
        wy_max = min_value;
    }
    else
    {
        plabort( "plcolorbar: Invalid PL_COLORBAR_ORIENT_* bits" );
    }

    plvpor( vx_min, vx_max, vy_min, vy_max );
    plwind( wx_min, wx_max, wy_min, wy_max );

    // What kind of color bar are we making?
    if ( opt & PL_COLORBAR_IMAGE )
    {
        // Interpolate
        // TODO: Should this be decided with an extra opt option instead of by
        // counting n_values?
        if ( n_values == 2 )
        {
            // Use the same number of steps as there are steps in
            // color palette 1.
            // TODO: Determine a better way to specify the steps here?
            n_steps   = plsc->ncol1;
            step_size = ( max_value - min_value ) / (PLFLT) n_steps;
            if ( opt & PL_COLORBAR_ORIENT_RIGHT )
            {
                ni = n_steps;
                nj = 2;
                plAlloc2dGrid( &color_data, ni, nj );
                for ( i = 0; i < ni; i++ )
                {
                    for ( j = 0; j < nj; j++ )
                    {
                        color_data[i][j] = min_value + (PLFLT) i * step_size;
                    }
                }
            }
            else if ( opt & PL_COLORBAR_ORIENT_TOP )
            {
                ni = 2;
                nj = n_steps;
                plAlloc2dGrid( &color_data, ni, nj );
                for ( i = 0; i < ni; i++ )
                {
                    for ( j = 0; j < nj; j++ )
                    {
                        color_data[i][j] = min_value + (PLFLT) j * step_size;
                    }
                }
            }
            else if ( opt & PL_COLORBAR_ORIENT_LEFT )
            {
                ni = n_steps;
                nj = 2;
                plAlloc2dGrid( &color_data, ni, nj );
                for ( i = 0; i < ni; i++ )
                {
                    for ( j = 0; j < nj; j++ )
                    {
                        color_data[i][j] = max_value - (PLFLT) i * step_size;
                    }
                }
            }
            else if ( opt & PL_COLORBAR_ORIENT_BOTTOM )
            {
                ni = 2;
                nj = n_steps;
                plAlloc2dGrid( &color_data, ni, nj );
                for ( i = 0; i < ni; i++ )
                {
                    for ( j = 0; j < nj; j++ )
                    {
                        color_data[i][j] = max_value - (PLFLT) j * step_size;
                    }
                }
            }
            else
            {
                plabort( "plcolorbar: Invalid orientation bits" );
            }
        }
        // No interpolation - use values array as-is
        else
        {
            n_steps = n_values;
            // Use the provided values in this case.
            if ( opt & PL_COLORBAR_ORIENT_RIGHT )
            {
                ni = n_steps;
                nj = 2;
                plAlloc2dGrid( &color_data, ni, nj );
                for ( i = 0; i < ni; i++ )
                {
                    for ( j = 0; j < nj; j++ )
                    {
                        color_data[i][j] = values[i];
                    }
                }
            }
            else if ( opt & PL_COLORBAR_ORIENT_TOP )
            {
                ni = 2;
                nj = n_steps;
                plAlloc2dGrid( &color_data, ni, nj );
                for ( i = 0; i < ni; i++ )
                {
                    for ( j = 0; j < nj; j++ )
                    {
                        color_data[i][j] = values[j];
                    }
                }
            }
            else if ( opt & PL_COLORBAR_ORIENT_LEFT )
            {
                ni = n_steps;
                nj = 2;
                plAlloc2dGrid( &color_data, ni, nj );
                for ( i = 0; i < ni; i++ )
                {
                    for ( j = 0; j < nj; j++ )
                    {
                        color_data[i][j] = values[ni - 1 - i];
                    }
                }
            }
            else if ( opt & PL_COLORBAR_ORIENT_BOTTOM )
            {
                ni = 2;
                nj = n_steps;
                plAlloc2dGrid( &color_data, ni, nj );
                for ( i = 0; i < ni; i++ )
                {
                    for ( j = 0; j < nj; j++ )
                    {
                        color_data[i][j] = values[nj - 1 - j];
                    }
                }
            }
            else
            {
                plabort( "plcolorbar: Invalid side" );
            }
        }
        // Draw the color bar
        plimage( (const PLFLT **) color_data, ni, nj, wx_min, wx_max, wy_min, wy_max,
            min_value, max_value, wx_min, wx_max, wy_min, wy_max );
        plFree2dGrid( color_data, ni, nj );
    }
    else if ( opt & PL_COLORBAR_SHADE )
    {
        // Transform grid
        // The transform grid is used to make the size of the shaded segments
        // scale relative to other segments.  For example, if segment A
        // makes up 10% of the scale and segment B makes up 20% of the scale
        // then segment B will be twice the length of segment A.
        PLcGrid grid;
        PLFLT   grid_axis[2] = { 0.0, 1.0 };
        n_steps = n_values;
        // Use the provided values.
        if ( opt & PL_COLORBAR_ORIENT_RIGHT )
        {
            grid.xg = (PLFLT *) values;
            grid.yg = grid_axis;
            grid.nx = n_steps;
            grid.ny = 2;
            ni      = n_steps;
            nj      = 2;
            plAlloc2dGrid( &color_data, ni, nj );
            for ( i = 0; i < ni; i++ )
            {
                for ( j = 0; j < nj; j++ )
                {
                    color_data[i][j] = values[i];
                }
            }
        }
        else if ( opt & PL_COLORBAR_ORIENT_TOP )
        {
            grid.xg = grid_axis;
            grid.yg = (PLFLT *) values;
            grid.nx = 2;
            grid.ny = n_steps;
            ni      = 2;
            nj      = n_steps;
            plAlloc2dGrid( &color_data, ni, nj );
            for ( i = 0; i < ni; i++ )
            {
                for ( j = 0; j < nj; j++ )
                {
                    color_data[i][j] = values[j];
                }
            }
        }
        else if ( opt & PL_COLORBAR_ORIENT_LEFT )
        {
            grid.xg = (PLFLT *) values;
            grid.yg = grid_axis;
            grid.nx = n_steps;
            grid.ny = 2;
            ni      = n_steps;
            nj      = 2;
            plAlloc2dGrid( &color_data, ni, nj );
            for ( i = 0; i < ni; i++ )
            {
                for ( j = 0; j < nj; j++ )
                {
                    color_data[i][j] = values[ni - 1 - i];
                }
            }
        }
        else if ( opt & PL_COLORBAR_ORIENT_BOTTOM )
        {
            grid.xg = grid_axis;
            grid.yg = (PLFLT *) values;
            grid.nx = 2;
            grid.ny = n_steps;
            ni      = 2;
            nj      = n_steps;
            plAlloc2dGrid( &color_data, ni, nj );
            for ( i = 0; i < ni; i++ )
            {
                for ( j = 0; j < nj; j++ )
                {
                    color_data[i][j] = values[nj - 1 - j];
                }
            }
        }
        else
        {
            plabort( "plcolorbar: Invalid orientation" );
        }

        // Draw the color bar
        plshades( (const PLFLT **) color_data, ni, nj, NULL, wx_min, wx_max, wy_min, wy_max,
            values, n_steps, 0, cont_color, cont_width, plfill, TRUE,
            pltr1, (void *) ( &grid ) );
        plFree2dGrid( color_data, ni, nj );
    }
    else if ( opt & PL_COLORBAR_GRADIENT )
    {
        PLFLT xs[4], ys[4];
        PLFLT angle;
        xs[0] = wx_min;
        ys[0] = wy_min;
        xs[1] = wx_max;
        ys[1] = wy_min;
        xs[2] = wx_max;
        ys[2] = wy_max;
        xs[3] = wx_min;
        ys[3] = wy_max;
        // Make sure the gradient runs in the proper direction
        if ( opt & PL_COLORBAR_ORIENT_RIGHT )
        {
            angle = 0.0;
        }
        else if ( opt & PL_COLORBAR_ORIENT_TOP )
        {
            angle = 90.0;
        }
        else if ( opt & PL_COLORBAR_ORIENT_LEFT )
        {
            angle = 180.0;
        }
        else if ( opt & PL_COLORBAR_ORIENT_BOTTOM )
        {
            angle = 270.0;
        }
        else
        {
            plabort( "plcolorbar: Invalid orientation" );
        }
        plgradient( 4, xs, ys, angle );
    }

    // Restore the previous drawing color to use for outlines and text
    plcol0( col0_save );

    label_offset = 0.0;

    // Draw end-caps
    if ( opt & PL_COLORBAR_CAP_LOW )
    {
        // Add an extra offset for the label so it does not bump in to the
        // cap if the label is placed on the same side as the cap.
        if ( ( opt & PL_COLORBAR_ORIENT_RIGHT && opt & PL_COLORBAR_LABEL_LEFT ) ||
             ( opt & PL_COLORBAR_ORIENT_TOP && opt & PL_COLORBAR_LABEL_BOTTOM ) ||
             ( opt & PL_COLORBAR_ORIENT_LEFT && opt & PL_COLORBAR_LABEL_RIGHT ) ||
             ( opt & PL_COLORBAR_ORIENT_BOTTOM && opt & PL_COLORBAR_LABEL_TOP ) )
        {
            label_offset += 2.5;
        }
        // Draw a filled triangle (cap/arrow) at the low end of the scale
        if ( opt & PL_COLORBAR_ORIENT_RIGHT )
        {
          draw_cap( opt, PL_COLORBAR_ORIENT_LEFT, vx_min - cap_height, vx_min, vy_min, vy_max, low_cap_color );
        }
        else if ( opt & PL_COLORBAR_ORIENT_TOP )
        {
          draw_cap( opt, PL_COLORBAR_ORIENT_BOTTOM, vx_min, vx_max, vy_min - cap_height, vy_min, low_cap_color );
        }
        else if ( opt & PL_COLORBAR_ORIENT_LEFT )
        {
          draw_cap( opt, PL_COLORBAR_ORIENT_RIGHT, vx_max, vx_max + cap_height, vy_min, vy_max, low_cap_color );
        }
        else if ( opt & PL_COLORBAR_ORIENT_BOTTOM )
        {
          draw_cap( opt, PL_COLORBAR_ORIENT_TOP, vx_min, vx_max, vy_max, vy_max + cap_height, low_cap_color );
        }
    }
    if ( opt & PL_COLORBAR_CAP_HIGH )
    {
        // Add an extra offset for the label so it does not bump in to the
        // cap if the label is placed on the same side as the cap.
        if ( ( opt & PL_COLORBAR_ORIENT_RIGHT && opt & PL_COLORBAR_LABEL_RIGHT ) ||
             ( opt & PL_COLORBAR_ORIENT_TOP && opt & PL_COLORBAR_LABEL_TOP ) ||
             ( opt & PL_COLORBAR_ORIENT_LEFT && opt & PL_COLORBAR_LABEL_LEFT ) ||
             ( opt & PL_COLORBAR_ORIENT_BOTTOM && opt & PL_COLORBAR_LABEL_BOTTOM ) )
        {
            label_offset += 2.5;
        }
        // Draw a filled triangle (cap/arrow) at the high end of the scale
        if ( opt & PL_COLORBAR_ORIENT_RIGHT )
        {
          draw_cap( opt, PL_COLORBAR_ORIENT_RIGHT, vx_max, vx_max + cap_height, vy_min, vy_max, high_cap_color );
        }
        else if ( opt & PL_COLORBAR_ORIENT_TOP )
        {
          draw_cap( opt, PL_COLORBAR_ORIENT_TOP, vx_min, vx_max, vy_max, vy_max + cap_height, high_cap_color );
        }
        if ( opt & PL_COLORBAR_ORIENT_LEFT )
        {
          draw_cap( opt, PL_COLORBAR_ORIENT_LEFT, vx_min - cap_height, vx_min, vy_min, vy_max, high_cap_color );
        }
        else if ( opt & PL_COLORBAR_ORIENT_BOTTOM )
        {
          draw_cap( opt, PL_COLORBAR_ORIENT_BOTTOM, vx_min, vx_max, vy_min - cap_height, vy_min, high_cap_color );
        }
     }

    // Write label.
    if ( opt & PL_COLORBAR_LABEL_LEFT )
    {
        if ( opt & PL_COLORBAR_ORIENT_TOP || opt & PL_COLORBAR_ORIENT_BOTTOM )
        {
            if ( position & PL_POSITION_LEFT )
                label_offset += 4.0;
            else
                label_offset += 2.0;
            perp          = '\0';
            just          = 0.5;
        }
        else
        {
            label_offset += 1.5;
            perp          = 'v';
            just          = 1.0;
        }
        snprintf( opt_string, max_opts, "l%c", perp );
        plmtex( opt_string, label_offset, 0.5, just, label );
    }
    else if ( opt & PL_COLORBAR_LABEL_RIGHT )
    {
        if ( opt & PL_COLORBAR_ORIENT_TOP || opt & PL_COLORBAR_ORIENT_BOTTOM )
        {
            if ( position & PL_POSITION_RIGHT )
                label_offset += 4.0;
            else
                label_offset += 2.0;
            perp          = '\0';
            just          = 0.5;
        }
        else
        {
            label_offset += 1.5;
            perp          = 'v';
            just          = 0.0;
        }
        snprintf( opt_string, max_opts, "r%c", perp );
        plmtex( opt_string, label_offset, 0.5, just, label );
    }
    else if ( opt & PL_COLORBAR_LABEL_TOP )
    {
        if ( opt & PL_COLORBAR_ORIENT_TOP || opt & PL_COLORBAR_ORIENT_BOTTOM )
        {
            label_offset += 1.5;
            perp          = '\0';
        }
        else
        {
            if ( position & PL_POSITION_TOP )
                label_offset += 4.0;
            else
                label_offset += 2.0;
            perp          = '\0';
        }
        snprintf( opt_string, max_opts, "t%c", perp );
        plmtex( opt_string, label_offset, 0.5, 0.5, label );
    }
    else if ( opt & PL_COLORBAR_LABEL_BOTTOM )
    {
        if ( opt & PL_COLORBAR_ORIENT_TOP || opt & PL_COLORBAR_ORIENT_BOTTOM )
        {
            label_offset += 1.5;
            perp          = '\0';
        }
        else
        {
            if ( position & PL_POSITION_BOTTOM )
                label_offset += 4.0;
            else
                label_offset += 2.0;
            perp          = '\0';
        }
        snprintf( opt_string, max_opts, "b%c", perp );
        plmtex( opt_string, label_offset, 0.5, 0.5, label );
    }

    if ( position & PL_POSITION_LEFT || position & PL_POSITION_BOTTOM )
    {
        tick_label_string = "n";
    }
    else if ( position & PL_POSITION_RIGHT || position & PL_POSITION_TOP )
    {
        tick_label_string = "m";
    }
 
    // Draw numerical labels and tick marks if this is a shade color bar
    // TODO: A better way to handle this would be to update the
    // internals of plbox to support custom tick and label positions
    // along an axis.

    if ( opt & PL_COLORBAR_SHADE && opt & PL_COLORBAR_SHADE_LABEL )
    {
        snprintf( opt_string, max_opts, "%s%s", tick_label_string, local_axis_opts );
        if ( opt & PL_COLORBAR_ORIENT_RIGHT || opt & PL_COLORBAR_ORIENT_LEFT)
            label_box_custom( opt_string, n_values, values, "", 0, NULL );
        else
            label_box_custom( "", 0, NULL, opt_string, n_values, values );
        // Exclude tick labels for plbox call below since those tick
        // labels have already been handled in a custom way above.
        tick_label_string = "";
        // Exclude ticks for plbox call below since those tick marks
        // have already been handled in a custom way above.
        remove_characters(local_axis_opts, "TtXx");
    }

    // Draw the outline for the entire colorbar, tick marks, tick labels.

    if ( opt & PL_COLORBAR_EDGE )
        edge_string = "bc";
    else
        edge_string = "uw";
    snprintf( opt_string, max_opts, "%s%s%s", edge_string, tick_label_string, local_axis_opts );
    if ( opt & PL_COLORBAR_ORIENT_TOP || opt & PL_COLORBAR_ORIENT_BOTTOM)
    {
          plbox( edge_string , 0.0, 0, opt_string, ticks, sub_ticks );
    }
    else 
    {
          plbox( opt_string, ticks, sub_ticks, edge_string, 0.0, 0 );
    }

    free(local_axis_opts);

    // Restore previous plot characteristics.
    plcol0( col0_save );
    plvpor( xdmin_save, xdmax_save, ydmin_save, ydmax_save );
    plwind( xwmin_save, xwmax_save, ywmin_save, ywmax_save );

    return;
}

#else
// Code version without reorganized API for plcolorbar
//--------------------------------------------------------------------------
//! Draw triangular end-caps for color bars.
//!
//! @param position This variable defines the placement of the colorbar on the
//! subpage.  The position can be one of PL_POSITION_TOP,
//! PL_POSITION_BOTTOM, PL_POSITION_LEFT or PL_POSITION_RIGHT.  The colorbar
//! will be drawn perpendicular to the given side of the subpage.
//! @param opt This variable can be PL_COLORBAR_CAP_LOW or
//! PL_COLORBAR_CAP_HIGH, indicating whether we are drawing a low-end cap or a
//! high-end cap.
//! @param a1 First primary coordinate for the end cap base.  If position
//! is PL_POSITION_LEFT or PL_POSITION_RIGHT then a1 and a2 are x coordinates.
//! if position is PL_POSITION_TOP or PL_POSITION_BOTTOM then a1 and a2 are y
//! coordinates.  (a1, b) and (a2, b) OR (b, a1) and (b, a2) define the base of
//! the triangular cap.
//! @param a2 Second primary coordinate for the end cap base.
//! @param b Secondary coordinate for the end cap base.  If a1 and a2 are x,
//! b is y.  If a1 and a2 are y, b is x.
//! @param color Color (color palette 1) used to fill the end cap.
//!

void
draw_cap( PLINT position, PLINT opt, PLFLT a1, PLFLT a2, PLFLT b, PLFLT color )
{
    // Height the cap in normalized coordinates
    PLFLT cap_height = 0.05;

    // Save drawing color
    PLINT col0_save = plsc->icol0;

    // Save window and viewport
    // Saved normalized coordinates of viewport.
    PLFLT xdmin_save, xdmax_save, ydmin_save, ydmax_save;
    // Saved world coordinates of viewport.
    PLFLT xwmin_save, xwmax_save, ywmin_save, ywmax_save;
    plgvpsp( &xdmin_save, &xdmax_save, &ydmin_save, &ydmax_save );
    plgvpw( &xwmin_save, &xwmax_save, &ywmin_save, &ywmax_save );

    // Use the entire sub-page, and make world coordinates 0.0 -> 1.0
    // This way the location and orientation of the cap can be easily
    // defined by a combination of position, opt, a1, a2 and b.
    plvpor( 0.0, 1.0, 0.0, 1.0 );
    plwind( 0.0, 1.0, 0.0, 1.0 );

    // Points for the triangle
    PLFLT xs[3];
    PLFLT ys[3];

    plcol1( color );

    if ( opt & PL_COLORBAR_CAP_LOW )
    {
        if ( position & PL_POSITION_LEFT || position & PL_POSITION_RIGHT )
        {
            // Draw the cap on the bottom
            xs[0] = a1;
            ys[0] = b;
            xs[2] = a2;
            ys[2] = b;
            xs[1] = ( xs[0] + xs[2] ) / 2.0;
            ys[1] = ys[0] - cap_height;

            plfill( 3, xs, ys );
        }
        else if ( position & PL_POSITION_TOP || position & PL_POSITION_BOTTOM )
        {
            // Draw the cap on the left
            xs[0] = b;
            ys[0] = a1;
            xs[2] = b;
            ys[2] = a2;
            xs[1] = xs[0] - cap_height;
            ys[1] = ( ys[0] + ys[2] ) / 2.0;

            plfill( 3, xs, ys );
        }
    }
    else if ( opt & PL_COLORBAR_CAP_HIGH )
    {
        if ( position & PL_POSITION_LEFT || position & PL_POSITION_RIGHT )
        {
            // Draw the cap on the top
            xs[0] = a1;
            ys[0] = b;
            xs[2] = a2;
            ys[2] = b;
            xs[1] = ( xs[0] + xs[2] ) / 2.0;
            ys[1] = ys[0] + cap_height;

            plfill( 3, xs, ys );
        }
        else if ( position & PL_POSITION_TOP || position & PL_POSITION_BOTTOM )
        {
            // Draw the cap on the right
            xs[0] = b;
            ys[0] = a1;
            xs[2] = b;
            ys[2] = a2;
            xs[1] = xs[0] + cap_height;
            ys[1] = ( ys[0] + ys[2] ) / 2.0;

            plfill( 3, xs, ys );
        }
    }

    // Restore the drawing color
    plcol0( col0_save );

    // Draw cap outline
    plline( 3, xs, ys );

    // Restore window and viewport
    plvpor( xdmin_save, xdmax_save, ydmin_save, ydmax_save );
    plwind( xwmin_save, xwmax_save, ywmin_save, ywmax_save );
}

//--------------------------------------------------------------------------
//! Plot color bar for image, shade or gradient plots.
//!
//! @param opt This variable contains bits which control the overall colorbar.
//! The type of colorbar can be specified with PL_COLORBAR_SHADE,
//! PL_COLORBAR_IMAGE or PL_COLORBAR_GRADIENT.  The position of the (optional)
//! label/title can be specified with
//! PL_COLORBAR_LABEL_(LEFT|RIGHT|TOP|BOTTOM).  If no label position is
//! specified then no label will be drawn.  End-caps for the colorbar can
//! added with PL_COLORBAR_CAP_LOW and PL_COLORBAR_CAP_HIGH.  If a particular
//! colorbar cap option is not specified then no cap will be drawn for that
//! end.  As a special case for PL_COLORBAR_SHADE, the option
//! PL_COLORBAR_SHADE_LABEL can be specified.  If this option is provided then
//! any tick marks and tick labels will be placed at the breaks between shaded
//! segments.  TODO: This should be expanded to support custom placement of
//! tick marks and tick labels at custom value locations for any colorbar type.
//! @param position This variable defines the placement of the colorbar on the
//! subpage.  The position can be one of PL_POSITION_TOP,
//! PL_POSITION_BOTTOM, PL_POSITION_LEFT or PL_POSITION_RIGHT.  The colorbar
//! will be drawn perpendicular to the given side of the subpage.
//! @param x Colorbar displacement distance along/away from the horizonal axis
//! in normalized subpage coordinates.
//! @param y Colorbar displacement distance along/away from the vertical axis
//! in normalized subpage coordinates.
//! @param length Length of the colorbar along its major axis (ex. along the
//! top of the subpage if pos is PL_POSITION_TOP) in normalized subpage
//! coordinates.
//! @param width Width of the colorbar along the minor axis (ex. fraction of
//! the vertical subpage size if pos is PL_POSITION_TOP) in normalized subpage
//! coordinates.
//! @param low_cap_color Color of the low-end color bar cap, if it is drawn.
//! @param high_cap_color Color of the high-end color bar cap, if it is drawn.
//! @param cont_color Contour color for PL_COLORBAR_SHADE plots.  This is
//! passed directly to plshades, so it will be interpreted according to the
//! design of plshades.
//! @param cont_width Contour width for PL_COLORBAR_SHADE plots.  This is
//! passed directly to plshades, so it will be interpreted according to the
//! design of plshades.
//! @param ticks Spacing of major ticks, as for plbox.
//! @param sub_ticks Number of subticks, as for plbox.
//! @param axis_opts Axis options for the colorbar's major axis, as for plbox.
//! @param label Text label for the colorbar.  No title is drawn if no label
//! position is specified in pos.
//! @param n_values Number of elements in the values array.
//! @param colors Colors (color map 1) used to draw the colorbar.  If this is a
//! PL_COLORBAR_SHADE bar then there should be one entry per break between
//! segments.  If this
//! is a PL_COLORBAR_IMAGE or PL_COLORBAR_GRADIENT bar then there should be two
//! elements - one to specify the high end and one to specify the low end.
//! This should have (n_values - 1) elements.
//! @param values Numeric values for the data range represented by the
//! colorbar.  For PL_COLORBAR_SHADE, this should include one value per break
//! between segments.  For PL_COLORBAR_IMAGE and PL_COLORBAR_GRADIENT this
//! include two values, one for the maximum value on the scale and one for the
//! minimum value.
//!

void
c_plcolorbar( PLINT opt, PLINT position,
              PLFLT x, PLFLT y, PLFLT length, PLFLT width,
              PLFLT low_cap_color, PLFLT high_cap_color,
              PLINT cont_color, PLINT cont_width,
              PLFLT ticks, PLINT sub_ticks,
              const char *axis_opts, const char *label,
              PLINT n_values, const PLFLT *values )
{
    // Justification of label text
    PLFLT just;

    // Min and max values
    // Assumes that the values array is sorted from smallest to largest
    // OR from largest to smallest.
    PLFLT min_value, max_value;
    min_value = values[0];
    max_value = values[ n_values - 1 ];

    // Min and max colors
    // Assumes that the colors array is sorted from smallest to largest.
    PLFLT min_color, max_color;
    plgcmap1_range( &min_color, &max_color );

    // Saved normalized coordinates of viewport.
    PLFLT xdmin_save, xdmax_save, ydmin_save, ydmax_save;
    // Saved world coordinates of viewport.
    PLFLT xwmin_save, xwmax_save, ywmin_save, ywmax_save;
    plgvpsp( &xdmin_save, &xdmax_save, &ydmin_save, &ydmax_save );
    plgvpw( &xwmin_save, &xwmax_save, &ywmin_save, &ywmax_save );

    // Active attributes to be saved and restored afterward.
    PLINT col0_save       = plsc->icol0;

    // Position of the color bar in normalized viewport (= normalized subpage
    // coordinates).
    PLFLT vx_min, vx_max, vy_min, vy_max;
    PLFLT wx_min, wx_max, wy_min, wy_max;
    // Build the proper viewport and window dimension along the requested side
    // of the subpage
    if ( position & PL_POSITION_LEFT )
    {
        vx_min = x;
        vy_min = y;
        vx_max = vx_min + width;
        vy_max = vy_min + length;
        wx_min = 0.0;
        wy_min = min_value;
        wx_max = 1.0;
        wy_max = max_value;
    }
    else if ( position & PL_POSITION_RIGHT )
    {
        vx_min = 1.0 - x - width;
        vy_min = y;
        vx_max = vx_min + width;
        vy_max = vy_min + length;
        wx_min = 0.0;
        wy_min = min_value;
        wx_max = 1.0;
        wy_max = max_value;
    }
    else if ( position & PL_POSITION_TOP )
    {
        vx_min = x;
        vy_min = 1.0 - y - width;
        vx_max = vx_min + length;
        vy_max = vy_min + width;
        wx_min = min_value;
        wy_min = 0.0;
        wx_max = max_value;
        wy_max = 1.0;
    }
    else if ( position & PL_POSITION_BOTTOM )
    {
        vx_min = x;
        vy_min = y;
        vx_max = vx_min + length;
        vy_max = vy_min + width;
        wx_min = min_value;
        wy_min = 0.0;
        wx_max = max_value;
        wy_max = 1.0;
    }
    else
    {
        plabort( "plcolorbar: Invalid or missing side" );
    }

    // The window used to draw the colorbar should take up the whole viewport
    plvpor( vx_min, vx_max, vy_min, vy_max );
    plwind( wx_min, wx_max, wy_min, wy_max );

    // The data to plot
    PLFLT **color_data;
    // Setting up the data for display
    PLINT i, j, ni, nj, n_steps;
    PLFLT step_size;

    // What kind of color bar are we making?
    if ( opt & PL_COLORBAR_IMAGE )
    {
        // Interpolate
        // TODO: Should this be decided with an extra opt option instead of by
        // counting n_values?
        if ( n_values == 2 )
        {
            // Use the same number of steps as there are steps in
            // color palette 1.
            // TODO: Determine a better way to specify the steps here?
            n_steps   = plsc->ncol1;
            step_size = ( max_value - min_value ) / (PLFLT) n_steps;
            if ( position & PL_POSITION_LEFT || position & PL_POSITION_RIGHT )
            {
                ni = 2;
                nj = n_steps;
                plAlloc2dGrid( &color_data, ni, nj );
                for ( i = 0; i < ni; i++ )
                {
                    for ( j = 0; j < nj; j++ )
                    {
                        color_data[i][j] = min_value + (PLFLT) j * step_size;
                    }
                }
            }
            else if ( position & PL_POSITION_TOP || position & PL_POSITION_BOTTOM )
            {
                ni = n_steps;
                nj = 2;
                plAlloc2dGrid( &color_data, ni, nj );
                for ( i = 0; i < ni; i++ )
                {
                    for ( j = 0; j < nj; j++ )
                    {
                        color_data[i][j] = min_value + (PLFLT) i * step_size;
                    }
                }
            }
            else
            {
                plabort( "plcolorbar: Invalid side" );
            }
        }
        // No interpolation - use values array as-is
        else
        {
            n_steps = n_values;
            // Use the provided values in this case.
            if ( position & PL_POSITION_LEFT || position & PL_POSITION_RIGHT )
            {
                ni = 2;
                nj = n_steps;
                plAlloc2dGrid( &color_data, ni, nj );
                for ( i = 0; i < ni; i++ )
                {
                    for ( j = 0; j < nj; j++ )
                    {
                        color_data[i][j] = values[j];
                    }
                }
            }
            else if ( position & PL_POSITION_TOP || position & PL_POSITION_BOTTOM )
            {
                ni = n_steps;
                nj = 2;
                plAlloc2dGrid( &color_data, ni, nj );
                for ( i = 0; i < ni; i++ )
                {
                    for ( j = 0; j < nj; j++ )
                    {
                        color_data[i][j] = values[i];
                    }
                }
            }
            else
            {
                plabort( "plcolorbar: Invalid side" );
            }
        }
        // Draw the color bar
        plimage( (const PLFLT **) color_data, ni, nj, wx_min, wx_max, wy_min, wy_max,
            min_value, max_value, wx_min, wx_max, wy_min, wy_max );
        plFree2dGrid( color_data, ni, nj );
    }
    else if ( opt & PL_COLORBAR_SHADE )
    {
        // Transform grid
        // The transform grid is used to make the size of the shaded segments
        // scale relative to other segments.  For example, if segment A
        // makes up 10% of the scale and segment B makes up 20% of the scale
        // then segment B will be twice the length of segment A.
        PLcGrid grid;
        PLFLT   grid_axis[2] = { 0.0, 1.0 };
        n_steps = n_values;
        // Use the provided values.
        if ( position & PL_POSITION_LEFT || position & PL_POSITION_RIGHT )
        {
            grid.xg = grid_axis;
            grid.yg = (PLFLT *) values;
            grid.nx = 2;
            grid.ny = n_steps;
            ni      = 2;
            nj      = n_steps;
            plAlloc2dGrid( &color_data, ni, nj );
            for ( i = 0; i < ni; i++ )
            {
                for ( j = 0; j < nj; j++ )
                {
                    color_data[i][j] = values[j];
                }
            }
        }
        else if ( position & PL_POSITION_TOP || position & PL_POSITION_BOTTOM )
        {
            grid.xg = (PLFLT *) values;
            grid.yg = grid_axis;
            grid.nx = n_steps;
            grid.ny = 2;
            ni      = n_steps;
            nj      = 2;
            plAlloc2dGrid( &color_data, ni, nj );
            for ( i = 0; i < ni; i++ )
            {
                for ( j = 0; j < nj; j++ )
                {
                    color_data[i][j] = values[i];
                }
            }
        }
        else
        {
            plabort( "plcolorbar: Invalid side" );
        }

        // Draw the color bar
        plshades( (const PLFLT **) color_data, ni, nj, NULL, wx_min, wx_max, wy_min, wy_max,
            values, n_steps, 0, cont_color, cont_width, plfill, TRUE,
            pltr1, (void *) ( &grid ) );
        plFree2dGrid( color_data, ni, nj );
    }
    else if ( opt & PL_COLORBAR_GRADIENT )
    {
        PLFLT xs[4], ys[4];
        xs[0] = wx_min;
        ys[0] = wy_min;
        xs[1] = wx_max;
        ys[1] = wy_min;
        xs[2] = wx_max;
        ys[2] = wy_max;
        xs[3] = wx_min;
        ys[3] = wy_max;
        PLFLT angle;
        // Make sure the gradient runs in the proper direction
        if ( position & PL_POSITION_LEFT || position & PL_POSITION_RIGHT )
        {
            // Top to bottom
            angle = 90.0;
        }
        else if ( position & PL_POSITION_TOP || position & PL_POSITION_BOTTOM )
        {
            // Left to right
            angle = 0.0;
        }
        else
        {
            plabort( "plcolorbar: Invalid side" );
        }
        plgradient( 4, xs, ys, angle );
    }

    // Restore the previous drawing color to use for outlines and text
    plcol0( col0_save );

    // How far away from the axis should the label be drawn?
    PLFLT label_offset;
    label_offset = 0.0;

    // Draw end-caps
    if ( opt & PL_COLORBAR_CAP_LOW )
    {
        // Add an extra offset for the label so it does not bump in to the
        // cap if the label is placed on the same side as the cap.
        if ( ( ( position & PL_POSITION_LEFT || position & PL_POSITION_RIGHT ) &&
               opt & PL_COLORBAR_LABEL_BOTTOM ) ||
             ( ( position & PL_POSITION_TOP || position & PL_POSITION_BOTTOM ) &&
               opt & PL_COLORBAR_LABEL_LEFT ) )
        {
            label_offset += 2.5;
        }
        // Draw a filled triangle (cap/arrow) at the low end of the scale
        if ( position & PL_POSITION_LEFT || position & PL_POSITION_RIGHT )
        {
            draw_cap( position, PL_COLORBAR_CAP_LOW, vx_min, vx_max, vy_min, low_cap_color );
        }
        else if ( position & PL_POSITION_BOTTOM || position & PL_POSITION_TOP )
        {
            draw_cap( position, PL_COLORBAR_CAP_LOW, vy_min, vy_max, vx_min, low_cap_color );
        }
    }
    if ( opt & PL_COLORBAR_CAP_HIGH )
    {
        // Add an extra offset for the label so it does not bump in to the
        // cap if the label is placed on the same side as the cap.
        if ( ( ( position & PL_POSITION_LEFT || position & PL_POSITION_RIGHT ) &&
               opt & PL_COLORBAR_LABEL_TOP ) ||
             ( ( position & PL_POSITION_TOP || position & PL_POSITION_BOTTOM ) &&
               opt & PL_COLORBAR_LABEL_RIGHT ) )
        {
            label_offset += 2.5;
        }
        // Draw a filled triangle (cap/arrow) at the high end of the scale
        if ( position & PL_POSITION_LEFT || position & PL_POSITION_RIGHT )
        {
            draw_cap( position, PL_COLORBAR_CAP_HIGH, vx_min, vx_max, vy_max, high_cap_color );
        }
        else if ( position & PL_POSITION_BOTTOM || position & PL_POSITION_TOP )
        {
            draw_cap( position, PL_COLORBAR_CAP_HIGH, vy_min, vy_max, vx_max, high_cap_color );
        }
    }

    // For building axis option string
    PLINT      max_opts = 25;
    char       opt_string[max_opts];
    const char *tick_string;

    tick_string = "";

    // Draw a title
    char perp;
    if ( opt & PL_COLORBAR_LABEL_LEFT )
    {
        if ( position & PL_POSITION_RIGHT || position & PL_POSITION_LEFT )
        {
            if ( position & PL_POSITION_LEFT )
                label_offset += 4.0;
            else
                label_offset += 2.0;
            perp          = '\0';
            just          = 0.5;
        }
        else
        {
            label_offset += 1.5;
            perp          = 'v';
            just          = 1.0;
        }
        snprintf( opt_string, max_opts, "l%c", perp );
        plmtex( opt_string, label_offset, 0.5, just, label );
    }
    else if ( opt & PL_COLORBAR_LABEL_RIGHT )
    {
        if ( position & PL_POSITION_RIGHT || position & PL_POSITION_LEFT )
        {
            if ( position & PL_POSITION_RIGHT )
                label_offset += 4.0;
            else
                label_offset += 2.0;
            perp          = '\0';
            just          = 0.5;
        }
        else
        {
            label_offset += 1.5;
            perp          = 'v';
            just          = 0.0;
        }
        snprintf( opt_string, max_opts, "r%c", perp );
        plmtex( opt_string, label_offset, 0.5, just, label );
    }
    else if ( opt & PL_COLORBAR_LABEL_TOP )
    {
        if ( position & PL_POSITION_RIGHT || position & PL_POSITION_LEFT )
        {
            label_offset += 1.5;
            perp          = '\0';
        }
        else
        {
            if ( position & PL_POSITION_TOP )
                label_offset += 4.0;
            else
                label_offset += 2.0;
            perp          = '\0';
        }
        snprintf( opt_string, max_opts, "t%c", perp );
        plmtex( opt_string, label_offset, 0.5, 0.5, label );
    }
    else if ( opt & PL_COLORBAR_LABEL_BOTTOM )
    {
        if ( position & PL_POSITION_RIGHT || position & PL_POSITION_LEFT )
        {
            label_offset += 1.5;
            perp          = '\0';
        }
        else
        {
            if ( position & PL_POSITION_BOTTOM )
                label_offset += 4.0;
            else
                label_offset += 2.0;
            perp          = '\0';
        }
        snprintf( opt_string, max_opts, "b%c", perp );
        plmtex( opt_string, label_offset, 0.5, 0.5, label );
    }

    // Draw labels and tick marks if this is a shade color bar
    // TODO: A better way to handle this would be to update the
    // internals of plbox to support custom tick and label positions
    // along an axis.
    if ( opt & PL_COLORBAR_SHADE && opt & PL_COLORBAR_SHADE_LABEL )
    {
        if ( position & PL_POSITION_LEFT )
        {
            snprintf( opt_string, max_opts, "nt%s", axis_opts );
            label_box_custom( "", 0, NULL, opt_string, n_values, values );
        }
        else if ( position & PL_POSITION_RIGHT )
        {
            snprintf( opt_string, max_opts, "mt%s", axis_opts );
            label_box_custom( "", 0, NULL, opt_string, n_values, values );
        }
        else if ( position & PL_POSITION_TOP )
        {
            snprintf( opt_string, max_opts, "mt%s", axis_opts );
            label_box_custom( opt_string, n_values, values, "", 0, NULL );
        }
        else if ( position & PL_POSITION_BOTTOM )
        {
            snprintf( opt_string, max_opts, "nt%s", axis_opts );
            label_box_custom( opt_string, n_values, values, "", 0, NULL );
        }
    }
    else
    {
        if ( position & PL_POSITION_LEFT || position & PL_POSITION_BOTTOM )
        {
            tick_string = "n";
        }
        else if ( position & PL_POSITION_RIGHT || position & PL_POSITION_TOP )
        {
            tick_string = "m";
        }
    }

    // Draw the outline for the entire colorbar, tick marks, tick labels.
    if ( position & PL_POSITION_LEFT )
    {
        snprintf( opt_string, max_opts, "bc%s%s", tick_string, axis_opts );
        plbox( "bc", ticks, sub_ticks, opt_string, ticks, sub_ticks );
    }
    else if ( position & PL_POSITION_RIGHT )
    {
        snprintf( opt_string, max_opts, "bc%s%s", tick_string, axis_opts );
        plbox( "bc", 0.0, 0, opt_string, ticks, sub_ticks );
    }
    else if ( position & PL_POSITION_TOP )
    {
        snprintf( opt_string, max_opts, "bc%s%s", tick_string, axis_opts );
        plbox( opt_string, ticks, sub_ticks, "bc", 0.0, 0 );
    }
    else if ( position & PL_POSITION_BOTTOM )
    {
        snprintf( opt_string, max_opts, "bc%s%s", tick_string, axis_opts );
        plbox( opt_string, ticks, sub_ticks, "bc", 0.0, 0 );
    }

    // Restore previous plot characteristics.
    plcol0( col0_save );
    plvpor( xdmin_save, xdmax_save, ydmin_save, ydmax_save );
    plwind( xwmin_save, xwmax_save, ywmin_save, ywmax_save );

    return;
}

#endif
