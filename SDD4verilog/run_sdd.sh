#!/bin/bash

################################################################################
# Executes command with a timeout
# Params:
#   $1 timeout in seconds
#   $2 command
# Returns 1 if timed out 0 otherwise
timeout() {

    time=$1

    # start the command in a subshell to avoid problem with pipes
    # (spawn accepts one command)
    command="/bin/sh -c \"$2\""

    expect -c "set echo \"-noecho\"; set timeout $time; spawn -noecho $command; expect timeout { exit 1 } eof { exit 0 }"    

    if [ $? = 1 ] ; then
        echo "Timeout after ${time} seconds"
    fi

}

echo "============================================================" >> mcnc-output-table.txt
date >> mcnc-output-table.txt
echo "run sdd ----------------------------------------------------" >> mcnc-output-table.txt
echo -e "Benchmarks\tInputs\tOutputs\tWires\tNode Count\tRuntime (s)" >> mcnc-output-table.txt

test_cases_file="./benchmarks/test_cases"
program="./test1"
cat $test_cases_file | while read oneline
do
    timeout 1800 "$program $oneline"
done
echo "============================================================" >> mcnc-output-table.txt
exit 0
