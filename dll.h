#define POSITIVE 1
#define NEGATIVE -1

typedef struct node{
    int num;
    struct node *next;
    struct node *prev;
}node;

typedef struct number{
    int sign;
    int dec;
    node *head;
    node *tail;
}number;

void init_number(number *x);
void append(number *x, int n);//adds digit in beginning
void add_digit(number *x, char ch);//adds digit in end
int length(number x);
void remove_zero(number *x);
void remove_digit(number *x, int position);
void traverse_number(number x);
//int digit_at(number *x, int position);
