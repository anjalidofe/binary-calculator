#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"numstack.h"
#include"opstack.h"
#include <limits.h>
#include "operations.h"
#include "functions.h"

#define OPERATOR 10
#define OPERAND	30
#define END	50
#define ERR 70
//contains tokenization, infix evaluation code for bc model

typedef struct token{
	int type;
	number *no;
	char op;
}token;
enum state { NUMBER, DOT, OP, FINISH, ERROR, SPC };
token gettoken(char *expr, int *reset) {
	static int i = 0;
	number *x = (number *)malloc(sizeof(number));
	init_number(x);
	char currchar;
	static enum state currstate = SPC;
	enum state nextstate;
	token t;
	if(*reset == 1) {
		currstate = SPC;
		*reset = 0;
		i = 0;

	}
	while(1) {
		currchar = expr[i];
		switch(currstate) {
			case NUMBER:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						nextstate = NUMBER;
						add_digit(x, currchar);
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
						nextstate = OP;
						t.type = OPERAND;
						t.no = x;
						currstate = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = FINISH;
						t.type = OPERAND;
						t.no = x;
						currstate = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPC;
						t.type = OPERAND;
						t.no = x;
						currstate = nextstate;
						i++;
						return t;
						break;

					case '.':
						nextstate = DOT;
						i++;
						currstate = nextstate;
						break;

					default: // if none of the above cases then this
						nextstate = ERROR;
						t.type = OPERAND;
						t.no = x;
						currstate = nextstate;
						return t;
						break;
				}
				break;



			case OP:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						t.type = OPERATOR;
						t.op = expr[i - 1];
						nextstate = NUMBER;
						currstate = nextstate;
						//i++;
						return t;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
						nextstate = OP;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = FINISH;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPC;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t;
						break;

					case '.':
						nextstate = DOT;
						t.type = OPERATOR;
						t.op = expr[i -1];
						currstate = nextstate;
						i++;
						return t;
						break;

					default: // if none of the above cases then this
						nextstate = ERROR;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						return t;
						break;
				}
				break;
			case DOT:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						add_digit(x, currchar);
						x->dec++;
						nextstate = DOT;
						currstate = nextstate;
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
						nextstate = OP;
						t.type = OPERAND;
						t.no = x;
						currstate = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = FINISH;
						t.type = OPERAND;
						t.no = x;
						currstate = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPC;
						t.type = OPERAND;
						t.no = x;
						currstate = nextstate;
						i++;
						return t;
						break;

					case '.':
						nextstate = DOT;
						i++;
						currstate = nextstate;
						break;

					default:
						nextstate = ERROR;
						t.type = OPERAND;
						t.no = x;
						currstate = nextstate;
						return t;
						break;
				}
				break;
			case FINISH:
				t.type = END;
				return t;
				break;
			case ERROR:
				t.type = ERR;
				return t;
				break;
			case SPC:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':

						add_digit(x, currchar);
						nextstate = NUMBER;
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
						nextstate = OP;
						i++;
						break;
					case '\0':
						nextstate = FINISH;
						break;
					case ' ':
						nextstate = SPC;
						i++;
						break;

					case '.':
						nextstate = DOT;
						i++;
						break;

					default: // if none of the cases then this
						nextstate = ERROR;
						break;
				}
				currstate = nextstate;
				break;
		}
	}
}
char optop(opstack *s) {
	char ch;
	ch = oppop(s);
	oppush(s, ch);
	return ch;
}
int precedance(char o) {
	switch(o) {
		case ')' :
			return 0;
			break;
		case '+' :
		case '-' :
			return 1;
			break;
		case '*' :
		case '/' :
			return 2;
			break;
		case '%' :
			return 3;
			break;
		case '(' :
			return 4;
			break;
		default :
			return 5;
			break;
	}
	return 5;
}
number *infix(char *exp) { //infix evaluation code
	token t;
	char currop, prevop, ch;
	int cntob = 0, cntcb = 0, reset  = 1, curr, pre;
	number *x, *y, *res;
	x = (number *)malloc(sizeof(number));
	y = (number *)malloc(sizeof(number));
	res = (number *)malloc(sizeof(number));
	init_number(x);
	init_number(y);
	init_number(res);
	num_stack ns;
	opstack os;
	num_init(&ns);
	opinit(&os);
	pre = ERR;
	while(1) {
		t = gettoken(exp, &reset);
		curr = t.type;
		if(curr == pre && pre == OPERAND) {
			return NULL;
		}
		if(t.type == OPERAND){
			num_push(&ns, t.no);
		}
		else if(t.type == OPERATOR){
			currop = t.op;
			if(currop == '(')
				cntob++;
			if(currop == ')')
				cntcb++;
			if(cntcb > cntob)
				return NULL;
			if(!opisempty(&os)) {
				prevop = optop(&os);
				while(precedance(prevop) >= precedance(currop)) {
					prevop = oppop(&os);
					if(!num_isempty(&ns)) {
						x = num_pop(&ns);
					}
					else{
						oppush(&os, prevop);
						break;
					}
					if(!num_isempty(&ns)) {
						y = num_pop(&ns);
					}
					else {
						oppush(&os, prevop);
						num_push(&ns, x);
						break;
					}
					if((prevop == '/' || prevop == '%') && x == 0) {
                        printf("Mathematical error\n");
                        return NULL;
                    }
					switch (prevop) {
						case '+' :
							res = addition(x, y);
							num_push(&ns, res);
							break;
						case '-' :
							res = subtraction(y, x);
							num_push(&ns, res);
							break;
						case '*' :
							res = multiplication(y, x);
							num_push(&ns, res);
							break;
						case '/' :
							res = division(y, x);
							num_push(&ns, res);
							break;
					  case '%' :
							res = modulus(y, x);
							num_push(&ns, res);
							break;
						case '(' :
							oppush(&os, prevop);
							num_push(&ns, y);
							num_push(&ns, x);
							break;
						default :

							return NULL;
					}
					if (prevop == '(')
						break;
					if(!opisempty(&os))
						prevop = optop(&os);
					else
						break;
				}
			}
			oppush(&os, t.op);
			if(currop == ')') {
				ch = oppop(&os);
				cntcb--;
				ch = oppop(&os);
				cntob--;
			}
		}
		else if(t.type == END) {
			if(cntob == cntcb) {
				while(!opisempty(&os)) {
					if(!num_isempty(&ns)) {
						x = num_pop(&ns);
					}
					else {
                        printf("Less operands\n");
                        return NULL;
					}
					if(!num_isempty(&ns)) {
						y = num_pop(&ns);
					}
					else {
                        printf("Less operands\n");
                        return NULL;
					}
					ch = oppop(&os);
					if((ch == '/' || ch == '%') && x == 0) {
                        printf("Mathematical error\n");
						return NULL;
					}
					switch(ch) {
						case '+' :
							res = addition(y, x);
							num_push(&ns, res);
							break;
						case '-' :
							res = subtraction(y, x);
							num_push(&ns, res);
							break;
						case '*' :
							res = multiplication(y, x);
							num_push(&ns, res);
							break;
						case '/' :
							res = division(y, x);
							num_push(&ns, res);
							break;
						case '%' :
							res = modulus(y, x);
							num_push(&ns, res);
							break;
						default :
							return NULL;
					}
				}
			}
			else {
				printf("Error in Expression\n");
				return NULL;
			}
			if(!num_isempty(&ns)){
				res = num_pop(&ns);
				if(num_isempty(&ns))
					return res;
				else {
                        printf("Less Operators\n");
                        return NULL;
                    }
			}
			else {
				printf("Less Operands\n");
				return NULL;
			}
		}
		else if(t.type == ERR) {
			return NULL;
		}
		pre = curr;
	}
}

