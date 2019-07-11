/*
Moses Cuevas
Comp 222 - Fall 2017
Meeting Time: 1100-1215
Programming Assignment 2: Cache Simulation
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct CacheLine {
    int tag;
    int* block;
} CacheLine;

void readInt(int*, char*, FILE*);
int isPwrOf2(int);

void getParameters(int*,int*,int*, FILE*, char*);
void createArrays(int, int, int**, CacheLine**);
void fillArrays(int, int, int*, CacheLine*);
void readCache(int , int , int , int*, CacheLine*,FILE*);
void writeCache(int , int , int , int*, CacheLine*,FILE*);

int main()
{
    int choice, i;
    int mainMemSize, cacheSize, blockSize;
    char isInt='t', error='f';
    FILE * filePtr;
    filePtr = fopen("prgassign_2_test_data.txt","r");

    printf("\n\n** Program Written by Moses Cuevas\n");
    printf("** Class Meeting Time: 11:00 - 12:15\n");
    printf("** Comp 222 - Fall 2017\n");

    if (filePtr == NULL)//check if file is missing
    {
        printf("\n** File \"prgassign_2_test_data.txt\" Not Found, Program Terminating\n");
    }
    else //file was found
    {
        printf("\n**\n**   Permitted Inputs: Main Memory to Cache Memory Mapping\n");
        printf("**   -----------------------------\n");
        printf("**   1 Enter Configuration Parameters\n");
        printf("**   2 Read from cache\n");
        printf("**   3 Write to cache\n");
        printf("**   4 Exit\n**\n\n");


        printf("\n** Starting to Read Data from the input file: prgassign_2_test_data.txt\n");
        CacheLine *cache = NULL;//pointer for cache
        int *mainMem = NULL;//pointer for main memory
        do{
            printf("\n_________________________________________________________________________\n");
            choice = 0;//in case of error when reading menu choice from file
            readInt(&choice, &isInt, filePtr);//select menu option

            switch (choice)
            {
            case 1:
                error='f';
                getParameters(&mainMemSize, &blockSize, &cacheSize, filePtr, &error);
                if (error=='f') {
                    createArrays(mainMemSize, cacheSize, &mainMem, &cache);
                    fillArrays(mainMemSize, cacheSize, mainMem, cache);
                }
                break;
            case 2:
                readCache(mainMemSize, blockSize, cacheSize, mainMem, cache, filePtr);
                break;
            case 3:
                writeCache(mainMemSize, blockSize, cacheSize, mainMem, cache, filePtr);
                break;
            case 4:
                break;
            default:
                if (isInt == 't')
                    printf("\n** Error - Invalid Menu Input.\n");
                break;
            }
        }while(choice != 4);
        for(i=0; i<cacheSize; i++){
            free(cache[i].block);
        }
        free(cache);
        free(mainMem);
        fclose(filePtr);
        printf("\nMemory Freed Up and Program Terminated Normally\n\n");
    }
    return 0;
}
void readInt(int *value, char *isInt, FILE *filePtr)//ensures all inputs are integers
{
    char bffr[15];
    char *ptr;//for strtol()
    char stop='f';
    *isInt='t';
    int i;

    if (feof(filePtr)){
        printf("\n** Warning! File has reached end. Exiting Program.\n");
        *value = 4;
    }
    else{
        fscanf(filePtr,"%s", bffr);//fscanf(filePtr, "%d", &bffr);
        bffr[14]='\0';

        for (i=0; i<15 && stop=='f'; i++){
            if (bffr[i]=='\0'){//stop when no more characters to read
                stop='t';
            }
            else if (!isdigit(bffr[i])){//check if each digit is a number
                printf("\n** Error - Non Integer Input.\n");
                *isInt = 'f';
                stop='t';
            }
        }
        if (*isInt=='t'){//assign value if integer
            *value=(int) strtol (bffr, &ptr, 10);//in base 10
        }
    }

}

int isPwrOf2(int value)
{
    while ((value%2 == 0)&&(value > 1))//loop stops when value is not divisible by two
        value/=2;
    return (value == 1);//if value is a power of two, value will be one after loop
}

void getParameters(int *mainMemSize, int *blockSize, int *cacheSize, FILE *filePtr, char *error)
{
    char isInt='t';
    readInt(mainMemSize, &isInt, filePtr);
    if (isPwrOf2(*mainMemSize)==0){
        printf("\n** Error - Main Memory Size is not a Power of 2!\n");
        printf("\n** Trying Again\n");
        *error='t';
    }

    readInt(cacheSize, &isInt, filePtr);
    if (isPwrOf2(*cacheSize)==0) {
        printf("\n** Error - Cache Size is not a Power of 2!");
        printf("\n\n** Trying Again\n");
        *error='t';
    }
    else if (*cacheSize > *mainMemSize) {
        printf("\n** Error - Cache Size is Larger than Main Memory Size!\n");
        printf("\n** Trying Again\n");
        *error='t';
    }

    readInt(blockSize, &isInt, filePtr);
    if (isPwrOf2(*blockSize)==0) {
        printf("\n** Error - Block Size is not a Power of 2!\n");
        printf("\n** Trying Again\n");
        *error='t';
    }
    else if (*blockSize > *cacheSize) {
        printf("\n** Error - Block Size is Larger than Cache Size!\n");
        printf("\n** Trying Again\n");
        *error='t';
    }

}
void createArrays(int mainMemSize, int cacheSize, int** mainMem, CacheLine** cache)
{
    free(*cache);
    *cache = (CacheLine*) malloc(cacheSize * sizeof(CacheLine));//create cache

    free(*mainMem);
    (*mainMem) = (int*) malloc(mainMemSize * sizeof(int));//create main memory
}

void fillArrays(int mainMemSize, int cacheSize, int* mainMem, CacheLine* cache)
{
    int i=0;
    for (i=0; i<cacheSize; i++) {
        cache[i].block = NULL;//cache is empty
        cache[i].tag = -1;//tag is "undefined"
    }

    for (i=0; i<mainMemSize; i++) {
        mainMem[i]= mainMemSize - i;//initialize memory with reverse order values
    }//delete previous cache line and load block with address
    printf("\n** All Input Parameters Accepted\n");
    printf("\n** Starting to Process Write/Read Requests\n");
}

void readCache(int mainMemSize,int blockSize,int cacheSize,int* mainMem,CacheLine* cache,FILE *filePtr)
{
    int i, r_w_addr, word, tempTag, tempLine, startAddr;
    char isInt;
    readInt(&r_w_addr, &isInt, filePtr);
    if (r_w_addr < mainMemSize){
        word = r_w_addr%blockSize;//word in line
        tempLine = (r_w_addr%cacheSize)/blockSize;//line in cache
        tempTag = r_w_addr/cacheSize;//tag to identify block

        if (tempTag == cache[tempLine].tag){
            printf("\n** Block in Cache\n");
        } else {
            if(cache[tempLine].block != NULL)
                printf("\n** Collision - First Replace Line of Cache from Memory!\n");
            else
                printf("\n** Read Miss... Load Block from Memory!\n");
            //delete previous cache line and load block with address
            free(cache[tempLine].block);
            cache[tempLine].block = (int*) malloc (blockSize * sizeof(int));
            cache[tempLine].tag = tempTag;

            //startAddr is first word of block in main memory
            startAddr = (tempTag*cacheSize)+(tempLine*blockSize);
            for (i=0; i<blockSize; i++) {
                cache[tempLine].block[i] = mainMem[startAddr + i];
            }
        }
        printf("\n** Word %d of Cache Line %d", word, tempLine);
        printf(" with Tag %d contains Value %d\n", cache[tempLine].tag, cache[tempLine].block[word]);
    } else {
        printf("\n** Error - Read Address Exceeds Main Memory\n\n** Trying Again\n");
    }
}

void writeCache(int mainMemSize,int blockSize,int cacheSize,int* mainMem,CacheLine* cache,FILE *filePtr)
{
    int i, r_w_addr, ctntOfAddr, word, tempTag, tempLine, startAddr;
    char isInt;
    readInt(&r_w_addr, &isInt, filePtr);
    readInt(&ctntOfAddr, &isInt, filePtr);

    if (r_w_addr < mainMemSize){
        word = r_w_addr%blockSize;
        tempLine = (r_w_addr%cacheSize)/blockSize;
        tempTag = r_w_addr/cacheSize;

        if (tempTag == cache[tempLine].tag){
            printf("\n** Block in Cache\n");
        } else {
            if(cache[tempLine].block != NULL)
                printf("\n** Collision - First Replace Line of Cache from Memory!\n");
            else
                printf("\n** Write Miss... First load Block from Memory!\n");
            //delete previous cache line and load block with address
            free(cache[tempLine].block);
            cache[tempLine].block = (int*) malloc (blockSize * sizeof(int));
            cache[tempLine].tag = tempTag;

            //startAddr is first word of block in main memory
            startAddr = (tempTag*cacheSize)+(tempLine*blockSize);
            for (i=0; i<blockSize; i++) {
                cache[tempLine].block[i] = mainMem[startAddr + i];
            }
        }
        //write-through
        cache[tempLine].block[word] = ctntOfAddr;//assign value to cache
        mainMem[r_w_addr] = cache[tempLine].block[word];//assign value to main memory from cache

        printf("\n** Word %d of Cache Line %d", word, tempLine);
        printf(" with Tag %d contains Value %d\n", cache[tempLine].tag, cache[tempLine].block[word]);
    } else {
        printf("\n** Error - Write/Read Address Exceeds Main Memory\n");
        printf("\n** Trying Again\n");
    }
}
