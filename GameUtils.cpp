#include "GameUtils.h"
#include <iostream>
#include <vector>

/* *********************************************************************
Function Name: GameUtils
Purpose: To construct a GameUtils object
Parameters: none
Return Value: a GameUtils object
Algorithm:
Assistance Received: none
********************************************************************* */
GameUtils::GameUtils() {}

/* *********************************************************************
Function Name: ~GameUtils
Purpose: To destroy/cleanup a GameUtils object
Parameters: none
Return Value: 
Algorithm:
Assistance Received: none
********************************************************************* */
GameUtils::~GameUtils() {}

/* *********************************************************************
Function Name: check_valid
Purpose: To check the possibilities of what kind of moves a player can make
		 according to the dice value they rolled.
Parameters:
			value, an integer passed by value. It holds the sum of dice values
			rolled by the player
			squares, an integer passed by value. It holds the total number of
			squares in a row the player chose to play with
			available_cover_moves, a vector of pointer of vector of integers.
			It holds all the possible available cover moves for a player.
			available_uncover_moves, a vector of pointer of vector of integers.
			It holds all the possible available uncover moves for a player.
Return Value: An integer variable which denotes that a player:
cannot cover or uncover - 0
can only cover - 1
can only uncover - 2
can both cover or uncover - 3

Algorithm:
			1) Display all available cover and uncover moves to the player.
			2) If the sizes of all available cover and uncover moves are both 0,
			return 0.
			3) Else if only size of available uncover moves is 0, then return 1
			4) Else if only size of available cover moves is 0, then return 2
			5) Else return 3
Assistance Received: none
********************************************************************* */
int GameUtils::check_valid(int value, int squares, std::vector<std::vector<int>*> available_cover_moves, std::vector<std::vector<int>*> available_uncover_moves) {
	// an int variable that will hold the return numeric constant value
	// assigning 3 at the beginning to indicate that by default, the user can both cover or uncover
	int retVal = 3;

	std::cout << "Available cover moves: " << available_cover_moves.size();
	if (available_cover_moves.size() > 0) std::cout << " => ";
	for (auto& it : available_cover_moves) {
		std::cout << "( ";
		for (auto& iit : *it) {
			std::cout << iit << " ";
		}
		std::cout << ")  ";
	}
	std::cout << std::endl;

	std::cout << "Available uncover moves: " << available_uncover_moves.size();
	if (available_uncover_moves.size() > 0) std::cout << " => ";
	for (auto& it : available_uncover_moves) {
		std::cout << "( ";
		for (auto& iit : *it) {
			std::cout << iit << " ";
		}
		std::cout << ")  ";
	}
	std::cout << std::endl;

	if (available_cover_moves.empty() && available_uncover_moves.empty()) {
		// cannot cover or uncover
		retVal = 0;
	}
	else if (available_uncover_moves.empty()) {
		// can only cover
		retVal = 1;
	}
	else if (available_cover_moves.empty()) {
		// can only uncover
		retVal = 2;
	}
	else {
		// can both cover or uncover
		retVal = 3;
	}

	return retVal;
}

/* *********************************************************************
Function Name: check_availability
Purpose: To check if a Human Player's entered moves is allowable in the
		 present state of the board. To check if a player isn't trying to
		 cover an already covered square, or vice versa.
Parameters:
			moves, a combination of moves (ints) chosen by a player as their next move
			currentP_uncovered_squares, a vector of integers that holds a vector of
			the current player's uncovered squares.
			nextP_uncovered_squares, a vector of integers that holds a vector of
			the next player's uncovered squares.
Return Value: A boolean variable that is true if the move is allowable, and false
			  if the move is not allowable.
Algorithm:
			1) Check if a player has chosen to cover or uncover.
			2) If the player has chosen to cover, check the player's vector of
			uncovered squares to see if any of the squares chosen by the player as
			the next move is not found.
			3) If the player has chosen to uncover, check the opponent's vector of
			uncovered squares to see if all of the squares chosen by the player as
			the next move is found.
Assistance Received: none
********************************************************************* */
bool GameUtils::check_availability(std::vector<int> moves, std::vector<int> currentP_uncovered_squares, std::vector<int> nextP_uncovered_squares) {
	if (moves.at(0) == 1) {
		bool flag;

		for (auto i = moves.begin() + 1; i != moves.end(); ++i) {
			flag = true;
			for (auto j = currentP_uncovered_squares.begin(); j != currentP_uncovered_squares.end(); ++j) {
				if (*i == *j) {
					flag = false;
					break;
				}
			}
			if (flag == true) {
				// means that the square is already covered
				std::cout << "Invalid move. Cannot cover an already covered square." << std::endl;
				return false;
			}
		}
	}
	else {
		std::cout << std::endl;
		for (auto i = moves.begin() + 1; i != moves.end(); ++i) {
			for (auto j = nextP_uncovered_squares.begin(); j != nextP_uncovered_squares.end(); ++j) {
				if (*i == *j) {
					// means that the square is already covered
					std::cout << "Invalid move. Cannot uncover an already uncovered square." << std::endl;
					return false;
				}
			}
		}
	}
	return true;
}

