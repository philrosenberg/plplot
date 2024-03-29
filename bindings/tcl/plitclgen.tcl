#!/usr/local/bin/perl
# Arjen Markus
# - translation of the original Perl script
#
# This script is used to automatically generate the [incr Tcl] methods
# needed in the PLWin [incr Tcl] class.  These methods mirror most of
# the Tcl API commands which are generated by the companion script,
# pltclgen.
#
# However, PLWin is a widget wrapper, and as such, not all of the Tcl
# API has any business being in the widget.  So we remove various
# shadow methods for portions of the API which are not relevant to
# widgets, such as the stream manipulation functions, etc.
#
# Also, note that this script is not quite as automatic as pltclgen.
# The output must be inserted by hand into PLWin.tcl.  Hopefully this
# will not seem like an unreasonable limitation, since the Tcl API
# only changes occasionally by this point.
###############################################################################

#
# Exclude several functions from the class
#
set ignore {
    "plend"
    "plend1"
    "plfamadv"
    "plgfam"
    "plsfam"
    "plsfnam"
    "plsstrm"
}

#
# Use eval or {*} - depending on the Tcl version
#
set use_expand [package vsatisfies [package present Tcl] 8.5]

# main code --
#
set verbose [expr {[lsearch $argv "-v"] >= 0}]

# Find the source tree directory that must be specified on the command line.
set sourcedir [lindex $argv 0]                    ;# Get the current source directory - for "out of source tree builds"
set specfile  [file join $sourcedir "plapi.tpl"]  ;# PLplot API template specification file.
set genfile   "gen.itcl"                          ;# Generated functions go here.

set SPECFILE  [open $specfile]
set GENFILE   [open $genfile "w"]

# Scan the PLplot API template specification file looking for function
# "prototypes".  These are introduced with the token "pltclcmd".  When
# we find one, go process it.  Anything other than a comment or a
# valid function "prototype" is considered an error, and is printed to
# stdout.

while { [gets $SPECFILE line] >= 0 } {
    regsub {#.*$} $line {} line
    if { $line == "" } continue

    if { [regexp {^pltclcmd (\w+) (.*)} $line ==> cmd] } {
        if { [lsearch $ignore $cmd] < 0 } {
            puts "Generating itcl method for $cmd.\n";

            puts $GENFILE "    method $cmd {args} {";
            if { $use_expand } {
                puts $GENFILE "        \$plwin cmd $cmd {*}\$args";
            } else {
                puts $GENFILE "        eval \$plwin cmd $cmd \$args";
            }
            puts $GENFILE "    }\n";
        }
    } else {

        # Just print the unrecognized output to stdout.
        puts "? $line"
    }
}