void modify(char *line) {
	int i;
	for(i = 0; i < strlen(line); i++) {
		if(line[i] == '(') {
			int j = 1;
			while(1) {
				if(line[i + j] == ' ') {
					j++;
				}
				else {
					break;
				}
			}
			if((line[i + j] == '-') || (line[i + j] == '+')) {
				int tmp = strlen(line);
				while(tmp >= i + j) {
					line[tmp + 1] = line[tmp];
					tmp--;
				}
				line[i + j] = '0';
			}
		}
	}
}
int readline(char *line, int l) {
	int i;
	char ch;
	i = 0;
	while(i < l - 1) {
		ch = getchar();
		if(ch == '\n') {
			line[i++] = '\0';
			return i - 1;
		}
		else
			line[i++] = ch;
	}
	line[l - 1] = '\0';
	return l - 1;
}
int main(int argc, char *argv[]) {
	int arg = 0;
	if(argc == 2 && strcmp(argv[1], "-h") == 0){
		printf("MODE :- HELP\n");
		printf("DSA MINIPROJECT by Anjali\nbc 1.06.95\nThis is free software with ABSOLUTELY NO WARRANTY.\n\n");
		printf("This program can perform few basic mathematical operations of bc\n");
		printf("USAGE:-\n");
		printf(" + for addition\n");
		printf(" - for subtraction\n");
        printf(" * for mutltiplication\n");
		printf(" / for division\n");
		printf(" %% for modulus\n");
		printf("Options : \n");
		printf("-h -->help : prints usage and exits\n");
		printf("-v -->version : prints version, copyright and quits\n");
		printf("-w -->warning : prints warning and exits\n");
		exit(0);
	}
	else if(argc == 2 && strcmp(argv[1], "-v") == 0){
        printf("bc Version 1.06.95\nCopyright 1991-1994, 1997, 1998, 2000, 2004, 2006, 2008\n");
        exit(0);
    }
    else if(argc == 2 && strcmp(argv[1], "-w") == 0){
        printf("bc Version 1.06.95\nWARNING :-\nThis is free software with ABSOLUTELY NO WARRANTY.\nMost of the calculations are right but it would be users risk to believe them as we do not guarantee results.\n");
        printf("No extensions to POSIX bc\n");
        exit(0);
    }
	else {

        printf("DSA MINIPROJECT by Anjali\nbc 1.06.95\nThis is free software with ABSOLUTELY NO WARRANTY.\n\n");
        number *r;
        char line[2000];
        while(readline(line, 2000)) {
            modify(line);
            r = infix(line);
            if(r != NULL)
                traverse_number(*r);
            else
                printf("Error in expression\n");
        }
        return 0;
    }
}


