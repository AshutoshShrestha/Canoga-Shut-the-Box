#pragma once

#include "Board.h"
#include <string>
#include <vector>

class Player {
public:
	// default constructor 
	Player();

	// destructor
	~Player();

	std::vector<int> roll_dice(bool);

	// mutators
	bool set_score(int);
	bool set_previous_round_score(int);
	bool set_had_first_turn(bool);
	bool set_advantage_square(int);

	// inline mutators
	inline bool cover(int square) { return player_board->cover_square(square) ? true : false; }
	inline bool uncover(int square) { return player_board->uncover_square(square) ? true : false; }

	// inline selectors
	inline int get_score() const { return score; }
	inline int get_previous_round_score() const { return previous_round_score; }
	inline int get_advantage_square() const { return advantage_square; }
	inline std::vector<int> get_uncovered_squares() const { return player_board->get_uncovered_squares(); }
	inline std::vector<int> get_covered_squares() const { return player_board->get_covered_squares(); }
	inline bool had_first_turn() const { return first_turn; }
	// isn't const
	inline int * get_board_state() { return player_board->get_square_states(); }

	// virtual functions
	virtual std::vector<int> choose_a_move(int, int, bool) { return std::vector<int>(); }
	virtual std::string get_name() const { return "BaseClass"; }
	virtual bool roll_one() { return false; }

	// inline functions
	inline void initialize_board(int squares) { player_board = new Board(squares); }
	// required to save and restore board values
	inline void initialize_board(int squares, int * state) { player_board = new Board(squares, state); }
	inline void clear_board() { delete player_board; }
	inline void show_board() { player_board->display(); }

private:
	int score;
	Board* player_board;
	bool first_turn;
	int previous_round_score;
	int advantage_square = 0;
};