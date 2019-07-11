/*
 * Moses Cuevas
 * Assignment 3: Hamming Codes
 * Comp 222 - Fall 2017
 * Meeting Time: 11:00-12:15
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* declare global vars */
/* define hamming string as a dynamic array of characters */

/***************************************************************/
void setParameters(FILE *filePtr, int *length, int *parity, char **hamStr)
{
/* prompt for maximum hamming code length and for even/odd parity */
    do{
        fscanf(filePtr,"%d", length);
        if (*length<=0){
            printf("\n*** Error - Length Must be Greater than Zero\n");
        }
    }while(*length<=0);
    do{
        fscanf(filePtr,"%d", parity);
        if (*parity != 1 && *parity != 0){
            printf("\n*** Error - Parity Must be Zero or One\n");
        }
    }while(*parity != 1 && *parity != 0);
    printf("\n\n*** Maximum Code Length is: %d\n", *length);
    if (*parity == 1)
        printf("\n*** Parity is: Odd\n\n");
    else
        printf("\n*** Parity is: Even\n\n");

/* allocate memory for hamming string based on maximum length and size of a character element*/
    free(*hamStr);
    (*hamStr) = (char*) malloc((*length) * sizeof(char));

    return;//why do i need this?--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--
}

/***************************************************************/
void checkCode(FILE *filePtr, int length, int parity, char *hamStr)
{
/* declare local vars */
    //char tempCode[50];
    int numCheckBits;//numCheckBits represent C1, C2, C4, C8 ...
    int i,j,k;
    int result, power, errorBit=0;

/* prompt for hamming code as a "string"*/

    char *tempCode;//garbage code that works like garbage
    char endLoop = 'f';
    int ch;
    size_t len = 0;
    size_t size = length;
    tempCode = realloc(NULL, sizeof(char)*size);//size is start size
    if(!tempCode) endLoop ='t';
    ch=fgetc(filePtr);//clear '\n' from input buffer
    while(EOF!=(ch=fgetc(filePtr)) && ch != '\n' && endLoop == 'f'){
        tempCode[len++]=ch;
        if(len==size){
            tempCode = realloc(tempCode, sizeof(char)*(size+=16));
            if(!tempCode) endLoop ='t';
        }
    }
    tempCode[len++]='\0';

    tempCode = realloc(tempCode, sizeof(char)*len);


    printf("tempCode length: %d\n", strlen(tempCode));
    printf("MaxLength: %d\n", length);
    //fscanf(filePtr,"%s", tempCode);
    numCheckBits = (int) ceil(log(strlen(tempCode))/ log(2));//ceil() returns a double value
    printf("numCheckBits: %d\n", numCheckBits);
    /*char temp;
    do{
        fscanf(filePtr,"%c", temp);
        tempCode[i] = temp;
        i++;
    }while(i<length && temp!='\0')*/

    if (strlen(tempCode)>length){
        printf("\n*** The incorrect Hamming Code is: %s\n", tempCode);
        printf("\n*** Invalid Entry - Code Exceeds Maximum Code Length of %d\n\n", length);
    }
    else {

/* OUTER LOOP: for each parity bit in the Hamming code*/
        for(i=0; i < numCheckBits; i++){
/* BEGIN OF OUTER LOOP */
/* initialize appropriate local variables */
            result = parity;
            power = (int) pow(2, i);//for skipping elements in hamStr
/* MIDDLE LOOP: for each starting bit of a sequence */
            for(j = strlen(tempCode) - power; j >= 0; j -= (power * 2)){
 /* INNER LOOP: for each bit to be checked, check value and update parity information for current parity bit*/
                for(k = j; k > (j - power) && k >= 0; k--){
                     result = result ^ (tempCode[k] - '0');
                }
            }
            printf("\n*** C%d result: %d\n", power, result);
/* update parity checking by including result for parity bit checked in total */
            if(result != 0)
                errorBit += power;
        }
/* END OF OUTER LOOP */
/* report error in hamming code based on result from parity bits or report no error if none */
        if(errorBit!=0){
            printf("\n*** The incorrect Hamming Code is: %s\n", tempCode);
            printf("\n*** There is an error in bit: %d\n", errorBit);

            if (tempCode[strlen(tempCode)-errorBit] == '0')//changes the bit
                tempCode[strlen(tempCode)-errorBit]='1';
            else
                tempCode[strlen(tempCode)-errorBit]='0';
        }
        else
            printf("\n*** There are no bits in error\n");
/* correct hamming code by flipping error bit (if necessary)*/
        printf("\n*** The correct Hamming Code is: %s\n\n", tempCode);//don't forget to change this_____________________________
    }
    return;//why do i need this?--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--
}

/***************************************************************/
int main(int argc, char *argv[])
{
/* print out menu, prompt for choice, and call appropriate procedure until user quits */
    int length=0, parity;
    char *hamStr = NULL;//hamming string - may be required to make a global variable
    FILE *filePtr;
    filePtr = fopen(argv[1], "r");
    //filePtr = fopen("hc_data_2017.txt", "r");//for testing only--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--

    printf("*** Moses Cuevas\n");
    printf("*** Assignment 3: Hamming Codes\n");
    printf("*** Comp 222 - Fall 2017\n");
    printf("*** Class Meeting Time: 11:00 - 12:15\n");

    if (filePtr == NULL) {
        printf("*** File \"%s\" Not Found. Program Terminating.\n", argv[1]);
    }
    else {
        int choice;
        do {
            fscanf(filePtr,"%d", &choice);//reads int, check this later--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--
            switch(choice)
            {
            case 1:
                //enter parameters
                setParameters(filePtr, &length, &parity, &hamStr);
                break;
            case 2:
                //enter hamming code
                checkCode(filePtr, length, parity, hamStr);
                break;
            case 3:
                //exit
                break;
            default:
                printf("\nInvalid Menu Input: %d\n",choice);
            }
        }while (choice != 3);
        free(hamStr);
        fclose(filePtr);
        printf("*** Program Terminated Normally\n\n");
    }
    return 0;//why return 1?--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--<+>--+--
}
