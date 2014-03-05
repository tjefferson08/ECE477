#!/bin/bash
if [ "${PWD##*/}" = "scripts" ]
then
    #scp -r ../src/website/* 477grp2@shay.ecn.purdue.edu:Website
    rsync -vau ../src/website/* 477grp2@shay.ecn.purdue.edu:~/Website
else
    echo "cd to the scripts directory before invoking this script"
fi