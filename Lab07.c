/***************************************************************************
 * Lab07.c
 *
 *   Created:10/13/2026
 *    Author: Jack Robinson
 * Processor: ATmega128A (on a ReadyAVR)
 *
 * Runs a 1-D cellular automaton (Wolfram Rule 26) for 122 generations and
 * sends each generation out the UART to be shown in PuTTY.
 *
 * The array has 61 cells: '*' is black and ' ' is white. It starts with one
 * black cell in the middle. The first and last cells always stay white.
 * The array is made in main() and the subroutines use it through a pointer.
 *
 **************************************************************************/

/* your #include file declaration(s) go here */
#include "UARTLibrary.h"
#include <string.h>

/* your #define declarations go here */
#define BaudRate 9600       // UART settings (match PuTTY)
#define DataBits 8
#define StopBits 1
#define NoParity 0

#define arraySize   61      // number of cells
#define middleCell  30      // starting black cell
#define generations 122     // generations to display
#define black       '*'
#define white       ' '

// prototype subroutine definitions
void initializeArray(unsigned char *cp);
void displayArray(unsigned char *cp);
void updateArray(unsigned char *cp);

int main(void)
{
	// create a 61-char array
	unsigned char cells[arraySize];

	// Initialize the UART
	// ProfNote:  Understand the UARTLibary.h before blindly running the next line!
	uart_init(BaudRate, DataBits, StopBits, NoParity);

	// set the starting pattern
	initializeArray(cells);

	// show each generation, then make the next one
	for (int gen = 0; gen < generations; gen++){
		displayArray(cells);
		updateArray(cells);
	}

	// done, stop here
	while(1);

}

/* your subroutines go here */

// Make every cell white, then make the middle cell black.
void initializeArray(unsigned char *cp){
	for (int i = 0; i < arraySize; i++)
	{
		*(cp + i) = white;   // make every cell a space
	}
	*(cp + middleCell) = black;   // put the star in the middle

}

// Send the array out the UART, then start a new line.
void displayArray(unsigned char *cp){
	for(int i = 0; i < arraySize; i++){
		uart_tx(*(cp + i));   // send one cell
	}
	uart_tx('\r');   // back to start of line
	uart_tx('\n');   // down one line
}

// Apply Rule 26 to make the next generation.
// New cells go in a temporary array so the current one isn't changed
// while it is still being read.
void updateArray(unsigned char *cp){
	unsigned char next[arraySize];

	// first and last cells always stay white
	next[0] = white;
	next[arraySize - 1] = white;

	for(int i = 1; i < arraySize - 1; i++){
		// current cell and its neighbors
		unsigned char left = *(cp + i -1);
		unsigned char center = *(cp + i);
		unsigned char right = *(cp + i +1);

		// only these three patterns become black
		if (left == black && center == white && right == white){
			next[i] = black;  // rule 4
		}
		else if (left == white && center == black && right == black){
			next[i] = black;  // rule 5
		}
		else if (left == white && center == white && right == black){
			next[i] = black;  // rule 7
		}
		else{
			next[i] = white;  // everything else is white
		}
	}
	// copy the new generation back into the original array
	memcpy(cp, next, arraySize);
}