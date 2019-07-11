#include <stdio.h>
#include <stdlib.h>

int main()
{
    int lineNum = 0;
    char c;
    while((c = getchar()) != EOF)
    {
        printf("%d\t",++lineNum);
        putchar(c);
        while (c != '\n' && c != EOF)
        {
            c = getchar();
            putchar(c);
        }
    }
    return 0;
}
