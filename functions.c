#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include"dll.h"
#include"functions.h"

//To make the digits equal after the decimal point by adding 0.
void EqualDecimal(number *x, number *y) {
	int i;
	if(y->dec > x->dec){
		int g = y->dec - x->dec;
		for(i = 0; i < g; i++) {
			add_digit(x, '0');
			x->dec++;
		}
	}
	else if(x->dec > y->dec) {
		int g = x->dec - y->dec;
		for(i=0; i < g ; i++) {
			add_digit(y, '0');
			y->dec++;
		}
	}
	return;
}

//to make the length of both no.s equal by inserting zeros in the beginning
void EqualLength(number *x, number *y){
	int g;
	g = length(*x) - length(*y);
	if(g > 0) {
		int i = 0;
		while(i < g) {
			append(y, 0);
			i++;
		}
	}
	else if(g < 0) {
		int i = 0;
		g = -g;
		while(i < g){
			append(x, 0);
			i++;
		}
	}
}

// To find the larger and smaller number among the two numbers with equal length
int LengthCompare(number x, number y) {
	EqualLength(&x, &y);
	EqualDecimal(&x, &y);
	node *s, *r;
	int len;
	len = length(x);
	s = x.head;
	r = y.head;
	for(int i = 1; i <= len; i++) {
		if(s->num > r->num)
			return 1; //shows that x is greater than y
		else if(s->num < r->num)
			return -1; //indicates that x is less than y
		s = s->next;
		r = r->next;
	}
	return 0; //implies that both the numbers are equal.
}

// to check whether the number is zero or not
int Number_is_Zero(number x) {
	int i, dig = 0;
	node *p = x.head;
	for(i = 0; i < length(x); i++) {
		if(p->num != 0)
			dig = 1;
		p = p->next;
	}
	return dig; //if no. is 0 returns 0 or else returns 1
}/*

//copies number a in b
void copy(number *x, number *y) {
	char ch;
	int i, d, len;

	len = length(*x);
	node *t = x->head;
	for(i = 0; i < len; i++) {
		d = t->num;
		ch = d + '0';
		add_digit(y, ch);
		t = t->next;
	}
	y->dec = x->dec;
	y->sign = x->sign;
	return;
}*/

