#!/usr/bin/env bash

test_script=./run.sh
test_root=$(pwd)

echo ""
echo "***********************"
echo "* ioPlacer unit tests *"
echo "***********************"
echo ""

if [ "$#" -ne 1 ]; then
    echo "Usage: ./run_all.sh <path_to_bin>"
	exit 1
fi

binary=$(readlink -f $1)
failed=0

echo " > ioPlacer binary: $binary"
if [ ! -e $binary ] ; 
then
	echo "    - Binary not found. Exiting...\n" 
	exit 1
fi

for unit_test_path in src/* ; 
do
	test_name=$(basename $unit_test_path)
	echo " > Now running test $test_name..."

	if [ ! -e $unit_test_path/$test_script ] ; 
	then
		echo "    - Script \"run.sh\" not found. Skipping..." 
		continue
	fi
	
	cd $unit_test_path 
	$test_script $binary
	test_return_code=$?
	cd $test_root

	if [ $test_return_code == 0 ];
	then
		echo "     - Test returned GREEN (passed)"
	elif [ $test_return_code == 1 ];
	then
		echo "     - Test return YELLOW (passed)"
	else
		echo "     - Test returned RED (failed)"
		failed=1
	fi
done

echo ""
echo "ioPlacer tests finished!"

if [ $failed -gt 0 ]
then
	exit 1
fi
