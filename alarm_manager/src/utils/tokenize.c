#include "utils/tokenize.h"
#include "utils/program_log.h"
#include <string.h>
#include <stdlib.h>

#define TOKEN_LENGTH 15

char** tokenize (char* str, unsigned sz)
{
    my_log("tokenize: start", 1);

    if (str == NULL)
        return NULL;

    char** temp = (char**)malloc(sizeof(char*) * sz);
    if (temp == NULL)
    {
        my_log("tokenize: malloc **", 0);
        exit(EXIT_FAILURE);
    }

    char* token = strtok(str, " ");
    temp[0] = (char*)malloc(sizeof(char) * TOKEN_LENGTH);
    if (temp[0] == NULL)
    {
        my_log("tokenize: malloc [0]", 0);
        exit(EXIT_FAILURE);
    }

    strcpy(temp[0], token);
    for (unsigned i = 1; i < sz; i++) 
    {
        token = strtok(NULL, " ");
        if (token == NULL)
            break;
        temp[i] = (char*)malloc(sizeof(char) * TOKEN_LENGTH);
        if (temp[0] == NULL)
        {
            my_log("tokenize: malloc [i]", 0);
            exit(EXIT_FAILURE);
        }

        strcpy(temp[i], token);
    }
    return temp;
}
