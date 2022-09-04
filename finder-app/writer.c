#include<stdio.h>
#include<stdlib.h>
#include<syslog.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        syslog(LOG_ERR, "Not enough input arguments");
        return 1;
    }

    FILE *pFile;

    char fileName[100];
    fileName="adamtest.txt";//argv[0];

    pFile=fopen(fileName, "w");

    if(pFile==NULL)
    {
        syslog(LOG_ERR, "Problem creating file");
        return 1;
    }

    fprintf(pFile, "%s", argv[1]);

    fclose(pFile);

    return 0;
}