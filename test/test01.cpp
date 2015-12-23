#include<stdio.h>
#include<list>

int mult(int aaa){
	return 2*aaa;
}
int getX(){
    printf("test\n");
    return 10+mult(10);
}

int main(void){
    printf("%d\n", getX());
}






