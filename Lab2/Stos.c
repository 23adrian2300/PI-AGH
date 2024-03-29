#include <stdio.h>
#include <math.h> // for NAN

#define STACK_SIZE 10

#define OK        0
#define EMPTY    -1
#define FULL     -2
#define OVERFLOW1 -3

double stack[STACK_SIZE];
int top = 0;

int stack_push(double x)
{ if (top+1 >STACK_SIZE){
    printf("OVERFLOW ");
    return OVERFLOW1;};

   top=top+1;
   stack[top] = x;
    return OK;
}

double stack_pop(void)
{   if(top-1 == -1){return NAN;};
    top=top-1;
    return stack[top+1];

}

int stack_state(void)
{ if (top == STACK_SIZE){return FULL;};
    return top;

}

// FIFO queue with shifts

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 0;

// ONE OVERFLOW for many resigning


void queue_push(int in_nr)
{   int k=0, curr = curr_nr;
    for(int i = in ; i<in_nr+in;++i){
        k+=1;
        if(in+k>QUEUE_SIZE){
            in = QUEUE_SIZE;
            for(int j = i ; j<in_nr+in; ++j)
        {   curr_nr+=1;}
            printf("OVERFLOW ");
            return;}
        queue[i] = curr+1;
        curr+=1;
        }
    in = in+in_nr;
    curr_nr = curr;
    }





void queue_pop(int out_nr)
{   if(in - out_nr < 0){
    for(int i=0; i <in ;++i){
        queue[i] = queue[i + out_nr];
        }
    in = 0;
    printf("UNDERFLOW ");
    return;}

    for(int i=0; i<in-out_nr+1;++i)
{  if(queue[i+out_nr] != 0){
    queue[i] = queue[i+out_nr];}
   else{
   in = in-out_nr;
    return;
    }}
in = in-out_nr;}

void queue_state(void)
{   if (in == 0)
{   printf("EMPTY ");
    return;};

    for(int i=0; i<in;++i)
{   if(queue[i]!=0){
    printf("%d", queue[i]);
    printf(" ");}
else
    {return;}
}}



// Queue with cyclic buffer

#define CBUFF_SIZE 10

int cbuff[CBUFF_SIZE];
int out=0, len=0;


void cbuff_push(int cli_nr)
{   if (len+1>CBUFF_SIZE){
    printf("OVERFLOW ");
    return;};
    cbuff[len] = cli_nr;
    len=len+1;
    out = 0;
}


void cbuff_pop(void)
 {if(len==0)
 {
     printf("UNDERFLOW ");
    return;
 }
{
    for(int i=0; i<len-1+1;++i)
{  if(cbuff[i+1] != 0)
{
    cbuff[i] = cbuff[i+1];
    }
   else
    {
   len-=1;
    return;
    }
   }
len -=1;
}

}

void cbuff_state(void)
{if (len == 0)
{   printf("EMPTY ");
    return;

}
    for(int i=0; i<len;++i)
{   if(cbuff[i]!=0){
    printf("%d", cbuff[i]);
    printf(" ");}
else
    {return;}

}}


int main(void) {
	int to_do, n, client_no, op_code;
	double x;
	scanf("%d",&to_do);
	switch(to_do) {
		case 1: // stack
			do {
				scanf("%lf",&x);
				if(x > 0) stack_push(x);
				else {
					if(x < 0) printf("%.2f ", stack_pop());
					else printf("\n%d\n", stack_state());
				}
			} while(x != 0);
            break;
		case 2: // FIFO queue with shifts
			do {
				scanf("%d",&n);
				if(n > 0) queue_push(n);
				else {
					if(n < 0) queue_pop(-n);
					else queue_state();
				}
			} while(n != 0);
			break;
		case 3: // queue with cyclic buffer
			client_no = 0;
			do {
				scanf("%d",&op_code);
				if(op_code > 0) cbuff_push(++client_no);
				else {
					if(op_code < 0) cbuff_pop();
					else cbuff_state();
				}
			} while(op_code != 0);
			break;
		default:
			printf("NOTHING TO DO!\n");
	}
	return 0;
}
