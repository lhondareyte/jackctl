#!/bin/sh
CONFILES="bad_action.ini bad_section_name.ini"
count=0
rc=0

for f in $CONFILES
do
	count="$(( $count + 1 ))"
	../jackctl -f $f > /dev/null 2>&1
	rc="$(( $? + $rc ))"
done
if [ "$count" != "$rc" ] ; then
	echo "tests failed"
	rc=1
else
	echo "tests passed"
	rc=0
fi
exit $rc