/* *********************************************************************
Function Name: check_unique
Purpose: To check if all the numbers in a combination are unique.
Parameters:
			one_combination, a vector of integers passed by value. It holds
			a combination of moves (ints)
Return Value: A boolean value that is true if the combination has all unique
			  numbers and false if the combination does not have all unique
			  numbers.
Algorithm:
			1) Iterate through the vector of the combination, and check if
			there are any two numbers that are equal to each other.
Assistance Received: none
********************************************************************* */
bool GameUtils::check_unique(std::vector<int> one_combination) {
	for (auto i = 0; i < (int)one_combination.size() - 1; i++) {
		for (auto j = i + 1; j < one_combination.size(); j++) {
			if (one_combination[i] == one_combination[j]) {
				return false;
			}
		}
	}
	return true;
}

/* *********************************************************************
Function Name: findCombinationsUtil
Purpose: To
Parameters:
			all_possible_combinations, a Pointer to Vector of Vectors.
			It holds all the possible combinations of moves a user
			can make from a given dice roll value.
			arr, an integer array. It holds one possible combination
			from all possible combinations of moves a user can make.
			index, an integer variable. It holds the index of array
			which holds one possible combination out of all combinations.
			num, an integer variable. It holds the value from which
			all moves need to be calculated from. This value is the sum
			of both dice rolled values of a player.
			reducedNum, an integer variable. It holds the number of levels
			in the recursive tree when the function findCombinationsUtil 
			is called recursively.
Return Value: none (void)
Algorithm:
			1) If reducedNum is less than 0, then return from the function.
			2) If reducedNum is equal to 0, then form a vector and copy
			the elements from arr into the vector one_possible_combination.
			Check if the combination has all unique elements. If yes, push
			the vector into the all_possible_combinations vector.
			3) If reducedNum is greater than 0, then find the previous
			number stored in arr, if none then use 1.
			4) Loop from the previous number upto passed num value.
			5) Set the value at index position of array to k.
			6) Recursively call findCombinationsUtil function with
			reducedNum.
Assistance Received: Geeks4Geeks
********************************************************************* */
void GameUtils::findCombinationsUtil(std::vector<std::vector<int> *> *all_possible_combinations, int arr[], int index, int num, int reducedNum) {
	// Base condition
	if (reducedNum < 0)
		return;

	// If combination is found, push it to vector
	if (reducedNum == 0)
	{
		std::vector<int> * one_possible_combination = new std::vector<int>();
		for (int i = 0; i < index; i++) {
			one_possible_combination->push_back(arr[i]);
		}
		if (check_unique(*one_possible_combination)) {
			all_possible_combinations->push_back(one_possible_combination);
		}
		return;
	}

	// Find the previous number stored in arr[]
	// It helps in maintaining increasing order
	int prev = (index == 0) ? 1 : arr[index - 1];

	// note: loop starts from previous number
	// i.e. at array location index - 1
	for (int k = prev; k <= num; k++)
	{
		// next element of array is k
		arr[index] = k;

		// call recursively with reduced number
		findCombinationsUtil(all_possible_combinations, arr, index + 1, num, reducedNum - k);
	}
}

/* *********************************************************************
Function Name: findCombinations
Purpose: To
Parameters:
			all_possible_combinations, a Pointer to Vector of Vectors.
			It holds all the possible combinations of moves a user
			can make from a given dice roll value.
			n, an integer variable. It holds the value from which
			all moves need to be calculated from. This value is the sum
			of both dice rolled values of a player.
Return Value: none (void)
Algorithm:
			1) call findCombinationsUtil function to initiate the 
			recursive sequence.
Assistance Received: Geeks4Geeks
********************************************************************* */
void GameUtils::findCombinations(std::vector<std::vector<int> *> *all_possible_combinations, int n) {
	// array to store the combinations
	// It can contain max n elements
	int* arr = new int[n];

	//find all combinations
	findCombinationsUtil(all_possible_combinations, arr, 0, n, n);
	delete[] arr;
}

/* *********************************************************************
Function Name: all_possible_moves
Purpose: To find all possible combinations from a given rolled dice value.
Parameters:
			value, an integer passed by value. It holds the sum of dice values
			rolled by the player.
Return Value: a Vector of Pointer of Vector of integers that contains the
			  memory locations that hold all the combinations of moves that
			  can be made from a given value.
Algorithm:
			1) Call the recursive function to find all possible combination
			from a given value.
Assistance Received: none
********************************************************************* */
std::vector<std::vector<int> *> GameUtils::all_possible_moves(int value) {
	// a vector pointer that will hold all possible moves from a given value
	std::vector<std::vector<int> *> * all_moves = new std::vector<std::vector<int>*>();
	findCombinations(all_moves, value);
	std::vector<std::vector<int>*> all_possible_moves = *all_moves;
	delete all_moves;
	return all_possible_moves;
}

