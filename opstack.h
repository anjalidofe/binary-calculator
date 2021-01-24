 /*stack for storing operators which are
 character operators*/
typedef struct opnode{
	char a;
	struct opnode *p;
}opnode;
typedef opnode *opstack;

void opinit(opstack *s);
void oppush(opstack *s, char t);
char oppop(opstack *s);
int opisempty(opstack *s);
