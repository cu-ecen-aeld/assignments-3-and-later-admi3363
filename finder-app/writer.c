#include<stdio.h>
#include<stdlib.h>
#include<syslog.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        syslog(LOG_ERR, "Not enough input arguments");
        //printf("Writer - Not enough input arguments");
        return 1;
    }

    FILE *pFile;

    //char fileName[100];
    //fileName="adamtest.txt";//argv[0];

    pFile=fopen(argv[1], "w");

    if(pFile==NULL)
    {
        syslog(LOG_ERR, "Problem creating file");
        //printf("Writer - Problem creating file");
        return 1;
    }

    fprintf(pFile, "%s", argv[2]);
    //printf("Writer - file string %s", argv[2]);

    fclose(pFile);

    return 0;
}