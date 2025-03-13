#include <stdio.h>
#include <stdlib.h>


int func1(int x){
    int pow = x * x;
    int retcode = 0;
    if (pow % 2){
    	retcode = 1;
    } else {
    	retcode = 0;
    }
    return retcode;
}


int main(){
    int x;
    scanf("%d", &x);
    if(x % 2){
    	printf("hello first if\n");
    } else {
	printf("hello first else\n");
    }

    if (x % 3){
    	func1(x);
    }
    
    return 0;
}
