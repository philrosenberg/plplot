#!/usr/bin/perl

use Getopt::Std;

getopts('g:i:o:');

print "Converting to JPEG ... ";
system "mogrify", "-geometry", $opt_g, "-format", "jpeg", $opt_i;
$_ = $opt_i;
s/\.eps/\.jpeg/;
system "mv", ($_ . ".0"), $_;
system "rm", "-f", ($_ . ".1");
if ($_ ne $opt_o) {
  system "mv", $_, $opt_o;
}
print "done!\n";
