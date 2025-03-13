#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// void mdafl_if_range(int index, int share_time, int val, int low, int high, int is_boolean)

void func1(int para1, int para2){

    mdafl_if_range(3, 1, para1 * para2, 0, INT_MAX, 0);
    if (para1 > 0 && para2 > 0){
        printf("[func1]: enter the first if range\n");

        mdafl_if_range(4, 1, para1, 0, 1000000, 0);
        if (para1 < 1000000){
            printf("[func1]: enter the second if range\n");

            mdafl_if_range(5, 1, para2, 0, 1000000, 0);    
            if (para2 < 1000000){
                printf("[func1]: enter the third if range\n");
            }
        }
    }

}

void func2(int para1, int para2){
    
    mdafl_if_range(6, 1, para1 * para2, 0, INT_MAX, 0);
    if (para1 * para2 > 0){
        printf("[func2]: enter the first if range\n");

        mdafl_if_range(7, 1, para1, 0, INT_MAX, 0);
        if (para1 > 0){
            printf("[func2]: enter the second if range\n");

            mdafl_if_range(8, 1, para2, 0, INT_MAX, 0);
            if (para2 > 0){
                printf("[func2]: enter the third if range\n");
            }
        }
    }
}

void func3(int para1, int para2){

    mdafl_if_range(9, 1, para1 * para2, INT_MIN, 0, 0);
    if (para1 * para2 < 0){
        printf("[func3]: enter the first if range\n");

        mdafl_if_range(10, 1, para1, 0, INT_MAX, 0);
        if (para1 > 0){
            printf("[func3]: enter the second if range\n");

            mdafl_if_range(11, 1, para2, INT_MIN, 0, 0);
            if (para2 < 0){
                printf("[func3]: enter the third if range\n");
            }
        }
    }

}


int main(){
    int para1 = 0, para2 = 0;
    scanf("%d", &para1);
    para2 = para1 + 60;

    printf("[main]: para1 is %d, para2 is %d\n", para1, para2);
    // 0-1 call func1
    mdafl_if_range(0, 1, para1, 0, 1, 0);
    if (0 < para1 && para1 < 1){
        printf("[main]: choosed func1\n");
        func1(para1, para2);
    }

    // 1-5 call func2
    mdafl_if_range(1, 1, para1, 1, 5, 0);
    if (1 < para1 && para1 < 5){
        printf("[main]: choosed func2\n");
        func2(para1, para2);
    }

    // 5-50 call func3
    mdafl_if_range(2, 1, para1, 5, 50, 0);
    if (5 < para1 && para1 < 50){
        printf("[main]: choose func3\n");
        func3(para1, para2);
    }

    return 0;
}