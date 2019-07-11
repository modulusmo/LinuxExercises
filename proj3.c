#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
This program copies the unix wc command
  i.e. it counts lines, words, and chars in a file.
*/

int main(unsigned argc, char* argv[])
{

    FILE *fPtr;
    char *curArg;//current argument
    char *file_names [15];
    unsigned file_count=0;

    unsigned i,j;
    unsigned ccount=0,lcount=0,wcount=0;
    unsigned ctotal=0,ltotal=0,wtotal=0;

    char c;//letter by letter

    short cflag=0;//characters
    short lflag=0;//lines
    short wflag=0;//words
    short allflag=1;
    short newword=0;

    for (i=1; i<argc; ++i)/**Identify files and flags**/
    {
        curArg = argv[i];
        if (*curArg == '-')//is a flag
        {
            for (j=1; j < strlen(curArg); ++j)
            switch (curArg[j])
            {
                case 'c':
                case 'C':
                {
                    cflag = 1;
                    allflag = 0;
                    break;
                }
                case 'l':
                case 'L':
                {
                    lflag = 1;
                    allflag = 0;
                    break;
                }
                case 'w':
                case 'W':
                {
                    wflag = 1;
                    allflag = 0;
                    break;
                }
                default:
                {
                    fprintf(stderr,"%s: invalid option -- '%c'\n",argv[0],curArg[j]);
                    exit(1);
                }
            }
        }
        else
        {
            file_count++;
        }
    }/*end reading arguments*/

    if (allflag==1)/** if no flags found, then counts all**/
    {
        lflag=1;
        wflag=1;
        cflag=1;
    }
    /*Open files one at a time using one filePtr*/
    for (i=1; i<argc; ++i)
    {
        curArg = argv[i];
        if (*curArg != '-')//not a flag
        {
            /*count each file*/

            fPtr = fopen(argv[i],"r");
            if (fPtr==NULL){
                fprintf(stderr,"%s: %s: No such file or directory\n", argv[0], argv[i]);
                fclose(fPtr);
                continue;//go to next argument
            }else{
                while((c = fgetc(fPtr)) != EOF)
                {
                    ++ccount;
                    if (c=='\n')
                        ++lcount;
                    if (c=='\t'||c=='\n'||c==' ')
                        newword=0;
                    if ((c!='\t'&&c!='\n'&&c!=' ' )&&newword==0){
                        ++wcount;
                        newword=1;
                    }
                }

                if(lflag){
                    printf("\t%d",lcount);
                    ltotal+=lcount;
                    lcount=0;
                }
                if(wflag){
                    printf("\t%d",wcount);
                    wtotal+=wcount;
                    wcount=0;
                }
                if(cflag){
                    printf("\t%d",ccount);
                    ctotal+=ccount;
                    ccount=0;
                }
                printf("\t%s\n",argv[i]);

                fclose(fPtr);
            }
        }
    }
    if(file_count>1){/**output totals**/
        if(lflag)
            printf("\t%d",ltotal);
        if(wflag)
            printf("\t%d",wtotal);
        if(cflag)
            printf("\t%d",ctotal);
        printf("\ttotal\n");
    }
    if (file_count==0){/**if no files, evaluate user input**/
        while((c = getchar()) != EOF)
            {
                ++ccount;
                if (c=='\n')
                    ++lcount;
                if (c=='\t'||c=='\n'||c==' ')
                    newword=0;
                if ((c!='\t'&&c!='\n'&&c!=' ' )&&newword==0)
                {
                    ++wcount;
                    newword=1;
                }
            }

            if(lflag)
                printf("\t%d",lcount);
            if(wflag)
                printf("\t%d",wcount);
            if(cflag)
                printf("\t%d",ccount);
            printf("\n");
    }
    return 0;
}

