#!/bin/sh

#Accepts the following runtime arguments: the first argument is a path to a directory on the filesystem, referred to below as filesdir; 
#the second argument is a text string which will be searched within these files, referred to below as searchstr
filesdir=$1;
searchstr=$2;

#Exits with return value 1 error and print statements if any of the parameters above were not specified
if [ $# -lt 2 ] 
then
    echo "Incorrect number of input params"
    exit 1
fi

#set default vars
numberOfFiles=0
numberOfMatchingLines=0

#Exits with return value 1 error and print statements if filesdir does not represent a directory on the filesystem
if [ -d "${filesdir}" ]
then
    #change to requested directory to be able to loop through files
    cd ${filesdir};

    #get the number of files
    numberOfFiles=$(find -L ${filesdir} -type f | wc -l)

    #loop through all the files in this directory and look for the search string(param 2)
    numberOfMatchingLines=$(grep -R ${searchstr} ${filesdir} | wc -l)
else
    echo "Directory [${filesdir}] does not represent a directory on the filesystem"
    exit 1
fi

#Prints a message "The number of files are X and the number of matching lines are Y" 
#where X is the number of files in the directory and all subdirectories 
#and Y is the number of matching lines found in respective files.
echo "The number of files are ${numberOfFiles} and the number of matching lines are ${numberOfMatchingLines}"