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

    expect -c "set echo \"-noecho\";
        set timeout $time; spawn -noecho $command;
        expect timeout { exit 1 } eof { exit 0 }"    

    if [ $? = 1 ] ; then
        echo "Timeout after ${time} seconds"
    fi

}

test_cases_file="test/test_cases"
test0="./test0"
test1="./test1"
test2="./test3"
test3="./test3"
test4="./test4"
prog="./sfdd"

# cat $test_cases_file | while read oneline
# do
#     $prog test/$oneline
#     # timeout 1800 "$prog test/$oneline"
# done

# The following loops open all pictures in dot
# killall Preview
for foldername in dotG/test0; do
    for filename in $foldername/*.dot; do
        # echo haha
        dot -Tpng $filename -o $filename.png
        # open $filename.png
    done
done

exit 0
