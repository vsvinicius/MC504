#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define COLUMNS 13
#define ROWS 12
#define WORDS 6
#define MAX_WORD 8

typedef struct {
  char** matriz;
  char* word;
} Args;

void* find_word(void* arguments) {
    Args *args = arguments;
    char **matriz = args->matriz;
    char* word = args->word;
    printf("PALAVRA PROCURADA: %s\n\n",args->word);
    int i,j;
    for(i=0;i<ROWS;i++){
        char* response = strstr(matriz[i],word);
        if(response != NULL){
            for(int k=0;k < strlen(word);k++){
                response[k] = toupper(response[k]);
            }
            return NULL;
        }
    }
    for(j=0;j<COLUMNS;j++){
        char column[13];
        for(int i=0;i<ROWS;i++){
            column[i] = matriz[i][j];
        }
        column[12] = '\0';
        char* response = strstr(column,word);
        if(response != NULL){
            int position = response - column;
             for(int k=position;k < strlen(word) + position; k++){
                matriz[k][j] = toupper(response[k-position]);
            }
            return NULL;
        }

    }

    return NULL;
};

int main() {

    FILE *file;

    if ((file = fopen("input.in", "r")) == NULL) {
        printf("Error! opening file");
        return 1;
    }
    char **matriz = malloc((sizeof(char *)) * ROWS);
    char *string = malloc((sizeof (char) * COLUMNS));
    char **words = malloc((sizeof (char *)) * WORDS);
    int i = 0;
    while(fscanf(file, "%s", string ) == 1){
        if(isdigit(string[0])){
            break;
        }
        matriz[i] = malloc(sizeof(char) * COLUMNS);
        strcpy(matriz[i],string);
        i++;
    }
    i = 0;
    while(fscanf(file, "%s", string ) == 1){
        words[i] = malloc(sizeof(char) * MAX_WORD);
        strcpy(words[i],string);
        i++;
    }

    pthread_t thr[WORDS];
    
    for(int i=0;i < WORDS;i++){
        Args *args = malloc(sizeof *args);
        args->matriz = matriz;
        args->word = words[i];
        pthread_create(&thr[i], NULL, find_word,args);
    }
    
    for(int i=0;i < WORDS;i++)
        pthread_join(thr[i], NULL);

    printf("%s\n\n","------------------------------");

    for(int i=0; i < ROWS; i++) {
        for (int j=0; j < COLUMNS; j++){
            printf("%c ",matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}