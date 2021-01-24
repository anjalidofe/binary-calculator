#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dll.h"
#include "operations.h"
#include "functions.h"

//Addition operation
number *addition(number *x, number *y) {
    number *res;
    res = (number *)malloc(sizeof(number));
    init_number(res);
    EqualDecimal(x, y);
    if(x->sign != y->sign) {
        if(x->sign == NEGATIVE) {
            x->sign = POSITIVE;
            res = subtraction(y, x);
        }
        else if(y->sign == NEGATIVE) {
            y->sign = POSITIVE;
            res = subtraction(x, y);
        }
    }
    else if(x->sign == y->sign){
        int i, num1, num2, c = 0, s; //c stands for carry & s for sum
        int lx, ly;
        node *ptr1 = x->tail;
        node *ptr2 = y->tail;
        lx = length(*x);
        ly = length(*y);
        if(x->sign == NEGATIVE)
            res->sign = NEGATIVE;
        else
            res->sign = POSITIVE;
        if(lx >= ly) {
            for(i = 1; i <= ly ; i++) {
                num1 = ptr1->num;
                num2 = ptr2->num;
                s = num1 + num2 + c;
                c = s / 10;
                s = s % 10;
                append(res, s);
                ptr1 = ptr1->prev;
                ptr2 = ptr2->prev;
            }
            for(i = 1; i <= lx - ly; i++) {
                num1 = ptr1->num;
                s = num1 + c;
                c = s / 10;
                s = s % 10;
                append(res, s);
                ptr1 = ptr1->prev;
            }
        }
        else{
            for(i = 1; i <= lx ; i++) {
                num1 = ptr1->num;
                num2 = ptr2->num;
                s = num1 + num2 + c;
                c = s / 10;
                s = s % 10;
                append(res, s);
                ptr1 = ptr1->prev;
                ptr2 = ptr2->prev;
            }
            for(i = 1; i <= ly -lx; i++){
                num1 = ptr2->num;
                s = num1 + c;
                c = s / 10;
                s = s % 10;
                append(res, s);
                ptr2 = ptr2->prev;
            }
        }
        res->dec = x->dec;
        if(c != 0)
            append(res, c);
    }
    return res;
}
//Multiplication operation.
number *multiplication(number *x, number *y) {
	number *ans = (number *)malloc(sizeof(number));
	init_number(ans);
	//check if any no is zero.
	if(Number_is_Zero(*x) == 0 || Number_is_Zero(*y) == 0) {
		add_digit(ans, '0');
		return ans;
	}
	int ld; //length difference
	if(x->sign == y->sign) {
		ans->sign = POSITIVE;
		x->sign = y->sign = POSITIVE;
	}
	else{
		ans->sign = NEGATIVE;
		x->sign = y->sign = POSITIVE;
	}
	ld = length(*x) - length(*y);
	if(ld < 0) {
		ans = multiplication(y, x);
		return ans;
	}
	else{
		node *ptr1, *ptr2;
		int lx = length(*x);
		int ly = length(*y);
		int i, j, num1 = 0, num2 = 0;
		int tempr[2 * lx]; //temporary result: reason for using this is as if lx>=ly, max length of their product will be 2 times lx
		for(i = 0; i < 2 * lx ; i++)
			tempr[i] = 0;
		int k = 2 * lx - 1;
		ptr2 = y->tail;
		for(i = 0; i < ly; i++) {
			ptr1 = x->tail;
			int c1 = 0, c2 = 0; // c1 = carry1 & c2 = carry2
			for(j = k - i; j > lx - 2; j--) {
				if(ptr1 != NULL && ptr2 != NULL) {
					num1 = ptr1->num *  ptr2->num + c1;
					ptr1 = ptr1->prev;
					c1 = num1 / 10;
					num1 = num1 % 10;
					num2 = tempr[j] + num1 + c2;
					c2 = num2 / 10;
					num2 = num2 % 10;
					tempr[j] = num2;
				}
				else{
					break;
				}
			}
			tempr[j] = c1 + c2 + tempr[j];
			lx--;
			ptr2 = ptr2->prev;
		}
		for(i = k; i >= lx - 1 && i >= 0; i--) {
			append(ans, tempr[i]);
		}
		ans->dec = x->dec + y->dec;
		return ans;
	}
}

