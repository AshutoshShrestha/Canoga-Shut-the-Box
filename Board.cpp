#include "Board.h"
#include <iostream>
#include <vector>
#include <string>

// default constructor
Board::Board() {
	// default value 
	squares = 9;
	square_status = new int[9];
	for (int i = 0; i < 9; i++) {
		// initializing all squares to 0 (which means uncovered)
		square_status[i] = 0;
	}
}

// parameterized constructor
Board::Board(int n) {
	squares = n;
	square_status = new int[n];
	for (int i = 0; i < n; i++) {
		// initializing all squares to 0 (which means uncovered)
		square_status[i] = 0;
	}
}

// parameterized constructor
Board::Board(int sq, int * player_board) {
	squares = sq;
	square_status = player_board;
}

// destructor
Board::~Board() {
	delete[] square_status;
}

/* *********************************************************************
Function Name: cover_square
Purpose: To cover an uncovered square
Parameters:
			sq, an integer passed by value. It holds the number of
			squares in a row the player chose to play with.
Return Value: none (void)
Algorithm:
			1) Set the value from 0 to 1 in a given board square position.
Assistance Received: none
********************************************************************* */
bool Board::cover_square(int sq) {
	if ((sq > 0 || sq <= squares) && (square_status[sq - 1] != 1)) {
		square_status[sq - 1] = 1;
		return true;
	}
	return false;
}

/* *********************************************************************
Function Name: uncover_square
Purpose: To uncover a covered square
Parameters:
			sq, an integer passed by value. It holds the number of
			squares in a row the player chose to play with.
Return Value: none (void)
Algorithm:
			1) Set the value from 1 to 0 in a given board square position.
Assistance Received: none
********************************************************************* */
bool Board::uncover_square(int sq) {
	if (sq > 0 || sq <= squares && (square_status[sq - 1] != 0)) {
		square_status[sq - 1] = 0;
		return true;
	}
	return false;
}

/* *********************************************************************
Function Name: display
Purpose: To display a row containing a player's squares
Parameters: none
Return Value: none (void)
Algorithm:
			1) Iterate through a player's board.
			2) If 1 is stored, print *, if 0 is stored print a space.
********************************************************************* */
void Board::display() {
	for (int i = 0; i < squares; i++) {
		if (square_status[i] == 1) {
			std::cout << "*  ";
		}
		else {
			std::cout << i + 1 << "  ";
		}
	}
}

/* *********************************************************************
Function Name: get_uncovered_squares
Purpose: To return a player's uncovered squares.
Parameters: none
Return Value: A vector of integers that holds a player's uncovered squares.
Algorithm:
			1) Iterate through a player's board
			2) If 0 is stored in a position, then push it into the vector
			to be returned.
Assistance Received: none
********************************************************************* */
std::vector<int> Board::get_uncovered_squares() const {
	std::vector<int> uncovered_sq;
	for (int i = 0; i < squares; i++) {
		if (square_status[i] == 0) {
			uncovered_sq.push_back(i + 1);
		}
	}
	return uncovered_sq;
}

/* *********************************************************************
Function Name: get_covered_squares
Purpose: To return a player's uncovered squares.
Parameters: none
Return Value: A vector of integers that holds a player's covered squares.
Algorithm:
			1) Iterate through a player's board
			2) If 1 is stored in a position, then push it into the vector
			to be returned.
Assistance Received: none
********************************************************************* */
std::vector<int> Board::get_covered_squares() const {
	std::vector<int> covered_sq;
	for (int i = 0; i < squares; i++) {
		if (square_status[i] == 1) {
			covered_sq.push_back(i + 1);
		}
	}
	return covered_sq;
}