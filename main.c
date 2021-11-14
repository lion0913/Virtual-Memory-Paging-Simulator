#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/syscall.h>


#define BUFFER_SIZE 256

void input_file();

int frame_cnt = 0;

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

    fscanf(fp, "%d\n%[^\n]", frame_cnt, buffer);
    printf("frame_cnt : "+frame_cnt);
    printf("buffer :"+buffer);
}
