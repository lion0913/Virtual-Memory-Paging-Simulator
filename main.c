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

int frame_cnt = 0;
int ref_list[MAX_REF] = {0};
char ref_string[BUFFER_SIZE];

int main(void) {
    input_file();
    for (int i = 0; i < 4; i++) {
        printf("Used Method : ");
        switch(i) {
            case OPT :
                printf("OPT\n");
                break;
            case FIFO :
                printf("FIFO\n");
                break;
            case LRU : 
                printf("LRU\n");
                break;
            case SC :
                printf("Second-Chance\n");
                break;
        }
        printf("page reference string : %s\n", ref_string);
        printf("\n\tframe");
        for (int i = 0; i < frame_cnt; i++)
            printf("\t%d", i + 1);
        printf("\tpage fault\n");
        printf("time\n");
        }
}

void input_file() {
    FILE *fp;
    char buffer[BUFFER_SIZE];

    printf("file name > ");
    scanf("%s", buffer);

    if ((fp = fopen(buffer, "r")) == NULL) {
        fprintf(stderr, "fopen error for %s\n", buffer);
        exit(1);
    }

    fscanf(fp, "%d\n%[^\n]", &frame_cnt, buffer);
    printf("frame_cnt : %d\n",frame_cnt);
    printf("buffer :%s\n",buffer);

    strcpy(ref_string, buffer);

    char *token;
    int i = 0;

    token = strtok(buffer, " ");
    while (token != NULL) {
        ref_list[i++] = atoi(token);
        token = strtok(NULL, " ");
    }
}
