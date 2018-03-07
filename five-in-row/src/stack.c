#include <stdio.h>
#include "5mok.h"

#define STACK_SIZE ROW*COL

static int top = 0;
static COORD stack[STACK_SIZE];

void show_stack(){

	int i;
	for( i = 0 ; i < top ; i++ ){

		if( (i % 2) == 0 )
			fprintf( stderr, "black: " );
		else
			fprintf( stderr, "white: " );
		
		fprintf( stderr, "(%d,%d)\n", stack[i].x, stack[i].y );
		
	}
	fprintf( stderr, "\n" );
}

COORD pop(){

	COORD ret;
	
	if( top > 0 ){
		
		top--;

		ret.x = stack[top].x;
		ret.y = stack[top].y;
		
	}
	else{
		
		ret.x = -1;
		ret.y = -1;

	}
	return ret;
	
}

void push( COORD var ){

	if( top < STACK_SIZE ){

		stack[top].x = var.x;
		stack[top].y = var.y;

		top++;

	}
	else
		fprintf( stderr, "Error: Stack is full\n" );
	
}

#ifdef STACK_TEST
int main ( ){

	COORD c;
	
	c.x = 1;
	c.y = 3;
	push(c);

	c.x = 1;
	c.y = 4;
	push(c);

	c.x = 2;
	c.y = 5;
	push(c);

	show_stack();

	pop();
	show_stack();
	
	c.x = 2;
	c.y = 5;
	push(c);
	show_stack();

	pop();
	show_stack();
	
	pop();
	show_stack();
	
	
	return 0;

}
#endif
