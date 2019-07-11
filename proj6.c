/* Moses Cuevas
 * Project 6 Forks and Pipes
 * Copies the unix wc command using a separate child process
 * for every file listed on the command line.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>//defines read() and write()

int fork(void);
int pipe(int pd[2]);//pd are file descriptors; [0] for reading, [1] for writing
void readall(int[],int*,int*,int*);
void writeall(int[],int*,int*,int*);
void error_exit(char *s,char *t);
pid_t getpid(void);

int main(unsigned argc, char* argv[])
{
    FILE *fPtr;
    char *curArg;//current argument
    int pd[2];//pipe descriptors

    unsigned i,j;
    unsigned ccount=0,lcount=0,wcount=0;
    unsigned ctotal=0,ltotal=0,wtotal=0;
    unsigned file_count=0;

    short cflag=0,lflag=0,wflag=0;
    short allflag=1,newword=0;
    char c;//for "no files" case

    for (i=1; i<argc; ++i)/**Identify files and flags**/
    {
        curArg = argv[i];
        if (*curArg == '-'){//is a flag
            for (j=1; j < strlen(curArg); ++j)
            switch (curArg[j]){
                case 'c':
                case 'C': {
                    cflag = 1;
                    allflag = 0;
                    break;
                }
                case 'l':
                case 'L': {
                    lflag = 1;
                    allflag = 0;
                    break;
                }
                case 'w':
                case 'W': {
                    wflag = 1;
                    allflag = 0;
                    break;
                }
                default: {
                    fprintf(stderr,"%s: invalid option -- '%c'\n",argv[0],curArg[j]);
                    exit(1);
                }
            }
        }
        else{
            file_count++;
        }
    }/*end reading arguments*/
    if (allflag==1){/** if no flags found, then counts all**/
        lflag=1;
        wflag=1;
        cflag=1;
    }
    /** Create Pipe **/
    if (pipe(pd)==-1){
        error_exit(argv[0],"pipe() failed\n");
    }
    /*Open files one at a time using one filePtr*/
    for (i=1; i<argc; ++i){
        curArg = argv[i];
        if (*curArg != '-')//not a flag
        {
            if(fork()==0){/** Create child process **/
                fPtr = fopen(argv[i],"r");
                if (fPtr==NULL){
                    fprintf(stderr,"%s: %s: No such file or directory\n", argv[0], argv[i]);
                    fclose(fPtr);
                    continue;//go to next argument
                }
                else{
                    while((c = fgetc(fPtr)) != EOF){
                        ++ccount;
                        if (c=='\n')
                            ++lcount;
                        if (c=='\t'||c=='\n'||c==' ')
                            newword=0;
                        if ((c!='\t'&&c!='\n'&&c!=' ')&&newword==0){
                            ++wcount;
                            newword=1;
                        }
                    }
                    fclose(fPtr);//finished with pointer
                    if(lflag)
                        printf("\t%d",lcount);
                    if(wflag)
                        printf("\t%d",wcount);
                    if(cflag)
                        printf("\t%d",ccount);

                    printf("\t%s\t%ld\n",argv[i],(long)getpid());
                    writeall(pd,&lcount,&wcount,&ccount);
                }
                return;
            }/** End child process **/
        }
    }
    for (i=1; i<argc; ++i){/** combine totals from child processes **/
        curArg = argv[i];
        if (*curArg != '-'){//if not a flag
            readall(pd,&lcount,&wcount,&ccount);
            if(lflag)
                ltotal+=lcount;
            if(wflag)
                wtotal+=wcount;
            if(cflag)
                ctotal+=ccount;
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
        if(fork()==0){
            while((c = getchar()) != EOF){
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
            writeall(pd,&lcount,&wcount,&ccount);
            printf("\nChild Process ID:\t%ld\n",(long)getpid());
            return;
        }
        /**return from child**/
        readall(pd,&lcount,&wcount,&ccount);
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

void error_exit(char *s,char *t)
{
    fprintf(stderr,"%s: %s\n",s,t);
    exit(1);
}
void writeall(int pd[],int*lcount,int*wcount,int*ccount)
{
    if(write(pd[1],lcount, sizeof(unsigned))==-1)
        error_exit("wc07","l total write() failed");
    if(write(pd[1],wcount, sizeof(unsigned))==-1)
        error_exit("wc07","w total write() failed");
    if(write(pd[1],ccount, sizeof(unsigned))==-1)
        error_exit("wc07","c total write() failed");
}
void readall(int pd[],int*lcount,int*wcount,int*ccount)
{
    if(read(pd[0],lcount, sizeof(unsigned))==-1)
        error_exit("wc07","l total write() failed");
    if(read(pd[0],wcount, sizeof(unsigned))==-1)
        error_exit("wc07","w total write() failed");
    if(read(pd[0],ccount, sizeof(unsigned))==-1)
        error_exit("wc07","c total write() failed");
}

