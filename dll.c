#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "dll.h"

void init_number(number *x){ //initializing to NULL
    x->head = x->tail = NULL;
    x->dec = 0;
    x->sign = POSITIVE; //setting positive by default in beginning
}


void append(number *x, int n){ //appending number in the beginning
    if(n < 0 || n > 9)
        return;
    node *temp = (node*)malloc(sizeof(node));
    if(!temp)
        return;
    temp->num = n;
    temp->prev = NULL;
    if((x->head == NULL) && (x->tail == NULL)){
        temp->next = NULL;
        x->head = x->tail = temp;
        return;
    }
    temp->next = x->head;
    x->head->prev = temp;
    x->head = temp;
    return;
}


void add_digit(number *x, char ch){ //adds digit in end , char passed in is converted to int and added to number
    if(ch < '0' || ch > '9')
		return;
	node *temp = (node *)malloc(sizeof(node));
	if(temp == NULL)
		return;
	temp->num = ch - '0';
	temp->next = NULL;
	if((x->head == NULL) && (x->tail == NULL)){
		temp->prev = NULL;
		x->head = temp;
		x->tail = temp;
		return;
	}
	temp->prev = x->tail;
	x->tail->next = temp;
	x->tail = temp;
	return;
}


void remove_zero(number *x){ //removes extra zeros on left side of number
    node *p = x->head;
	int i, l = length(*x);
	for(i = 0; i < l - x->dec -1; i++){
		if(p->num == 0){
			x->head = p->next;
			p->next = NULL;
			x->head->prev = NULL;
			free(p);
			p = x->head;
		}else {
			break;
		}

	}
	p = x->tail;   //removing zeros after decimal i.e., right to the decimal point
	int deci = x->dec;
	for(i = 0; i < deci -1; i++){
		if(p->num == 0){
			x->tail = p->prev;
			p->prev = NULL;
			x->tail->next = NULL;
			free(p);
			p = x->tail;
			x->dec--;
		}else{
			break;
		}
	}
	return;
}

void remove_digit(number *x, int position){
    int i;
	node *p, *temp;
	// handles errors on position
	if(position < 0 || position >= length(*x))
		return;

	p = x->head;
	for(i=0; i < position - 1;i++)
		p = p->next;
	if(x->head == NULL) {
		return;
	}
	if(length(*x) == 1) {
		x->head = x->tail = p = NULL;
		return;
	}
	if(position == 0){
		p->next->prev = NULL;
		x->head = p->next;
		free(p);
		return;
	}
	if(position == length(*x) - 1) {
		temp = p->next;
		p->next = NULL;
		x->tail = p;
		free(temp);
		return;
	}

	temp = p->next;
	p->next = p->next->next;
	temp->next->prev = p;
	free(temp);
	return;
}


int length(number x){ //length of number
    node *p;
	p = x.head;
	int count = 0;
	while(p != NULL){
		count++;
		p = p->next;
	}
	return count;
}

void traverse_number(number x){
    node *p;
	int position = 0;
	remove_zero(&x);
	p = x.head;
	if(x.sign == NEGATIVE){
		printf("-");
		x.sign = POSITIVE;
	}
	while(p != NULL){
		position++;
		if(position == (length(x) - x.dec + 1))
			printf(".");
		printf("%d" , p->num);
		p = p->next;
	}
	printf("\n");
}

/*int digit_at(number *x, int position) {
	node *p;
	if(position < 0 || position > length(* x))
		return 0;
	p = (*x).head;
	int i;
	for(i = 0; i < position; i++)
		p = p->next;
	return p->num;
}
*/
