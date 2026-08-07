#!/usr/bin/env perl
use strict;
use warnings;

my %messages = (
	"test_1.ini" => 'Error: Missing source in "Missing source keyword" section',
	"test_2.ini" => 'Error: Missing destination in "Missing destination keyword" section',
	"test_3.ini" => "Error: test_3.ini[4]: Unknown action \"src\"\nError: Missing source in \"Wrong keyword\" section",
	"test_4.ini" => 'Error: test_4.ini[4]: Invalid section name',
	"test_5.ini" => 'Error: cannot connect foo to bar',
	"test_6.ini" => 'Error: Missing source in "Missing source and destination" section',
	"test_7.ini" => 'Error: Missing destination in "Missing destination" section'
);

foreach my $file (glob("*.ini")) {
	if ( not defined $messages{$file} ) {
		next;
	}
	my $msg = `../jackctl -f $file 2>&1`;
	chomp $msg;
	if ( $msg ne $messages{$file} ) {
		die "Regression with $file configuration file\n";
	}
}
print STDOUT "All tests passed.\n"