/* *********************************************************************
Function Name: all_available_cover_moves
Purpose: To find all available cover moves by comparing a given list of all
		 possible moves with the player's uncovered squares.
Parameters:
			all_possible_moves, a vector of pointer of vector of integers.
			It holds the memory locations that point to all possible moves
			from a rolled dice value.
			currentPlayer_uncovered_sq, a vector of integers passed by value.
			It holds all the uncovered squares of the current player.
Return Value: A Vector of Pointer of Vector of integers that holds all the
			  memory locations for all available cover moves.
Algorithm:
			1) Iterate through the vector of all_possible_moves.
			2) Iterate thorugh each move inside the vector.
			3) Iterate through the currentPlayer_uncovered_sq vector.
			4) Check if you find all the numbers from each vector of all_possible_moves
			in the currentPlayer_uncovered_sq vector.
			5) If there is a vector that has an integer which isn't found in the
			currentPlayer_uncovered_sq vector, then remove that vector from
			all_possible_moves.
			6) Return all_possible_moves which has now been modified into
			all_available_cover_moves vector.
Assistance Received: none
********************************************************************* */
std::vector<std::vector<int>*> GameUtils::all_available_cover_moves(std::vector<std::vector<int>*> all_possible_moves, std::vector<int> currentPlayer_uncovered_sq) {
	// check currentPlayer's uncovered squares with all_possible_cover_moves
	std::vector<std::vector<int>*> all_available_moves;
	for (auto i = 0; i < (int) all_possible_moves.size(); i++) {
		bool found = false;;
		for (auto j = 0; j < (int) all_possible_moves[i]->size(); j++) {
			found = false;
			for (auto k = 0; k < (int) currentPlayer_uncovered_sq.size(); k++) {
				if (all_possible_moves[i]->at(j) == currentPlayer_uncovered_sq[k]) {
					found = true;
					break;
				}
			}
			if (!found) {
				break;
			}
		}
		if (found) {
			all_available_moves.push_back(all_possible_moves[i]);
		}
	}
	return all_available_moves;
}

/* *********************************************************************
Function Name: all_available_uncover_moves
Purpose: To find all available uncover moves by comparing a given list of all
		 possible moves with the opponent's covered squares.
Parameters:
			all_possible_moves, a vector of pointer of vector of integers.
			It holds the memory locations that point to all possible moves
			from a rolled dice value.
			nextPlayer_covered_sq, a vector of integers passed by value.
			It holds all the covered squares of the opponent player.
			advantage_square, an integer passed by value. It holds the square
			that has been decided to be covered from the start of the round for
			a given advantaged player.
Return Value: A Vector of Pointer of Vector of integers that holds all the
			  memory locations for all available uncover moves.
Algorithm:
			1) Iterate through the vector of all_possible_moves.
			2) Iterate thorugh each move inside the vector.
			3) Iterate through the nextPlayer_covered_sq vector.
			4) Check if you find all the numbers from each vector of all_possible_moves
			in the nextPlayer_covered_sq vector.
			5) If there is a vector that has an integer which isn't found in the
			nextPlayer_covered_sq vector, then remove that vector from
			all_possible_moves.
			6) If the size of all_available_moves vector is 1 and that one vector has
			the advantage square as the first element, empty the all_available_moves and
			return empty vector.
			7) Else return all_possible_moves which has now been modified into
			all_available_uncover_moves vector.
Assistance Received: none
********************************************************************* */
std::vector<std::vector<int>*> GameUtils::all_available_uncover_moves(std::vector<std::vector<int>*> all_possible_moves, std::vector<int> nextPlayer_covered_sq, int advantage_square) {
	// check nextPlayer's covered squares with all_possible_uncover_moves
	std::vector<std::vector<int>*> all_available_moves;
	for (auto i = 0; i < (int) all_possible_moves.size(); i++) {
		bool found = false;;
		for (auto j = 0; j < (int) all_possible_moves[i]->size(); j++) {
			found = false;
			for (auto k = 0; k < (int )nextPlayer_covered_sq.size(); k++) {
				if (all_possible_moves[i]->at(j) == nextPlayer_covered_sq[k]) {
					found = true;
					break;
				}
			}
			if (!found) {
				break;
			}
		}
		if (found) {
			all_available_moves.push_back(all_possible_moves[i]);
		}
	}
	if (all_available_moves.size() == 1 && all_available_moves.at(0)->at(0) == advantage_square) {
		all_available_moves.clear();
	}
	return all_available_moves;
}

/* *********************************************************************
Function Name: roll_one_available
Purpose: To check if a player is eligible to have to option to roll one dice.
Parameters:
			uncovered_squares, a vector of integers passed by value. It holds
			the player's own uncovered squares.
Return Value: a boolean value which is true if a player is eligible to have
			  the option to roll one dice and vice versa.
Algorithm:
			1) Iterate through the vector of uncovered squares.
			2) If a number greater than 6 is found, return false.
			3) If no number is greater than 6, return true.
Assistance Received: none
********************************************************************* */
bool GameUtils::roll_one_available(std::vector<int> uncovered_squares) {
	for (auto i = uncovered_squares.begin(); i != uncovered_squares.end(); ++i) {
		if (*i > 6) {
			return false;
		}
	}
	return true;
}