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

echo -e "Benchmarks\tInputs\tOutputs\tWires\tNode Count\tRuntime (s)"

test_cases_file="./benchmarks/test_cases_random_vtree"
program="./src/sfdd"
cat $test_cases_file | while read verilog_and_vtree_
do
    timeout 1800 "$program $verilog_and_vtree_"
done
exit 0
