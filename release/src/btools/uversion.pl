#!/usr/bin/perl
#
#	uversion.pl
#	Copyright (C) 2006 Jonathan Zarate
#
#	- update the build number for Tomato
#	!!TB - Added version suffix
#
#	some additional changes - pedro
#

use strict;
use warnings;
use POSIX qw(strftime);


sub error
{
	my $txt = shift;
	print "\nuversion.pl error: $txt\n";
	exit(1);
}

sub help
{
	print "Usage: uversion.pl --gen\n";
	exit(1);
}


if ($#ARGV < 0) {
	help();
}

if ($ARGV[0] ne "--gen") {
	help();
}


my $time = strftime("%a, %d %b %Y %H:%M:%S %z", localtime());
my $path = "router/shared";
my $major = 0;
my $minor = 0;
my $build = 0;
my $i = 0;
my $buildc = "";
my $space = "";
my $suffix = "";


local $/;
open(F, "$path/tomato_version.h") || error("opening tomato_version.h: $!");
$_ = <F>;
($major) = /^#define TOMATO_MAJOR\t+"(\d+)"/gm;
($minor) = /^#define TOMATO_MINOR\t+"(\d+)"/gm;
($build) = /^#define TOMATO_BUILD\t+"(\d+)"/gm;
if (!$major or !$minor or !$build) {
	error("Invalid version: '$_'");
}
close(F);


# read the build number from the command line
if ($ARGV[1] ne "--def") {
	$build = sprintf("%03d", $ARGV[1]);
	$buildc = sprintf(".%03d", $ARGV[1]);
}


# read the version suffix from the command line
my $start = 2;
my $stop = $#ARGV;
$suffix = "";
for ($i=$start; $i <= $stop; $i++) {
	if ($suffix eq "") {
		$suffix = $ARGV[$i];
	}
	elsif ($ARGV[$i] ne "") {
		$suffix = sprintf("%s %s", $suffix, $ARGV[$i]);
	}
}


open(F, ">$path/tomato_version.h~") || error("creating temp file: $!");
print F <<"END";
#ifndef __TOMATO_VERSION_H__
#define __TOMATO_VERSION_H__
#define TOMATO_MAJOR		"$major"
#define TOMATO_MINOR		"$minor"
#define TOMATO_BUILD		"$build"
#define	TOMATO_BUILDTIME	"$time"
#define TOMATO_VERSION		"$major.$minor$buildc $suffix"
#define TOMATO_SHORTVER		"$major.$minor"
#endif
END
close(F);
rename("$path/tomato_version.h~", "$path/tomato_version.h") || error("renaming: $!");


open(F, ">$path/tomato_version.~") || error("creating temp file: $!");
# add build number only if entered manually
if ($buildc eq "") {
	printf F "%d.%d %s", $major, $minor, $suffix;
} else {
	printf F "%d.%d.%03d %s", $major, $minor, $build, $suffix;
}
close(F);
rename("$path/tomato_version.~", "$path/tomato_version") || error("renaming: $!");


print "Version: $major.$minor$buildc $suffix ($time)\n";
exit(0);
