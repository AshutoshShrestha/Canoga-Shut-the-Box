#pragma once

#include <vector>

class Board {
public:
	// default constructor 
	Board();

	// parameterized constructor
	Board(int);
	Board(int, int *);

	// destructor
	~Board();

	// mutators
	bool cover_square(int);
	bool uncover_square(int);
	inline void set_square_states(int * board_state) { square_status = board_state; }

	// selector
	std::vector<int> get_uncovered_squares() const;
	std::vector<int> get_covered_squares() const;
	inline int * get_square_states() { return square_status; }

	void display();

private:
	// total number of squares in a player's row
	int squares;

	// int pointer to keep track of dynamic array that stores the state of a square
	// which denotes whether a square is covered or not
	// 1 if covered
	// 0 if uncovered
	int* square_status;
};