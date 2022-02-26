#include "Player.h"
#include <time.h>
#include <string>
#include <iostream>

/* *********************************************************************
Function Name: Player
Purpose: To construct a Player object
Parameters: none
Return Value: a Player object
Algorithm:
			1) Set the default values of the member variabes
Assistance Received: none
********************************************************************* */
Player::Player() {
	score = 0;
	first_turn = false;
	advantage_square = 0;
}

/* *********************************************************************
Function Name: ~Player
Purpose: To destroy/cleanup a Player object
Parameters: none
Return Value:
Algorithm:
Assistance Received: none
********************************************************************* */
Player::~Player() {}

/* *********************************************************************
Function Name: roll_dice
Purpose: To simulate rolling a dice for a player.
Parameters:
			rollOne, a boolean passed by value. It is true if the player
			has decided to roll one die only, and is false if the player
			has decided to roll both dice.
Return Value: a vector of integers that holds the rolled dice value/s.
Algorithm:
			1) If rollOne is true, then push a random int from 1-6 into
			a integer vector and return it.
			2) If rollOne is false, then push two random ints from 1-6 into
			a integer vector and return it.
Assistance Received: none
********************************************************************* */
std::vector<int> Player::roll_dice(bool rollOne) {
	// a vector of ints to hold the random dice values that will be assigned to a player
	std::vector<int> dice_values;
	if (rollOne) {
		dice_values.push_back((rand() % 6) + 1);
		std::cout << get_name() << " rolled " << dice_values[0] << "." << std::endl;
		return dice_values;
	}
	dice_values.push_back((rand() % 6) + 1);
	dice_values.push_back((rand() % 6) + 1);
	std::cout << get_name() << " rolled " << dice_values[0] << " and " << dice_values[1] << "." << std::endl;
	return dice_values;
}

/* *********************************************************************
Function Name: set_score
Purpose: To set a player's score
Parameters:
			points, an integer passed by value. It holds the points that
			are to be set as a player's score.
Return Value: a boolean variable that is true if the score is successfully set
			and false if there is error setting the score.
Algorithm:
			1) Check if points is a positive number or 0.
Assistance Received: none
********************************************************************* */
bool Player::set_score(int points) {
	if (points >= 0) {
		score = points;
		return true;
	}
	return false;
}

/* *********************************************************************
Function Name: set_previous_round_score
Purpose: To set a player's score from the previous round
Parameters:
			score, an integer variable passed by value. It holds the
			score that is to be set as the score obtained by a player
			in the previous round.
Return Value: a vector of integers that holds the rolled dice value/s.
Algorithm:
			1) Check if the score is a positive number or 0.
Assistance Received: none
********************************************************************* */
bool Player::set_previous_round_score(int score) {
	if (score >= 0) {
		previous_round_score = score;
		return true;
	}
	return false;
}

/* *********************************************************************
Function Name: set_had_first_turn
Purpose: To set a member variable that denotes whether the player played
		 the first turn or not.
Parameters:
			first, a boolean variable passed by value. It holds true if
			the player had played the first turn and vice versa.
Return Value: returns true if the value was set correctly, and false if
			  an error occurs.
Algorithm:
			1) Check if the value of first is null or not.
Assistance Received: none
********************************************************************* */
bool Player::set_had_first_turn(bool first) {
	if (first == true || first == false) {
		first_turn = first;
		return true;
	}
	return false;
}

/* *********************************************************************
Function Name: set_advantage_square
Purpose: To set the advantage square for a decided player
Parameters:
			square, an integer variable passed by value. It holds the
			square that is to be set as the advantage square for a
			decided player.
Return Value: returns true if the value was set correctly, and false if
			  an error occurs.
Algorithm:
			1) Check if the value of square is less than or equal to 0 or not.
Assistance Received: none
********************************************************************* */
bool Player::set_advantage_square(int square) {
	if (square > 0) {
		advantage_square = square;
		return true;
	}
	return false;
}
