#!/bin/bash

#Accepts the following arguments: the first argument is a full path to a file (including filename) on the filesystem, referred to below as writefile; 
#the second argument is a text string which will be written within this file, referred to below as writestr
writefile=$1;
writestr=$2;

#Exits with return value 1 error and print statements if any of the parameters above were not specified
if [ $# -lt 2 ] 
then
    echo "Incorrect number of input args"
    exit 1
fi

#Creates a new file with name and path writefile with content writestr, overwriting any existing file and creating the path if it doesn’t exist. 
#Exits with value 1 and error print statement if the file could not be created.
directory=$(dirname ${writefile})
filename=$(basename ${writefile})

#make the directory
mkdir -p ${directory}

cd ${directory}

if [ -d "${directory}" ]
then 
    echo ${writestr}>${filename}
else
    echo "$writefile was not created"
	exit 1
fi