//SUBTRACTION OPERATION
number *subtraction(number *x, number *y) {
	number *res;
	res = (number *)malloc(sizeof(number));
	init_number(res);
	//make decimal length and length of both numbers equal
	EqualDecimal(x, y);
	EqualLength(x, y);
	if(x->sign != y->sign) {
		if(x->sign == NEGATIVE) {
			x->sign = POSITIVE;
			res = addition(x, y);
			res->sign = NEGATIVE; // form would be -x-y = -(x+y)
		}
		else if(y->sign == NEGATIVE) {
			y->sign = POSITIVE;
			res = addition(x, y);
			res->sign = POSITIVE; // form would be x-(-y) = x+y
		}
	}
	else if(x->sign == y->sign) {
		if(x->sign == NEGATIVE) {
			x->sign = y->sign = POSITIVE;
			res = subtraction(y, x); // -x-(-y) = -x+y = y-x
		}
		if(x->sign == POSITIVE) {
			int num1, num2, d, b = 0, i , l;//difference = d; borrow = b;
			node *ptr1 = x->tail;
			node *ptr2 = y->tail;
			//length of both the no.s are same now due to EqualLength
			l = length(*y);
            if(LengthCompare(*x, *y) == 1) {//when x>y will be true
                for(i = 1; i <= l; i++){
                    num1 = ptr1->num;
                    num2 = ptr2->num;
                    num1 = num1 - b;
                    if(num1 >= num2) {
                        d = num1 - num2;
                        b = 0;
                        append(res, d);
                    }
                    else{
                        num1 = num1 + 10;
                        d = num1 - num2;
                        b = 1;
                        append(res, d);
                    }
                    ptr1 = ptr1->prev;
                    ptr2 = ptr2->prev;
                }
            }
            else if(LengthCompare(*x, *y) == -1) {//when x<y will be true
                res->sign = NEGATIVE;
                for(i = 1; i <= l; i++){
                    num1 = ptr1->num;
                    num2 = ptr2->num;
                    num2 = num2 - b;
                    if(num2 >= num1) {
                        d = num2 - num1;
                        b = 0;
                        append(res, d);
                    }
                    else{
                        num2 = num2 + 10;
                        d = num2 - num1;
                        b = 1;
                        append(res, d);
                    }
                    ptr1 = ptr1->prev;
                    ptr2 = ptr2->prev;
				}
			}
			else{
				if(LengthCompare(*x, *y) == 0){
					append(res, 0);
				}
			}
		}
	}
	res->dec = x->dec;
	return res;
}
//Division operation
number *division(number *x, number *y) {
	if(Number_is_Zero(*y) == 0) {
		printf("ERROR : Division by zero is not defined.\n");
		return NULL;
	}
	/*number *res;
	res = (number *)malloc(sizeof(number));
	init_number(res);
	if(Number_is_Zero(*x) == 0 && Number_is_Zero(*y) != 0){
        append(res, 0);
        return res;
	}*/
	int i = 0, k;
	if(x->dec > y->dec) {
        k = x->dec;
	}
    else {
        k = y->dec;
    }
	while(i < k) { //this loop removes decimal points of the numbers
		if(x->dec > 0)
			x->dec--;
		else
			add_digit(x, '0');
		if(y->dec > 0)
			y->dec--;
		else
			add_digit(y, '0');
		i++;
	}
	number *a , *b , *intres, *quotient, *pr;
	a = (number *)malloc(sizeof(number));
	b = (number *)malloc(sizeof(number));
	intres = (number *)malloc(sizeof(number)); //intermediate result for repeated subtraction
	quotient = (number *)malloc(sizeof(number));//final answer
	pr = (number *)malloc(sizeof(number));//intermediate result of multiplication

	init_number(a);
	init_number(b);
    init_number(intres);
	init_number(quotient);
	init_number(pr);
	if(x->sign == y->sign) {
		quotient->sign = POSITIVE;
		x->sign = y->sign = POSITIVE;
	}
	else{
		quotient->sign = NEGATIVE;
		x->sign = y->sign = POSITIVE;
	}
	node *p = x->head;
	char ch = p->num + '0';
	add_digit(b, ch);
	i = 9;
	while(quotient->dec < SCALE){
		while(i >= 0){
			append(a, i);
			pr = multiplication(y, a);
			intres = subtraction(b, pr);
			if(intres->sign != NEGATIVE) {
				add_digit(quotient, i + '0');
				node *temp = a->head;
				free(temp);
				a->head = a->tail = NULL;
				break;
			}
			else{
				node *temp = a->head;
				free(temp);
				a->head = a->tail = NULL;
				i--;
			}
		}
		b = intres;
		if(p->next != NULL) {
			p = p->next;
			ch = p->num + '0';
			add_digit(b, ch);
		}
		else{
			quotient->dec++;
			add_digit(b, '0');
		}
		i = 9;
		node *temp = a->head;
		free(temp);
		a->head = a->tail = NULL;
	}
	quotient->dec--;
	return quotient;
}

//Modulus operation
number *modulus(number *x, number *y) {
	if(Number_is_Zero(*y) == 0) {
		printf("ERROR : Modulo by zero is not allowed.\n");
		return NULL;
	}
	int tempsign;
	if(x->sign == NEGATIVE) {
		tempsign = NEGATIVE;
		x->sign = y->sign = POSITIVE;
	}
	else{
		tempsign = POSITIVE;
		x->sign = y->sign = POSITIVE;
	}
	EqualDecimal(x, y);
	int xdec = x->dec;
	number *mod = (number *)malloc(sizeof(number));
	number *temp = (number *)malloc(sizeof(number));
	init_number(mod);
	init_number(temp);
	temp = division(x, y);
	if(temp->dec != 0){
		int position = length(*temp)-1;
		while(temp->dec != 0) {
			remove_digit(temp, position);
			temp->dec--;
			position--;
		}
	}
	temp = multiplication(temp, y);
	mod = subtraction(x, temp);
	mod->sign = tempsign;
	mod->dec = xdec;
	return mod;
}


