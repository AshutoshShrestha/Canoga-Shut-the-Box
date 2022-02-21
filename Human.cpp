#include "Human.h"
#include <iostream>
#include <algorithm>
#include <string>

// default constructor
Human::Human() {
	name = "Anonymous";
	if (!set_previous_round_score(0) || !set_previous_round_score(0)) {
		std::cerr << "Error while setting previous round score." << std::endl;
		exit(1);
	}
}

// parameterized constructor
Human::Human(std::string player_name) {
	name = player_name;
	if (!set_previous_round_score(0) || !set_previous_round_score(0)) {
		std::cerr << "Error while setting previous round score." << std::endl;
		exit(1);
	}
}

// destructor
Human::~Human() {}

/* *********************************************************************
Function Name: choose_a_move
Purpose: To allow the Human player to input their moves.
Parameters:
			value, an integer passed by value. It holds the sum of dice values
			rolled by the player.
			moving_possibility, an integer passed by value. It holds integers
			ranging from 0-3 which denotes whether the player has the option to
			cover or uncover or do either or ask for help.
			opponent_is_computer, a boolean variable pass by value. It holds
			true if the opponent is computer (literally) and vice versa.
Return Value: A vector of integers that contains a Human player's moves.
Algorithm:
			1) Ask the Human player whether they want to cover or uncover squares
			and provide a help option.
			2) If the player asks for help, then return a vector with 0 as the
			only value which denotes that a player is asking for help.
			3) If the player chooses to cover or uncover, allow them to input
			turn by turn the squares that they want to cover or uncover.
			4) As the player is entering their moves, validate the input by
			checking if the chosen numbers all add up to a given rolled dice value.
Assistance Received: none
********************************************************************* */
std::vector<int> Human::choose_a_move(int value, int moving_possibility, bool opponent_is_computer) {
	std::vector<int> user_choices;
	std::string action;
	bool valid_action = false;
	while (!valid_action) {
		valid_action = true;
		std::cout << "Choose which action:" << std::endl;
		if (opponent_is_computer) {
			std::cout << "h => Ask for help" << std::endl;
		}
		if (moving_possibility == 3) {
			std::cout << "c => cover" << std::endl;
			std::cout << "u => uncover" << std::endl;
		}
		else if (moving_possibility == 1) {
			std::cout << "c => cover" << std::endl;
		}
		else if (moving_possibility == 2) {
			std::cout << "u => uncover" << std::endl;
		}
		std::cout << "Action: ";
		std::cin >> action;

		if (opponent_is_computer & (action == "h" || action == "H")) {
			// return an indication that the user asked for help
			user_choices.push_back(0);
			return user_choices;
		}
		else if ((moving_possibility == 3 && action != "u" && action != "U" && action != "c" && action != "C") ||
			(moving_possibility == 2 && action != "u" && action != "U") ||
			(moving_possibility == 1 && action != "c" && action != "C"))
		{
			std::cout << "Invalid choice. Choose again." << std::endl;
			valid_action = false;
		}
	}
	int sum_of_choices = 0;
	(action == "c" || action == "C") ? user_choices.push_back(1) : user_choices.push_back(2);
	while (sum_of_choices != value) {
		int choice;
		(action == "c" || action == "C") ? std::cout << "Pick a square to cover: " : std::cout << "Pick a square to uncover: ";
		std::cin >> choice;

		sum_of_choices += choice;

		// if a choice is 0 or choices add up to be more than value or if a number has been chosen twice
		if (choice == 0 || sum_of_choices > value || count(user_choices.begin() + 1, user_choices.end(), choice)) {
			std::cout << "Invalid choice. Choose from the start." << std::endl;

			// in case user enters something other than an integer
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			// reset and start again
			user_choices.clear();
			(action == "c" || action == "C") ? user_choices.push_back(1) : user_choices.push_back(2);
			sum_of_choices = 0;
		}
		else {
			user_choices.push_back(choice);
		}
	}
	return user_choices;
}

/* *********************************************************************
Function Name: roll_one
Purpose: To allow the Human player to choose whether they want to roll one
		 die or two dices if they are eligible.
Parameters: none
Return Value: A boolean value that is true if the Human player decides to
			  roll one and false if the player decides to roll two.
Algorithm:
			1) Ask the player whether they would like to roll one dice or
			two dices.
Assistance Received: none
********************************************************************* */
bool Human::roll_one() {
	char rollOne;

	std::cout << "Squares greater than 6 are all covered." << std::endl;
	std::cout << "Would you like to roll a single die only? (Press Y for Yes/Anything else for No)" << std::endl;
	std::cin >> rollOne;

	if (rollOne == 'Y' || rollOne == 'y') {
		return true;
	}
	else {
		return false;
	}
}