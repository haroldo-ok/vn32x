/*
 *
 *     Eight Queens Problem (Sega 32X)
 *
 *           by Sebastian Mihai, 2015
 *           http://sebastianmihai.com
 *
 *     This program outputs solutions to the Eight Queens Problem,
 *     which requires eight chess queens to be placed on a chess
 *     board such that no two queens threaten each other.
 *
 *     The code quality here is not very high, as I only allocated one day
 *     to write this.
 *
 */
 
#include <stdlib.h>
#include "32x.h"

/* Create a 5:5:5 RGB color with the MSB set */
#define COLOR(r,g,b)    (((r)&0x1F)|((g)&0x1F)<<5|((b)&0x1F)<<10|0x8000)
#define abs(x) ((x)<0 ? -(x) : (x))

#define BLACK 0x0000
#define WHITE 0x0101
#define RED 0x0202
#define BLUE 0x0303
#define GREEN 0x0404

#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 160

#define NUM_QUEENS 8
#define LAST_QUEEN NUM_QUEENS - 1
#define LAST_COLUMN NUM_QUEENS - 1

#define DELAY_ON_SOLUTION 200000

typedef struct {
	int row;
	int col;
} Queen;

Queen g_queens[NUM_QUEENS];
int g_currentQueen;
int g_currentConfigurationIsSolution;

void init() {
	int i;
	vu16 *palette = &MARS_CRAM;
	
	palette[0] = COLOR( 0, 0, 0 ); // black
	palette[1] = COLOR( 31, 31, 31 ); // white
	palette[2] = COLOR( 31, 0, 0 ); // red
	palette[3] = COLOR( 0, 0, 31 ); // blue
	palette[4] = COLOR( 0, 18, 0 ); // green
	
	for( i = 0; i < NUM_QUEENS; i++ ) {
		g_queens[i].row = i;
		g_queens[i].col = 0;
	}
	
	g_currentQueen = 0;
	g_currentConfigurationIsSolution = 0;
}

void drawSquare( int row, int col, int colour, int inside ) {
	const int squareSide = 24;
	const int verticalPadding = 18;
	const int horizontalPadding = 66;
	
	int i,j;
	int pixelOffset;
	vu16* frameBuffer16 = &MARS_FRAMEBUFFER;
	
	int startLine = verticalPadding + squareSide * row + inside;
	int endLine = startLine + squareSide / 2 - inside;
	
	int startColumn = horizontalPadding / 2 + ( squareSide / 2 ) * col + inside / 2;
	int endColumn = startColumn + squareSide - inside / 2 - inside;
	
	pixelOffset = 0x100 + SCREEN_HEIGHT*startLine + startColumn;
	
	for ( i = startColumn; i < endColumn; i++ ) {		
		for ( j = startLine; j < endLine; j++ ) {
			frameBuffer16[pixelOffset++] = colour;
		}
		pixelOffset = pixelOffset + SCREEN_WIDTH - 76 + inside;
	}
}

void clearScreen( int colour ) {
	int i, j;
	int pixelOffset, lineOffset;
	vu16* frameBuffer16 = &MARS_FRAMEBUFFER;
	
	// clear screen
	pixelOffset = 0x100;
	lineOffset = 0;
	for ( i = 0; i < SCREEN_WIDTH; i++ ) {
		frameBuffer16[lineOffset++] = pixelOffset;
		for ( j = 0; j < SCREEN_HEIGHT; j++ ) {
			frameBuffer16[pixelOffset++] = colour;
		}
	}
}

void displayBoard() {
	int i, j, colour;
	
	for( i = 0; i < NUM_QUEENS; i++ ) {
		for( j = 0; j < NUM_QUEENS; j++ ) {
			colour = ( i + j ) % 2 == 0 ? WHITE : BLACK;
			drawSquare( i, j, colour, 0 );
		}
	}
	
	for( i = 0; i < NUM_QUEENS; i++ ) {
		drawSquare( 
			g_queens[i].row, 
			g_queens[i].col, 
			RED, 
			4 
			);
	}
}

void swapBuffers() {
	static uint16 currentBuffer = 0;
	
	// swap the framebuffer selection bit and wait for it to take effect
	MARS_VDP_FBCTL = currentBuffer ^ 1;
	while ((MARS_VDP_FBCTL & MARS_VDP_FS) == currentBuffer) {}
	currentBuffer ^= 1;
}

void delay() {
	int i;
	for( i = 0; i < DELAY_ON_SOLUTION; i++ ) {
		swapBuffers();
	}
}

// are all queens up to current one valid with respect to current one?
int isValidUpToCurrent() {
	int i;
	int rowDifference, colDifference;
		
	for( i = 0; i < g_currentQueen; i++ ) {
		if( g_queens[i].col == g_queens[g_currentQueen].col ) {
			return 0;
		}
		
		rowDifference = g_queens[i].row - g_queens[g_currentQueen].row;
		colDifference = g_queens[i].col - g_queens[g_currentQueen].col;
		
		// check diagonals
		if( abs( rowDifference ) == abs( colDifference ) ) {
			return 0;
		}
	}
	
	return 1;
}

// move to next column, backtracking to prior queens if necessary
void nextColumn() {
	int overflow = 1;

	while( overflow ) {		
		overflow = 0;
		g_queens[g_currentQueen].col++;
		
		// are we past the end?
		if( g_queens[g_currentQueen].col > LAST_COLUMN ) {
			g_queens[g_currentQueen].col = 0;
			
			g_currentQueen--;
			
			if( g_currentQueen < 0 ) {
				g_currentQueen = 0;
				return;
			}
			
			overflow = 1;
		}
	}
}

void nextConfiguration() { 
	
	// if we've already handled this solution or is invalid, try the next column
	if( g_currentConfigurationIsSolution || !isValidUpToCurrent() ) {
		g_currentConfigurationIsSolution = 0;
		nextColumn();
		return;
	}
	
	// all valid up to and including current queen
	
	if( g_currentQueen == LAST_QUEEN ) {
		g_currentConfigurationIsSolution = 1;
		return;
	}
	
	// next queen
	g_currentQueen++;
}

void displayAll( int backgroundColour ) {
	clearScreen( backgroundColour );
	displayBoard();

	vu16* frameBuffer = &MARS_FRAMEBUFFER + 0x100;
	frameBuffer[0] = RED;
	frameBuffer[160] = RED;
	frameBuffer[319] = 0x0404;
	/*
	frameBuffer[1] = RED;
	frameBuffer[2] = RED;
	frameBuffer[3] = RED;
	*/
	
	swapBuffers();
}

int main() {
	// Wait for the SH2 to gain access to the VDP
	while ((MARS_SYS_INTMSK & MARS_SH2_ACCESS_VDP) == 0) {}
	// Set 8-bit paletted mode, 224 lines
	MARS_VDP_DISPMODE = MARS_224_LINES | MARS_VDP_MODE_256;
	MARS_VDP_FBCTL = 0;	

	init();
	
    while( 1 ) {
		nextConfiguration();

		if( g_currentConfigurationIsSolution ) {
			displayAll( GREEN );
			displayAll( GREEN );
			delay();
		} else {
			displayAll( BLUE );
		}
	}
	
	return 0;
}
