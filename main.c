#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdbool.h>


#define BUFFER_SIZE 256
#define MAX_REF 30
#define OPT 0
#define FIFO 1
#define LRU 2
#define SC 3

void input_file();
void print_header();
void opt();
void fifo();
void lru();
void second_chance();

int frame_cnt = 0;
int ref_cnt = 0;
int ref_list[MAX_REF] = {0};
char ref_string[BUFFER_SIZE];

int main(void) {
    input_file();
    for (int i = 0; i < 4; i++) {
        printf("Used Method : ");
        switch(i) {
            case OPT :
                printf("OPT\n");
                print_header();
                opt();
                break;
            case FIFO :
                printf("FIFO\n");
                print_header();
                fifo();
                break;
            case LRU : 
                printf("LRU\n");
                print_header();
                lru();
                break;
            case SC :
                printf("Second-Chance\n");
                print_header();
                second_chance();
                break;
        }
    }
}

void input_file() {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    char *token;

    printf("file name > ");
    scanf("%s", buffer);

    if ((fp = fopen(buffer, "r")) == NULL) { //파일 오픈
        fprintf(stderr, "fopen error for %s\n", buffer);
        exit(1);
    }

    fscanf(fp, "%d\n%[^\n]", &frame_cnt, buffer);
    printf("frame_cnt : %d\n",frame_cnt);
    printf("buffer :%s\n",buffer);

    strcpy(ref_string, buffer);

    token = strtok(buffer, " ");
    while (token != NULL) {
        ref_list[ref_cnt++] = atoi(token);
        token = strtok(NULL, " ");
    }
    printf("count : %d\n",ref_cnt);
}

void print_header() {
    printf("page reference string : %s\n", ref_string);
    printf("\n\tframe");
    for (int i = 0; i < frame_cnt; i++)
        printf("\t%d", i + 1);
    printf("\tpage fault\n");
    printf("time\n");
}

void opt() {
    printf("opt\n");
    int i = 0, j = 0, k = 0,index = 0;
    int fault_cnt = 0;
    int frame_list[30] = {-1};
    int far_list[30] = {0};

    memset(frame_list, -1, sizeof(frame_list));
    bool is_fault = true;
    for(i = 0; i < ref_cnt; i++) {
        is_fault = true;
        for(j = 0; j < frame_cnt; j++) {
            if(frame_list[j] == ref_list[i]) {
                is_fault = false;
                break;
            }
        }

        if(is_fault) {
            if (index == frame_cnt) { //프레임이 꽉찬경우(미래에 가장 덜 호출될 frame out)
                for(k = 0; k < frame_cnt; k++) {
                    for(j = i; j < ref_cnt; j++) {
                        if(frame_list[k] == ref_list[j]) far_list[k] = i;
                    }
                }
                int max = 0, maxcnt=0;
                for(k = 0;k<frame_cnt;k++){
                    if (max < far_list[k]){ max = far_list[k]; maxcnt=k;}
                }
                frame_list[maxcnt] = ref_list[i];

            } else {
                frame_list[index++] = ref_list[i];
            }
            fault_cnt++;
        }
        printf("%d\t\t", i+1);
        for (j = 0; j < frame_cnt; j++)
            if (frame_list[j] != -1)
                printf("%d\t", frame_list[j]);
            else
                printf("\t");
        if (is_fault)
            printf("F");
        printf("\n");
        }

    printf("Number of page faults: %d times\n", fault_cnt);

}

void fifo() {
    printf("fifo\n");
    printf("ref_count : %d\n",ref_cnt);

    int i = 0, j = 0, index = 0;
    int fault_cnt = 0;
    int frame_list[30] = {-1};

    memset(frame_list, -1, sizeof(frame_list));
    
    bool is_fault = true;

    for(i = 0; i < ref_cnt; i++) {
        is_fault = true;
        for(j = 0; j < frame_cnt; j++) {
            if(frame_list[j] == ref_list[i]) {
                is_fault = false;
                break;
            }
        }

        if(is_fault) {
            frame_list[index++] = ref_list[i];
            fault_cnt++;
        }

        if (index == frame_cnt)
            index = 0;

        printf("%d\t\t", i+1);
        for (j = 0; j < frame_cnt; j++)
            if (frame_list[j] != -1)
                printf("%d\t", frame_list[j]);
            else
                printf("\t");
        if (is_fault)
            printf("F");
        printf("\n");

    }
    printf("Number of page faults: %d times\n", fault_cnt);

}

void lru() {
    printf("lru\n");

}

void second_chance() {
    printf("second chance\n");
}