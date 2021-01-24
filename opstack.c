#include <stdlib.h>
#include <stdlib.h>
#include "opstack.h"

void opinit(opstack *s) {
	*s = NULL;
}


void oppush(opstack *s, char t){
	opstack temp;
	temp = (opnode*)malloc(sizeof(opnode));
	temp->a = t;
	temp->p = (*s);
	*s = temp;
}

char oppop(opstack *s){
	char t;
	opstack temp;
	temp = (*s);
	t = (*s)->a;
	*s = (*s)->p;
	free(temp);
	return t;
}

int opisempty(opstack *s){
	return *s == NULL;
}




