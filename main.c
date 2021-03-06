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
        printf("\n\n==========================================================\n");
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

    strcpy(ref_string, buffer);

    token = strtok(buffer, " ");
    while (token != NULL) {
        ref_list[ref_cnt++] = atoi(token);
        token = strtok(NULL, " ");
    }
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
    int i = 0, j = 0, k = 0,index = 0;
    int fault_cnt = 0;
    int frame_list[30] = {-1};
    int far_list[30] = {0, };

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
                    for(j = i+1; j < ref_cnt; j++) {
                        if(frame_list[k] == ref_list[j]){
                            far_list[k] = j;
                            break;
                        } 
                        far_list[k]=100;
                    }
                }
                int max = far_list[0], maxcnt=0;
                
                for(k = 0; k < frame_cnt; k++){
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
    int i = 0, j = 0, k = 0,index = 0;
    int fault_cnt = 0;
    int frame_list[30] = {-1};
    int far_list[30] = {0, };

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
            if (index == frame_cnt) { //프레임이 꽉찬경우(최근에 가장 호출된 적이 없던 frame out)
                for(k = 0; k < frame_cnt; k++) {
                    for(j = i-1; j > 0; j--) {
                        if(frame_list[k] == ref_list[j]){
                            far_list[k] = j;
                            break;
                        } 
                        far_list[k]=100;
                    }
                }
                int min = far_list[0], mincnt=0;
                
                for(k = 0; k < frame_cnt; k++){
                    if (min > far_list[k]){ min = far_list[k]; mincnt=k;}
                }

                frame_list[mincnt] = ref_list[i];

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

void second_chance() {
    int i = 0, j = 0, k = 0,index = 0;
    int fault_cnt = 0;
    int frame_list[30] = {-1};
    int ref_bit_list[30] = {0, };

    memset(frame_list, -1, sizeof(frame_list));
    bool is_fault = true;

    for(i = 0; i < ref_cnt; i++) {
        is_fault = true;
        for(j = 0; j < frame_cnt; j++) { //frame_list에 이미 있는경우 ref값 1로 만들고 멈춤
            if(frame_list[j] == ref_list[i]) {
                ref_bit_list[j] = 1;
                is_fault = false;
                break;
            }
        }

        if(is_fault){ //페이지 fault가 발생한 경우
            while(1) {
                if (index == frame_cnt)
                    index = 0;

                if(ref_bit_list[index] == 1) { //해당 bit reference값이 1인 경우 0으로 바꾸고 넘어감
                    ref_bit_list[index] = 0;
                    index++;
                } else { //0인경우 frame_list에 집어넣고 fault 카운트 개수 증가
                    frame_list[index++] = ref_list[i];
                    fault_cnt++;
                    break;
                }
            }
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