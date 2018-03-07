#define BUFSIZE 255

#define ROW 11
#define COL 11

#define BLACK 1
#define WHITE -1
#define BLANK 0

#define PRUNING_LEVEL 3

#define HEURISTIC_WIN 1000000
#define HEURISTIC_4   100000
#define HEURISTIC_4B  10000
#define HEURISTIC_3   1000
#define HEURISTIC_3B  100
#define HEURISTIC_2   10
#define HEURISTIC_2B  1

#define HORIZONTAL    1
#define VERTICAL      2
#define DIAGONAL1     3
#define DIAGONAL2     4

typedef struct _COORD {
	int x;
	int y;
} COORD;

/* some stack stuff */
COORD pop();
void  push( COORD var );
void  show_stack();

/* initialize stat */
void omok_init_stat();

/* is there exist five in a row? */
int omok_check_win();

/* insert the stone */
int omok_place( int who, int x, int y );

/* use heuristic for computer turn */
COORD omok_ai_turn();

/* some global values */
int human;
int computer;

int omok_game_end;

int* stat[ROW];
