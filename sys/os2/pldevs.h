/* -*-C-*-
 *
 *  plDevs.h
 *
 *  Maurice LeBrun
 *  IFS, University of Texas at Austin
 *  18-Jul-1994
 *
 *  Contains macro definitions that determine what device drivers are
 *  compiled into the PLplot library.  On a Unix system, typically the
 *  configure script builds plDevs.h from plDevs.h.in.  Elsewhere, it's
 *  best to hand-configure a plDevs.h file and keep it with the
 *  system-specific files.
*/

#ifndef __PLDEVS_H__
#define __PLDEVS_H__

#undef PLD_emxvga
#undef PLD_plmeta		
#undef PLD_null			
#undef PLD_xterm		
#undef PLD_tek4010		
#undef PLD_tek4107		
#undef PLD_mskermit		
#undef PLD_vlt			
#undef PLD_versaterm		
#undef PLD_dg300		
#undef PLD_ps
#undef PLD_xfig			
#undef PLD_ljii			
#undef PLD_lj_hpgl		
#undef PLD_hp7470		
#undef PLD_hp7580		
#undef PLD_imp			
#undef PLD_xwin			
#undef PLD_tk
#undef PLD_dp
#undef PLD_os2pm
#undef PLD_impress

#define PLD_ljii
#define PLD_ljiiip
#define PLD_hpgl
#define PLD_dg300
#define PLD_ps
#define PLD_xfig
#define PLD_os2pm
#define PLD_plmeta
#define PLD_null
#define PLD_impress

#endif	/* __PLDEVS_H__ */
