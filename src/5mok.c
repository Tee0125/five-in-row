/* VIM Settings {{{
 * Local variables:
 * tab-width: 4
 * vim600: sw=4 ts=4 cindent fdm=marker
 * }}} */
 
#include <string.h>
#include "5mok.h"

#ifdef DEBUG
	#include <stdio.h>
#endif

static int  STAT[ROW+4][COL+4];

#ifdef DEBUG
FILE* debug;

/* void omok_debug_print_stat() {{{ */
void omok_debug_print_stat(){
	int x, y;

	fprintf( debug, "    " );
	for ( x = 0 ; x < COL ; x++ )
		fprintf( debug, "%2d ", x );
	
	/* bottom-up */
	for( y = (ROW-1) ; y > -1 ; y-- ){
		
		fprintf( debug, "\n%2d: ", y );
		for ( x = 0 ; x < COL ; x++ ){
			if( stat[y][x] == BLACK )
				fprintf( debug, " X " );
			else if( stat[y][x] == WHITE )
				fprintf( debug, " O " );
			else
				fprintf( debug, "   " );
		}
		
	}
	fprintf( debug, "\n" );
	fflush( debug );

}
/* }}} */
#endif

/* int omok_ place( who, x, y ) {{{ */
int omok_place( int who, int x, int y ){

	COORD c;
	
	if( omok_game_end != 0 || stat[y][x] != BLANK )
		return -1;

	c.x = x;
	c.y = y;
		
	stat[y][x] = who;
	push(c);

	return 0;
		
}
/* }}} */

/* initialize the game */
/* void omok_init_stat() {{{ */
void omok_init_stat(){

	int i;
	
	memset( STAT, 0, (COL+4)*(ROW+4) );
	omok_game_end = 0;
	
	for( i = 0 ; i < ROW ; i++ )
		stat[i] = STAT[i+2] + 2;

#ifdef DEBUG
	debug = fopen( "debug.out", "w" );
	//debug = fopen( "/dev/null", "w" );
#endif

}
/* }}} */

/* int omok_is_promising( x, y ) {{{ */
int omok_is_promising( int x, int y ){

	int i,j;

	if( stat[y][x] != BLANK )
		return 0;

	for( i = -2 ; i < 3 ; i++ )
		for( j = -2 ; j < 3 ; j++ )
			if( (y+j) > 0 && (y+j) < ROW && 
			    stat[y+j][x+i] != BLANK )
				return 1;

	return 0;
	
}
/* }}} * /

/*
 * d:    direction 
 * dist: distance
 */
/* int omok_next_pos( x, y, d, dist ) {{{ */
int omok_next_pos( int x, int y, int d, int dist ){

	switch( d ){

		/* left->right */
		case HORIZONTAL:
			return stat[y][x+dist];

		/* up->down */
		case VERTICAL:
			return stat[y+dist][x];

		/* up,left -> down,right */
		case DIAGONAL1:
			return stat[y+dist][x+dist];

		/* up,right -> down, left */
		case DIAGONAL2:
			return stat[y+dist][x-dist];

	}

	/* don't use */
	return 0;

}
/* }}} */ 

#define omok_evaluation_c1( size, type, i, pleft, nleft, turn, h ) do { \
	/* not implemented yet */\
	/* 10001 10011 10111 */\
	/* 1011 1001 */\
	/* 101 10101 */\
	;\
} while(0)

#define omok_evaluation_c2( size, type, i, pleft, nleft, turn, h ) do { \
\
	int blocked = 2;\
	int flag = 0;\
\
	/* 1101 11001 */ \
	if( nleft > 1 && type[i+1] == BLANK ){\
\
		/* 1101 */ \
		if( size[i+1] == 1 && type[i+2] == type[i] && size[i+2] == 1 ){\
\
			if( nleft > 2 && type[i+3] == BLANK )\
				if( size[i+3] != 1 || nleft == 3 || type[i+4] != type[i] )\
					blocked--;\
\
			if( pleft > 0 && type[i-1] == BLANK )\
				if( size[i-1] != 1 || pleft == 1 || type[i-2] != type[i] )\
					blocked--;\
\
			if( blocked == 0 )\
				h += type[i] * HEURISTIC_3;\
			else if( blocked == 1 )\
				h += type[i] * HEURISTIC_3B;\
\
		}\
		/* 11001 */\
		else if( size[i+1] == 2 && type[i+2] == type[i] && size[i+2] == 1 )\
			h += type[i] * HEURISTIC_3B;\
\
		/* 11011 */\
		else if( size[i+1] == 1 && type[i+2] == type[i] && size[i+2] == 2 )\
			h += type[i] * HEURISTIC_4B;\
\
	}\
