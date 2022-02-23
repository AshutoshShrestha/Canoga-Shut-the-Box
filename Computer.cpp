#include "Computer.h"
#include <iostream>

// calling default constructor will crash the program since opponent is not initialized
Computer::Computer() {
	helper = false;
	if (!set_previous_round_score(0) || !set_previous_round_score(0)) {
		cerr << "Error while setting previous round score." << endl;
		exit(1);
	}
}

// parameterized constructor
Computer::Computer(Player * opp_player) {
	helper = false;
	opponent = opp_player;
	if (!set_previous_round_score(0) || !set_previous_round_score(0)) {
		cerr << "Error while setting previous round score." << endl;
		exit(1);
	}
}

// parameterized constructor
Computer::Computer(bool is_helper, Player *opp_player) {
	helper = true;
	opponent = opp_player;
	if (!set_previous_round_score(0) || !set_previous_round_score(0)) {
		cerr << "Error while setting previous round score." << endl;
		exit(1);
	}
}

// destuctor
Computer::~Computer() {}

/* *********************************************************************
Function Name: choose_a_move
Purpose: To find and return the best move from all available moves.
Parameters:
			value, an integer passed by value. It holds the sum of dice values
			rolled by the player.
			moving_possibility, an integer passed by value. It holds integers
			ranging from 1-3 which denotes whether the player has the option to
			cover or uncover or do either.
			opponent_is_computer, a boolean variable pass by value. It holds
			true if the opponent is computer (literally) and vice versa.
Return Value: A vector of integers that holds the combination of integers
			  that a player wants to cover or uncover.
Algorithm:
			1) Get all possible moves from a rolled dice value.
			2) Get all available cover and uncover moves from all possible moves.
			3) If the player can do either of both, check if it is better to
			cover or uncover, and find then the best move.
			4) If the player can only do one of both, find the best move.
			5) Return the best move as a player's moves.
Assistance Received: none
********************************************************************* */
vector<int> Computer::choose_a_move(int value, int moving_possibility, bool opponent_is_computer) {
	GameUtils game_utils;
	vector<int> *player_moves = new vector<int>;
	vector<vector<int>*> all_moves = game_utils.all_possible_moves(value);
	vector<vector<int>*> available_cover_moves;
	vector<vector<int>*> available_uncover_moves;

	available_cover_moves = game_utils.all_available_cover_moves(all_moves, get_uncovered_squares());
	available_uncover_moves = game_utils.all_available_uncover_moves(all_moves, opponent->get_covered_squares(), opponent->get_advantage_square());

	if (moving_possibility == 3) {
		// find best possible move from available cover and available uncover moves
		available_cover_moves = game_utils.all_available_cover_moves(all_moves, get_uncovered_squares());
		available_uncover_moves = game_utils.all_available_uncover_moves(all_moves, opponent->get_covered_squares(), opponent->get_advantage_square());
		if (cover_or_uncover()) {
			player_moves = find_best_move(available_cover_moves, player_moves, 1);
		}
		else {
			player_moves = find_best_move(available_uncover_moves, player_moves, 2);
		}
	}
	else if (moving_possibility == 1) {
		// find best possible move from available cover moves
		cout << "Because there are no available moves to uncover, ";
		player_moves = find_best_move(available_cover_moves, player_moves, 1);
	}
	else {
		// find best possible move from available uncover moves
		cout << "Because there are no available moves to cover, ";
		player_moves = find_best_move(available_uncover_moves, player_moves, 2);
	}

	// returning by dereferencing player_moves and releasing the pointer memory
	vector<int> best_moves = *player_moves;
	for (auto &it : all_moves) {
		it->clear();
		delete it;
	}
	return best_moves;
}

