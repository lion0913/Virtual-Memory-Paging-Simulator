#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/syscall.h>


#define BUFFER_SIZE 256
#define MAX_REF 30
#define OPT 0
#define FIFO 1
#define LRU 2
#define SC 3

void input_file();
char buffer[BUFFER_SIZE];

int frame_cnt = 0;
int ref_list[MAX_REF] = {0};

int main(void) {
    input_file();
    printf("Used Method : ");
    for (int i = 0; i < 4; i++) {
        switch(i) {
            case OPT :
                print("OPT\n");
                break;
            case FIFO :
                print("FIFO\n");
                break;
            case LRU : 
                print("LRU\n");
                break;
            case SC :
                print("Second-Chance\n");
                break;
        }
        printf("page reference string : %s\n",buffer);
    }
}

void input_file() {
    FILE *fp;
    

    printf("file name > ");
    scanf("%s", buffer);

    if ((fp = fopen(buffer, "r")) == NULL) {
        fprintf(stderr, "fopen error for %s\n", buffer);
        exit(1);
    }

    fscanf(fp, "%d\n%[^\n]", &frame_cnt, buffer);
    printf("frame_cnt : %d\n",frame_cnt);
    printf("buffer :%s\n",buffer);

    char *token;
    int i = 0;

    token = strtok(buffer, " ");
    while (token != NULL) {
        ref_list[i++] = atoi(token);
        token = strtok(NULL, " ");
    }
}