\
	/* 11 */\
	else {\
\
		if( nleft > 0 && type[i+1] == BLANK ){\
			blocked--;\
			flag = size[i+1];\
		}\
\
		if( pleft > 0 && type[i-1] == BLANK ){\
			blocked--;\
			flag += size[i-1];\
		}\
\
		/* 00110 01100 */ \
		if( blocked == 0 ){\
			/* 000110 001100 011000 */ \
			if( flag > 3 )\
				h += type[i] * HEURISTIC_2;\
			/* 00110 01100 */ \
			else if( flag == 3 ){\
				if( ( pleft == 1 || type[i-2] != type[i] ) &&\
				    ( nleft == 1 || type[i+2] != type[i] ) )\
						h += type[i] * HEURISTIC_2B;\
			}\
		}\
		/* 11000 00011 */ \
		else {\
			/* 110000 000011 */ \
			if( flag > 3 )\
				h += type[i] * HEURISTIC_2B;\
			/* 11000 00011 */ \
			else if( flag == 3 ){\
				/* x00011 */ \
				if( pleft > 0 && ( pleft == 1 || type[i-2] != type[i] ) )\
						h += type[i] * HEURISTIC_2B;\
				/* 11000x */ \
				else if( nleft > 0 && ( nleft == 1 || type[i+2] != type[i] ) )\
						h += type[i] * HEURISTIC_2B;\
			}\
\
		}\
\
	}\
\
} while(0)\

#define omok_evaluation_c3( size, type, i, pleft, nleft, turn, h ) do { \
\
	int blocked = 2; \
	int flag = 0; \
\
	/* 11101 */ \
	if( nleft > 1 && type[i+1] == BLANK ){ \
\
		if( size[i+1] == 1 && type[i+2] == type[i] && size[i+2] == 1 ){ \
				h += HEURISTIC_4B * type[i]; \
				break; \
		} \
\
	} \
\
	/* right opened */ \
	if( nleft > 0 && type[i+1] == BLANK ){ \
		blocked--; \
		flag = size[i+1]; \
	} \
\
	/* left opened */ \
	if( pleft > 0 && type[i-1] == BLANK ){ \
		blocked--; \
		flag += size[i-1]; \
	} \
\
	/* 11100 01110 00111 */ \
	if( flag > 2 ){ \
		if( blocked == 0 ) \
				h += HEURISTIC_3 * type[i]; \
		else \
				h += HEURISTIC_3B * type[i]; \
	} \
	else if( flag == 2 ) \
			h += HEURISTIC_3B * type[i]; \
\
} while(0)

#define omok_evaluation_c4( size, type, i, pleft, nleft, turn, h ) do { \
	int blocked = 2; \
\
	if( pleft > 0 && type[i-1] == BLANK ) \
		blocked--; \
	if( nleft > 0 && type[i+1] == BLANK ) \
		blocked--; \
\
	if( blocked == 0 ) \
		h += HEURISTIC_4 * type[i]; \
	else if( blocked == 1 ) \
		h += HEURISTIC_4B * type[i];  \
\
} while(0)

#define omok_evaluation_c5( size, type, i, pleft, nleft, turn, h ) do { \
	h += HEURISTIC_WIN * type[i]; \
} while(0)

/* int omok_evaluation_line( x, y, msize, d, turn ) {{{ */
int omok_evaluation_line( int x, int y, int msize, int d, int turn ){

	int prev, cur, i;

	int size[COL];
	int type[COL];

	int count = 0;
	int h = 0;

	int pleft, nleft;

	size[0] = 1;
	prev = type[0] = omok_next_pos( x, y, d, 0 );

	for( i = 1 ; i < msize ; i++ ){
		
		cur = omok_next_pos( x, y, d, i);

		if( cur != prev ){
			prev = cur;
			count++;

			type[count] = cur;
			size[count] = 1;
		}
		else
			size[count]++;

	}

	/* caculate heuristics */
	for( i = 0 ; i <= count ; i++ ){
	
		if( type[i] == BLANK )
			continue;

		pleft = i;
		nleft = count-i;
		
		switch( size[i] ){
			case 1:
				omok_evaluation_c1( size, type, i, pleft, nleft, turn, h );
				break;
			case 2:
				omok_evaluation_c2( size, type, i, pleft, nleft, turn, h );
				break;
			case 3:
				omok_evaluation_c3( size, type, i, pleft, nleft, turn, h );
				break;
			case 4:
				omok_evaluation_c4( size, type, i, pleft, nleft, turn, h );
				break;
			case 5:
				omok_evaluation_c5( size, type, i, pleft, nleft, turn, h );
				break;
		}
			
	}
	return h;

}
/* }}} */