/* *********************************************************************
Function Name: find_best_move
Purpose: To find and return the best move from all available moves.
Parameters:
			moves, a vector of pointer of vector of integers. It holds the pointers
			that point to a sequence of all moves out of which one best move must be
			selected
			best_move, a pointer of vector of integers. It holds the best move out of
			all the available moves passed as the first parameter
			indicator, an integer value passed by value. It holds 1 if the player
			has decided to cover, and 2 if the player has decided to uncover
Return Value: A pointer variable for a vector of integers which points to
			the location of a vector of integers that contains the cmobination of
			best possible moves for a player.
Algorithm:
			1) Iterate through the moves vector to find the vector with the largest
			number in it.
			2) If two vectors in the move vector has the same largest number, then
			pick the one with the smaller size, since the vector with the smaller
			size will have the larger numbers than the vector with bigger size.
			3) If this instance of the Computer class is a helper, then use the
			appropriate words a helper would use. And likewise if this instance is a
			Computer player.
Assistance Received: none
********************************************************************* */
vector<int>* Computer::find_best_move(vector<vector<int>*>moves, vector<int> *best_move, int indicator) {
	int max_square = 1;
	for (auto i = 0; i < moves.size(); i++) {
		for (auto j = 0; j < moves.at(i)->size(); j++) {
			// picking the move with the largest value of square
			if (moves.at(i)->at(j) > max_square) {
				max_square = moves.at(i)->at(j);
				best_move = moves.at(i);
			}
			// if two combinations have same largest square, then pick the combination with smaller size
			else if (moves.at(i)->at(j) == max_square) {
				if (moves.at(i)->size() < best_move->size()) {
					best_move = moves.at(i);
				}
			}
		}
	}
	best_move->insert(best_move->begin(), indicator);

	helper ? cout << "you should probably " : cout << get_name() << " decided to ";

	best_move->at(0) == 1 ? cout << "cover " : cout << "uncover ";

	cout << "( ";
	for (auto i = 1; i < best_move->size(); i++) {
		cout << best_move->at(i) << " ";
	}
	cout << ")." << endl;

	if (moves.size() == 1) {
		cout << "Because there is no other options for ";
		best_move->at(0) == 1 ? cout << "covering." : cout << "uncovering.";
		cout << endl;
	}
	else {
		(best_move->at(0) == 1) ? cout << "Because covering" : cout << "Because uncovering";
		cout << " the largest possible values maximizes winning score." << endl;
	}


	return best_move;

}

/* *********************************************************************
Function Name: cover_or_uncover
Purpose: To figure out whether to cover your own square of uncover an
		 opponent's squares in order to win and maximize points.
Parameters: none
Return Value: A boolean value which is true if the player decides to cover
			 and is false if the player decides to uncover.
Algorithm:
			1) If player has less covered squares than opponent's uncovered square,
			go for uncover
			2) If player has less uncovered squares than opponent's covered square,
			go for cover, unless there is less than or equal to 3 opponents uncovered squares
			left and more than 3 own uncovered squares, then go for uncover. Because you
			have to minimize the opponent's chances of winning.

Assistance Received: none
********************************************************************* */
bool Computer::cover_or_uncover() {

	if (opponent->get_uncovered_squares().size() <= 3 && get_uncovered_squares().size() > 3) {
		// uncover
		cout << "In order to minimize opponent's chances, ";
		return false;
	}

	if (get_covered_squares().size() < opponent->get_uncovered_squares().size()) {
		// uncover 
		cout << "In order to maximize uncovered squares, ";
		return false;

	}
	else if (get_uncovered_squares().size() < opponent->get_covered_squares().size()) {
		// cover
		cout << "In order to maximize covered squares, ";
		return true;

	}
	else {
		// if both equal then compare your uncovered squares with opponents uncovered squares 
		// and see for number with max value
		int max_num = 1;
		for (int i = 0; i < get_uncovered_squares().size(); i++) {
			if (get_uncovered_squares()[i] > max_num) {
				max_num = get_uncovered_squares()[i];
			}
		}
		for (int i = 0; i < opponent->get_covered_squares().size(); i++) {
			if (opponent->get_covered_squares()[i] > max_num) {
				// uncover
				cout << "In order to maximize uncovered squares and maximize score, ";
				return false;
			}
		}
		cout << "In order to maximize covered squares and minimize opponents score, ";
		// cover
		return true;
	}
}

// 
/* *********************************************************************
Function Name: roll_one
Purpose: To decide whether it is good to roll one die or two dice
Parameters: none
Return Value: A boolean value that is true if the player decides to roll one
			  dice and false if the player decides to roll two.
Algorithm:
			1) Iterate through a vector of the opponent's covered squares.
			2) roll one if opponent's covered squares are all less than 6, else
			roll two.
Assistance Received: none
********************************************************************* */
bool Computer::roll_one() {
	bool rollOne = true;

	cout << get_name() << " decided to ";

	for (int i = 0; i < opponent->get_covered_squares().size(); i++) {
		if (opponent->get_covered_squares()[i] > 6) {
			rollOne = false;
			break;
		}
	}

	rollOne ?
		cout << "roll one die because opponent has all squares greater than 6 uncovered." :
		cout << "roll two dice because opponent's covered squares can be uncovered as well.";

	cout << endl;
	return rollOne;
}

/* *********************************************************************
Function Name: set_opponent
Purpose: To set the opponent player for a computer player
Parameters:
			opp, a Player type pointer. It points to the memory location
			that contains the opponent player.
Return Value: a boolean value which is true if the opponent is set successfully,
			  and false if error occurs.
Algorithm:
			1) Check if opponent player pointer is pointing to a null pointer.
Assistance Received: none
********************************************************************* */
bool Computer::set_opponent(Player * opp) {
	if (opponent != nullptr) {
		opponent = opp;
		return true;
	}
	return false;
}
