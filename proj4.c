/* Moses Cuevas
 * Project 4 File Access
 * This program displays the user's permissions
 * for each file shown on the command line.
 *
 * When a file is specified with a relative path name,
 * treat it as relative to the user's home directory.
 *
 * Use "ls -l" for proof of correctness.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

struct stat buf;
struct passwd *userPtr;

int main(int argc, char *argv[])
{
    int err=0,i,j,k=0;
    uid_t uid;
    uid = geteuid ();//get current user's id
    userPtr = getpwuid(uid);//get the user's passwd struct
    char filePath[100];//for other paths in system
    char dirFlag='f';//checks if it is directory
    unsigned modifier=01;
    char *currentArg;

    for (i=1; i<argc; ++i)
    {
        printf("\nChecking file: %s\n", argv[i]);
        currentArg=argv[i];
        //check if argv[i] is a file directory
        for(j=0; j<strlen(currentArg);++j)
        {
            if(currentArg[j]=='/')
            {
                dirFlag='t';
                strcpy(filePath,userPtr->pw_dir);
                strcat(filePath,"/");
                strcat(filePath,argv[i]);
                break;
            }
        }

        if(dirFlag=='f'){
            strcpy(filePath,argv[i]);
        }
        /** This part checks permissions of the file**/
        if(stat(filePath, &buf)==(-1))//returns -1 if error, else returns 0
        {
            fprintf(stderr, "%s: cannot access %s\n",argv[0],argv[i]);//writes to standard error
            err++;
            continue;//skip and go to next argument
        }

        /** shorter way of determining permissions **/
        if (userPtr->pw_uid==buf.st_uid)
            modifier *= 0100;
        else if (userPtr->pw_gid==buf.st_gid)
            modifier *= 010;

        if (modifier == 64)
            printf("Access as user\n");
        else if (modifier == 8)
            printf("Access as group\n");
        else
            printf("Access as other\n");

        if(buf.st_mode & (modifier * 04)) //(buf.st_mode & 0400), checks if user has read permission
            printf("read, ");
        else
            printf("no read, ");
        if(buf.st_mode & (modifier * 02))
            printf("write, ");
        else
            printf("no write, ");
        if(buf.st_mode & (modifier))
            printf("execute\n");
        else
            printf("no execute\n");

        dirFlag='f';//reset flag
        modifier=01;
    }
    printf("\n");
    if (err)
        return 1;
    else
        return 0;
}