#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "../5mok.h"

/* 
 * ui code
 */
void print_stat(){

	int x, y;

	fprintf( stdout, "    " );
	for ( x = 0 ; x < COL ; x++ )
		fprintf( stdout, "%2d ", x );
	
	/* bottom-up */
	for( y = (ROW-1) ; y > -1 ; y-- ){
		
		fprintf( stdout, "\n%2d: ", y );
		for ( x = 0 ; x < COL ; x++ ){
			if( stat[y][x] == BLACK )
				fprintf( stdout, " X " );
			else if( stat[y][x] == WHITE )
				fprintf( stdout, " O " );
			else
				fprintf( stdout, "   " );
		}
		
	}
	fprintf( stdout, "\n" );
	
}

int main( int argc, char** argv ){

	char buf[BUFSIZE+1];
	COORD c;

	omok_init_stat();
	
	if( argc > 1 ){
		human = WHITE;
		computer = BLACK;
		fprintf( stdout, "Computer First...\n" );
		fprintf( stdout, "Computer Select (5,5)\n" );

		omok_place( BLACK, 5, 5 );
	}
	else {
		human = BLACK;
		computer = WHITE;
		fprintf( stdout, "Human First...\n" );
	}

	do {
	
		fputc( '>',  stdout );
		fputc( ' ',  stdout );
		fgets( buf, BUFSIZE, stdin );

		if( strncmp( "stats", buf, 5 ) == 0 ){
			fprintf( stdout, "show stats...\n" );
			print_stat();
		}
		else if( strncmp( "stack", buf, 5 ) == 0 ||
				 strncmp( "bt", buf, 2 ) == 0 ){
			fprintf( stdout, "show stack stats...\n" );
			show_stack();
		}
		else if( strncmp( "back", buf, 4 ) == 0 ){
			fprintf( stdout, "remove 2 from stack...\n" );
			c = pop();
			if(c.x < 0 )
				continue;
			
			stat[c.y][c.x] = BLANK;
			fprintf( stdout, "remove (%d,%d) from stat...\n", c.x, c.y );

			c = pop();
			stat[c.y][c.x] = BLANK;
			fprintf( stdout, "remove (%d,%d) from stat...\n", c.x, c.y );

			omok_game_end = 0;

		}
		else if( strncmp( "help", buf, 4 ) == 0 ){
			fprintf( stdout, "help commands\n\n" );
			fprintf( stdout, "x,y: put the stone to the coordinate\n\n" );
			fprintf( stdout, "stats: draw the stat\n" );
			fprintf( stdout, "stack or bt: show the stack\n" );
			fprintf( stdout, "back: remove last two stones..\n" );
			fprintf( stdout, "quit: finish the game..\n\n" );
		}
		else if( strncmp( "quit", buf, 4 ) == 0 ){
			fprintf( stdout, "quit...\n" );
			break;
		}
		else {

			if( sscanf( buf, " %d, %d", &c.x, &c.y ) == 2 ){

				if( omok_game_end != 0 ){
					fprintf( stdout, "Game already ends...\n" );
					continue;
				}

				fprintf( stdout, "you select coord( %d, %d )\n", c.x, c.y );
				if( omok_place( human, c.x, c.y ) < 0 ){
					fprintf( stdout, "you can't put on (%d,%d)\n", c.x, c.y );
					continue;
				}
				else if ( omok_check_win() ){	
					fprintf( stdout, "Human Win!!\n" );
					omok_game_end = 1;

					continue;
				}
				print_stat();

				c = omok_ai_turn();
				
				fprintf( stdout, "computer select coord( %d, %d )\n", c.x, c.y);
				omok_place( computer, c.x, c.y );
				
				print_stat();

				if ( omok_check_win() ){	
					fprintf( stdout, "Computer Win!!\n" );
					omok_game_end = 1;

					continue;
				}
								
			}
			else 
				fprintf( stdout, "Unknown Input...\n" );

		}

	} while (1);

	return 0;

}
