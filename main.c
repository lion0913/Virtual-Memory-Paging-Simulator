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

void input_file();

int frame_cnt = 0;
int ref_list[MAX_REF] = {0};

int main(void) {
    input_file();

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

    char *token;
    int i = 0;

    token = strtok(buffer, " ");
    while (token != NULL) {
        ref_list[i++] = atoi(token);
        token = strtok(NULL, " ");
        printf("%d번째 값 : %d\n",i,token);
    }
}
