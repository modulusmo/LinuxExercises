/* Moses Cuevas
 * Project 5 List Directory
 * This program lists all directories in the subtree
 * under a given directory on the command line.
 *
 * If no directory is on the command line, then
 * assume current directory
 */
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>//may require <dirent.h>
#include <string.h>

void listcontent(DIR*, struct direct*, char*);

int main(int argc, char *argv[])
{
    DIR *dp;
    struct direct *dir;
    char *startdir;

    if(argc == 1)//if no arguments
        startdir=".";//assumes current directory
    else if(argc == 2)
        startdir=argv[1];
    else{
        fprintf(stderr,"%s: too many arguments",argv[0]);
        exit(1);
    }
    listcontent(dp, dir, startdir);

    return 0;
}

void listcontent(DIR *dp, struct direct *dir, char *dirName)
{
    struct stat buf;
    char tempDir[100];
    char root[100];
    strcpy(root,dirName);//to discover sub directories
    strcpy(tempDir,dirName);
    DIR *newdp;
    struct direct *newdir;

    if((dp=opendir(dirName)) == NULL){//directory may require permissions
        fprintf(stderr,"cannot open directory.\n");
        exit(1);
    }
//print contents of current directory
    while((dir=readdir(dp)) != NULL){
        if(dir->d_ino == 0)//removed files have 0
            continue;//skip removed files
        strcat(tempDir,"/");
        strcat(tempDir,dir->d_name);//search new directory
        if(stat(tempDir, &buf) >=0){//negative values are error
            if((S_ISDIR(buf.st_mode)) && strcmp(dir->d_name,".")!=0
               && strcmp(dir->d_name,"..")!=0){
                printf("%s\n",dir->d_name);
            }
        }
        strcpy(tempDir,root);//reset to root
    }

    rewinddir(dp);
//checks subdirectories recursively
    while((dir=readdir(dp)) != NULL){
        if(dir->d_ino == 0)
            continue;
        strcat(tempDir,"/");
        strcat(tempDir,dir->d_name);

        if(stat(tempDir, &buf) >=0){
            if((S_ISDIR(buf.st_mode)) && strcmp(dir->d_name,".")!=0
               && strcmp(dir->d_name,"..")!=0){
                listcontent(newdp,newdir,tempDir);//check current dir
            }
        }
        strcpy(tempDir,root);
    }
    closedir(dp);
}

