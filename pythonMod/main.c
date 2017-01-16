#include <stdio.h>
#include <string.h>


double toto(double d){
	printf("Yay");
	return 2*d;	
}

char* print(char* st){
	printf("st : %s",st);
	char* ret = "Hello World !!";
	return ret;
}

int rien(int i){
	printf("i : %d",i);
	return 0;
}