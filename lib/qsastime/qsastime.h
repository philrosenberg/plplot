#ifndef _QSASTIME_H_
#define _QSASTIME_H_
/*
  This software originally contributed under the LGPL in January 2009 to
  PLplot by the
  Cluster Science Centre
  QSAS team,
  Imperial College, London
  Copyright (C) 2009 Imperial College, London

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

/* MJD measures from the start of 17 Nov 1858 */
	
/* These utilities use the Gregorian calendar after 4 Oct 1582 (Julian) i.e. from 15 Oct 1582 Gregorian
   Note C libraries use Gregorian only from 14 Sept 1752
   More detailed discussion can be found at http://aa.usno.navy.mil/data/docs/JulianDate.php
   These routines have been compared with the results of the US Naval Observatory online converter.
   Modified Julian Date (MJD) = Julian Date (JD) - 2400000.5
	
   In all routines, specifying a day, hour, minute or second field greater than would be valid is
   handled with modulo arithmetic and safe.
   Thus 2006-12-32 00:62:00.0 will safely, and correctly, be treated as 2007-01-01 01:02:00.0
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* include header file for dll definitions */
#include "qsastimedll.h"


typedef struct MJDtimeStruct
{
  /*
     MJD starts at 0h, so truncating MJD always gives the same day whatever the time (unlike JD).
     The MJD base day is arbitrary, i.e. seconds can be greater than one day or even negative.
  */
	
  int base_day; /* integer part of MJD used as default */
  double time_sec; /* seconds from start of base_day */
	
}MJDtime;


QSASTIMEDLLIMPEXP int setFromUT(int year, int month, int day, int hour, int min, double sec, MJDtime *MJD, int forceJulian);
QSASTIMEDLLIMPEXP void breakDownMJD(int *year, int *month, int *day, int *hour, int *min, double *sec, const MJDtime *MJD, int forceJulian);
QSASTIMEDLLIMPEXP size_t strfMJD(char * buf, size_t len, const char *format, const MJDtime *MJD, int forceJulian);

/* The following are externally accessible for now only because
   the test routine calls them directly */
QSASTIMEDLLIMPEXP const char * getDayOfWeek(const MJDtime *MJD);
QSASTIMEDLLIMPEXP const char * getLongDayOfWeek( const MJDtime *MJD);
QSASTIMEDLLIMPEXP const char * getMonth(int m);
QSASTIMEDLLIMPEXP const char * getLongMonth(int m);
QSASTIMEDLLIMPEXP int getDOY(const MJDtime *MJD, int forceJulian);

#endif