/* omok_evaluation {{{ */
int omok_evaluation( int turn ){

	int i, heuristics = -1 * turn;

	// check horizontal
	for ( i = 0 ; i < ROW ; i++ )
		heuristics += omok_evaluation_line( 0, i, COL, HORIZONTAL, turn );

	// check vertical
	for ( i = 0 ; i < COL ; i++ )
		heuristics += omok_evaluation_line( i, 0, ROW, VERTICAL, turn );

	// check diagonal
	for ( i = 0 ; i < COL-4 ; i++ ){
		heuristics += omok_evaluation_line( i, 0, COL-i, DIAGONAL1, turn );
		heuristics += omok_evaluation_line( COL-1-i, 0, COL-i, DIAGONAL2, turn);
	}	
	for ( i = 1 ; i < ROW-4 ; i++ ){
		heuristics += omok_evaluation_line( 0, i, ROW-i, DIAGONAL1, turn );
		heuristics += omok_evaluation_line( COL-1, i, ROW-i, DIAGONAL2, turn );
	}
	return heuristics;
	
}
/* }}} */

int omok_five_in_row( int x, int y, int size, int d ){

	int prev  = BLANK, cur;
	int count = 0;

	int i;

	for( i = 0 ; i < size ; i++ ){

		cur = omok_next_pos( x, y, d, i );

		/* continuous allocation */
		if( cur == prev )
			count++;

		else if ( prev != BLANK && count == 5 )
			return prev;
			
		else {
			prev = cur;
			count = 1;
		}

	}
	if ( prev != BLANK && count == 5 )
		return prev;

	return 0;
}

/* check some wins? */
/* int omok_check_win() {{{ */
int omok_check_win(){

	int win;
	int i;

	// check horizontal
	for ( i = 0 ; i < ROW ; i++ )
		if( win = omok_five_in_row(0, i, COL, HORIZONTAL) )
			return win;

	// check vertical
	for ( i = 0 ; i < COL ; i++ )
		if( win = omok_five_in_row(i, 0, ROW, VERTICAL) )
			return win;

	// check diagonal
	for ( i = 0 ; i < COL-4 ; i++ ){
		if( win = omok_five_in_row(i, 0, COL-i, DIAGONAL1) )
			return win;

		if( win = omok_five_in_row(COL-1-i, 0, COL-i, DIAGONAL2) )
			return win;
	}	
	for ( i = 1 ; i < ROW-4 ; i++ ){
		if( win = omok_five_in_row(0, i, ROW-i, DIAGONAL1) )
			return win;

		if( win = omok_five_in_row(COL-1, i, ROW-i, DIAGONAL2) )
			return win;
	}

	return 0;
	
}
/* }}} */

/* 
 * alpha beta pruning 
 *
 * pseudo code from en.wikipedia...
 *
 * current: 
 *   human   ->maxnode;
 *   computer->minnode;
 *
 * next version:
 *   black   -> maxnode;
 *   white   -> minnode;
 */
