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

test_cases_file="../test/test_cases"
program="./start"
cat $test_cases_file | while read oneline
do
    timeout 1800 "$program $oneline"
done

# killall Preview
for filename in ../dotG/*.dot; do
    dot -Tpng ../dotG/"$filename" -o ../dotG/"$filename".png
    open ../dotG/"$filename".png
done

exit 0