int ab_prun( int turn, int alpha, int beta, int level ){

	int x, y;

	int next;
	int ret;
	
	/* node is leaf: return the heuristic */
	if ( level == 0 ){
		ret = omok_evaluation( turn );

#ifdef DEBUG
		if( ret != 0 ){
			fprintf( debug, "** DEBUG: leaf... %d **\n", ret );
			omok_debug_print_stat();
		}
#endif
		
		return ret;
	}

	/* node is not leaf */
	level--;
	next = -1 * turn;

	/* for each node */
	for ( y = 0 ; y < ROW ; y++ ){

		for ( x = 0 ; x < COL ; x++ ){
		
			if( omok_is_promising( x, y ) != 1 )
				continue;
				
			stat[y][x] = turn;
			if( ret = omok_check_win() ){

				stat[y][x] = BLANK;
				return ret * HEURISTIC_WIN;

			}

#ifdef DEBUG1
			fprintf( debug, "** level: %d **\n", level );
			fprintf( debug, "** turn: %2d check (%d,%d) **\n", next, x, y );
#endif
			
			ret = ab_prun( next, alpha, beta, level );
			
#ifdef DEBUG1
			if( turn == BLACK )
				fprintf( debug, "*** BLACK turn ***\n" );
			else 
				fprintf( debug, "*** white turn ***\n" );
			
			fprintf( debug, "*** DEBUG: current spot is (%d, %d) ***\n", x, y );
			fprintf( debug, "*** DEBUG: return value is %d ***\n", ret );
			
			//omok_debug_print_stat();
#endif
			
			stat[y][x] = BLANK;
				
			/* if node is a maximizing node */
			if( turn == BLACK ){
			
				if( ret == HEURISTIC_WIN )
					return ret;

				if( ret > alpha )
					alpha = ret;

#ifdef DEBUG1
				fprintf( debug, "*** DEBUG: in maximizing node ***\n" );
				fprintf( debug, "*** DEBUG: alpha = %d ***\n", alpha );
				fprintf( debug, "*** DEBUG: beta = %d ***\n", beta );
				fprintf( debug, "*** DEBUG: return value will be beta ***\n" );
#endif

				if( beta <= alpha )
					return beta;

			}
			/* if node is a minimizing node */
			else {
					
				if( ret == (-1 * HEURISTIC_WIN) )
					return ret;
		
				if( ret < beta )
					beta = ret;
					
#ifdef DEBUG1
				fprintf( debug, "*** DEBUG: in minimizing node ***\n" );
				fprintf( debug, "*** DEBUG: alpha = %d ***\n", alpha );
				fprintf( debug, "*** DEBUG: beta = %d ***\n", beta );
				fprintf( debug, "*** DEBUG: return value will be alpha ***\n" );
#endif

			 	if( beta <= alpha )
					return alpha;

			}
#ifdef DEBUG1
			fprintf( debug, "\n" );
#endif
			
		}

	}

	if( turn == BLACK )
		return alpha;
	else
		return beta;
	
}

/* do alpha beta pruning */
COORD omok_ai_turn(){

	int x, y;
	int ret;

	int alpha = -1000000;
	int beta = 1000000;

	/* if there exist no stones... then put on 5,5 */
	COORD c = { 5, 5 };

	for ( y = 0 ; y < ROW ; y++ ){

		for ( x = 0 ; x < COL ; x++ ){

			if( omok_is_promising( x, y ) != 1 )
				continue;

			stat[y][x] = computer;
			if( omok_check_win() ){

				c.x = x;
				c.y = y;

				stat[y][x] = BLANK;
				
				return c;

			}

			ret = ab_prun( human, alpha, beta, PRUNING_LEVEL-1 );
			
#ifdef DEBUG1
			printf( "**** DEBUG: current spot is (%d, %d) ****\n", x, y );
			printf( "**** DEBUG: return value is %d ****\n", ret );
			// omok_debug_print_stat();
#endif
			
			stat[y][x] = BLANK;

			/* if node is a maximizing node */
			if( computer == BLACK ){
			
#ifdef DEBUG1
				fprintf( debug, "**** DEBUG: in maximizing node ****\n" );
				fprintf( debug, "**** DEBUG: alpha = %d ****\n", alpha );
				fprintf( debug, "**** DEBUG: return value will be beta ****\n");
#endif

				if( ret > alpha ){
					
#ifdef DEBUG1
					fprintf( debug, "**** DEBUG: choose here ****\n" );
#endif
					
					alpha = ret;
					
					c.x = x;
					c.y = y;

				}

			}
			/* if node is a minimizing node */
			else {
					
#ifdef DEBUG1
				fprintf( debug, "*** DEBUG: in minimizing node ***\n" );
				fprintf( debug, "*** DEBUG: beta = %d ***\n", beta );
#endif

				if( ret < beta ){
					
#ifdef DEBUG1
					fprintf( debug, "**** DEBUG: choose here ****\n" );
#endif
					
					beta = ret;
					
					c.x = x;
					c.y = y;

				}
					
			}
#ifdef DEBUG1
			fprintf( debug, "\n" );
#endif
			
		}
		
	}

	if( c.x == 5 && c.y == 5 && stat[5][5] != BLANK )
		for( c.y = 0 ; c.y < ROW ; c.y++ )
			for( c.x = 0 ; c.x < COL ; c.x++ )
				if( stat[c.y][c.x] == BLANK )
					return c;
	
	return c;
	
}

