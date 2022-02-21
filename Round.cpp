#include "Round.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "Computer.h"
#include <experimental/filesystem>

// resolves the cin.ignore(numeric_limits<streamsize>::max(), '\n') error
#define NOMINMAX
#include <Windows.h>

namespace fs = experimental::filesystem;

// program will crash if default constructor is called because players aren't initialized
Round::Round() {}

Round::Round(Player* player1, Player* player2) {
	this->player1 = player1;
	this->player2 = player2;
	this->winning_score = 0;
}

// destructor
Round::~Round() {
	delete view;
}

/* *********************************************************************
Function Name: start_round
Purpose: To simulate a round in a game.
Parameters:
			none
Return Value: none (void)
Algorithm:
			1) Allow a human player to pick the number of squares in a row.
			2) Initialize player boards with the chosen number of squares.
			3) Decide who plays the first turn by rolling dices.
			4) Display players names and boards.
			5) Let the current player make a move or if the player is human,
			allow the player to ask for help. The player will move if they
			roll values that provide possible cover or uncover moves when they
			are summed up.
			6) If there are no possible moves, then the turn switches.
Assistance Received: none
********************************************************************* */
void Round::start_round() {
	srand((u_int)time(NULL));

	// choose number of squares
	if (squares == NULL) {
		cout << "Choose the number of squares => 9, 10, or 11 (default - 9): ";
		cin >> squares;
		if (squares < 9 || squares >11) {
			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			squares = 9;
		}

		// initialize the player boards
		player1->initialize_board(squares);
		player2->initialize_board(squares);
	}

	view = new BoardView(player1, player2);

	GameUtils game_utils;

	// variable that denotes if it is the first turn of the round
	bool first_turn_round = true;

	if (currentPlayer == NULL && nextPlayer == NULL) {
		decide_first_player(squares);
	}
	else {
		// if the next player has all squares uncovered, then it is the first round because 
		// the player would have already been declared winner before being able to save the game.
		first_turn_round = nextPlayer->get_covered_squares().size() == 0 ? true : false;
	}

	// variable that denotes if the round is over
	bool round_over = false;
	while (!round_over) {
		view->display_boards();
		cout << "-----------" << currentPlayer->get_name() << "'s turn-----------" << endl;

		int action;
		if (currentPlayer->get_name() == "Computer") {
			// the computer should always pick option 2: make a move
			action = 2;
		}
		else {
			show_menu();
			cin >> action;
		}
		switch (action) {
		case 1:
			save_game(first_turn_round, squares);
			round_over = true;
			game_over = true;
			break;
		case 2:
		{
			vector<int> dice_values;
			int value = 0;

			if (l_dice_rolls != NULL && !(l_dice_rolls->empty())) {
				dice_values = l_dice_rolls->front();
				cout << currentPlayer->get_name() << " has been made to play dice rolls loaded from a file." << endl;
				cout << "Dices: ";
				for (auto & roll : dice_values) {
					cout << roll << " ";
				}
				cout << endl;
				l_dice_rolls->pop();
			}
			else {
				if (game_utils.roll_one_available(currentPlayer->get_uncovered_squares())) {
					if (currentPlayer->roll_one()) {
						dice_values = currentPlayer->roll_dice(true);
					}
					else {
						dice_values = currentPlayer->roll_dice(false);
					}
				}
				else {
					dice_values = currentPlayer->roll_dice(false);
				}
			}
			for (auto& it : dice_values) {
				value += it;
			}

			// check if the rolled value is valid to keep the next turn
			vector<vector<int>*> all_moves = game_utils.all_possible_moves(value);

			int moving_possibility = game_utils.check_valid(
				value,
				squares,
				game_utils.all_available_cover_moves(all_moves, currentPlayer->get_uncovered_squares()),
				game_utils.all_available_uncover_moves(all_moves, nextPlayer->get_covered_squares(), nextPlayer->get_advantage_square())
			);

			// deleting from memory now that we don't need the vector after this
			for (auto vec : all_moves) {
				delete vec;
			}
			all_moves.clear();

			if (moving_possibility == 0) {
				first_turn_round = false;
				cout << value << " doesn't leave " << currentPlayer->get_name() << " any possible moves." << endl;
				game_over = !set_player_turns(nextPlayer, currentPlayer, squares);
				continue;
			}

			bool valid_move_chosen = false;
			bool opponent_is_computer = nextPlayer->get_name() == "Computer" ? true : false;
			while (!valid_move_chosen) {
				vector<int> player_moves = currentPlayer->choose_a_move(value, moving_possibility, opponent_is_computer);
				if (currentPlayer->get_name() == "Computer") {
					valid_move_chosen = true;
				}
				else if (player_moves.at(0) == 0) {
					provide_help(squares, value, moving_possibility);
				}
				else {
					if (game_utils.check_availability(player_moves, currentPlayer->get_uncovered_squares(), nextPlayer->get_uncovered_squares())) {
						valid_move_chosen = true;
					}
					else {
						cout << "Please choose a valid move." << endl;
					}
				}
				if (valid_move_chosen) {
					// make the move (can make a player method)

					for (auto it = player_moves.begin() + 1; it != player_moves.end(); ++it) {
						player_moves.at(0) == 1 ? currentPlayer->cover(*it) : nextPlayer->uncover(*it);
					}
					if (currentPlayer->get_name() == "Computer") {
						system("pause");
					};
				}
			}

			// check if player won
			if (player_won(first_turn_round, squares)) {
				round_over = true;
				view->display_boards();
				continue;
			}

			break;
		}
		case 3:
			round_over = true;
			game_over = true;
			break;
		default:
			cout << "Invalid action." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

/* *********************************************************************
Function Name: set_player_turns
Purpose: To switch player turns
Parameters:
			curr_p, a Player type pointer. It points to the memory location
			of the player that should play the current turn.
			next_p,a Player type pointer. It points to the memory location
			of the player that should play the next turn.
			sq, an integer passed by value. It holds the total number of
			squares in a row the player chose to play with.
Return Value: none (void)
Algorithm:
			1) Assign the players and squares to this round's corresponding
			member variables.
Assistance Received: none
********************************************************************* */
bool Round::set_player_turns(Player * curr_p, Player * next_p, int sq) {
	if (curr_p == NULL || next_p == NULL || sq < 9 || sq > 11) {
		return false;
	}
	currentPlayer = curr_p;
	nextPlayer = next_p;
	squares = sq;
	return true;
}

/* *********************************************************************
Function Name: set_l_dice_rolls
Purpose: To assign the loaded dice rolls (if any) from a file
Parameters:
			loaded_dice_rolls, a queue of vector of integers passed by value.
			It holds a series of dice rolls loaded from a saved serializer file.
Return Value: none(void)
Algorithm:
			1) Assign passed dice rolls into one of the round object's members.
Assistance Received: none
********************************************************************* */
void Round::set_l_dice_rolls(queue<vector<int>> * loaded_dice_rolls) {
	if (loaded_dice_rolls != NULL) {
		l_dice_rolls = loaded_dice_rolls;
	}
}

/* *********************************************************************
Function Name: set_players
Purpose: To assign players to play in a round.
Parameters:
			p1 and p2, two Player type pointer variables. They holds memory location
			of the players who will play in the round.
Return Value: none (void)
Algorithm:
			1) Assign p1 and p2 to the round object's player1 and player2 member variables.
********************************************************************* */
bool Round::set_players(Player * p1, Player * p2) {
	if (p1 != NULL && p2 != NULL) {
		player1 = p1;
		player2 = p2;
		return true;
	}
	return false;
}

/* *********************************************************************
Function Name: decide_first_player
Purpose: To decide the first player after checking whom to provide the advantage to.
Parameters:
			squares, an integer passed by value. It holds the total number of
			squares in a row the player chose to play with.
Return Value: none (void)
Algorithm:
			1) Check if a player had the first turn on the last round they played.
			2) Call the function to provide advantage accordingly.
			3) Roll the dice for both players.
			4) The player with the greater total sum value of dices plays the first
			turn. If both players roll equal values, the process will repeat until
			a winner is decided.
Assistance Received: none
********************************************************************* */
void Round::decide_first_player(int squares) {
	int previous_winner_score = player1->get_previous_round_score() != 0 ?
		player1->get_previous_round_score() : player2->get_previous_round_score();

	// had_first_turn will only be true for any player only at the end of this 
	// function and this function is called only on the first turn. So this if else
	// block will never be executed on the first turn of the first round 
	if (player1->had_first_turn()) {
		give_advantage(player2, previous_winner_score, squares);
	}
	else if (player2->had_first_turn()) {
		give_advantage(player1, previous_winner_score, squares);
	}
	int player1Total = 0, player2Total = 0;
	vector<int> player1_dice_val;
	vector<int> player2_dice_val;
	while (player1Total == player2Total) {
		player1_dice_val = player1->roll_dice(false);
		player1Total = player1_dice_val[0] + player1_dice_val[1];
		player2_dice_val = player2->roll_dice(false);
		player2Total = player2_dice_val[0] + player2_dice_val[1];
	}
	if (player1Total > player2Total) {
		currentPlayer = player1;
		nextPlayer = player2;
		if (!(player1->set_had_first_turn(true)) || !(player2->set_had_first_turn(false))) {
			cerr << "Error while setting first turn identifier." << endl;
			exit(1);
		}
		cout << player1->get_name() << " goes first." << endl;
	}
	else {
		currentPlayer = player2;
		nextPlayer = player1;
		if (!(player1->set_had_first_turn(false)) || !(player2->set_had_first_turn(true))) {
			cerr << "Error while setting first turn identifier." << endl;
			exit(1);
		}
		cout << player2->get_name() << " goes first." << endl;
	}

	system("pause");
}

/* *********************************************************************
Function Name: show_menu
Purpose: To display the menu for the Human player before rolling the dice
Parameters: none
Return Value: none (void)
Algorithm:
			1) Print out the menu.
Assistance Received: none
********************************************************************* */
void Round::show_menu() {
	cout << "1 => Save the game." << endl;
	cout << "2 => Make a move." << endl;
	cout << "3 => Quit the game. " << endl;
	cout << "Choice: ";
}

/* *********************************************************************
Function Name: end_round
Purpose: To declare the scores at the end of a round and ask the user if
		 if they want to play again
Parameters: none
Return Value: a boolean, true if the user wants to end the game, false if
			  the user wants to play another round
Algorithm:
			1) Add winning score into the winner's total score.
			2) Set the winning score as the winner's previous round score
			   and set 0 as the other player's previous round score.
			3) Display the scores of this round along with the winner.
			4) Ask the user if they want to play another round.
Assistance Received: none
********************************************************************* */
bool Round::end_round() {
	if (winner == player1->get_name()) {
		if (!(player1->set_score(player1->get_score() + winning_score))) {
			cerr << "Error while setting score." << endl;
			exit(1);
		}
		if (!(player1->set_previous_round_score(winning_score)) || !(player2->set_previous_round_score(0))) {
			cerr << "Error while setting previous round score." << endl;
			exit(1);
		}
	}
	else {
		if (!(player2->set_score(player1->get_score() + winning_score))) {
			cerr << "Error while setting score." << endl;
			exit(1);
		}
		if (!(player2->set_previous_round_score(winning_score)) || !(player1->set_previous_round_score(0))) {
			cerr << "Error while setting previous round score." << endl;
			exit(1);
		}
	}
	view->display_scores(winner, winning_score);

	// will be true if the user decided to quit the game in the middle of their turn
	if (game_over) {
		system("pause");
		exit(0);
		return true;
	}

	char answer;
	cout << "Would you like to play another round?(Y/N) ";
	cin >> answer;

	if (answer == 'Y' || answer == 'y') {
		return false;
	}
	return true;
}

/* *********************************************************************
Function Name: provide_help
Purpose: To provide suggestions to the user when making a move.
Parameters:
			squares, an integer passed by value. It holds the total number of
			squares in a row the player chose to play with.
			value, an integer passed by value. It holds the sum of dice values
			rolled by the player.
			moving_possibility, an integer passed by value. It holds integers
			ranging from 1-3 which denotes whether the player has the option to
			cover or uncover or do either.
Return Value: none (void)
Algorithm:
			1) Instantiate helper, a pointer variable of the type Computer.
			2) Initialize the board.
			3) Choose the best move.
********************************************************************* */
void Round::provide_help(int squares, int value, int moving_possibility) {
	// passing true to constructor denotes that this is a helper Computer object
	// passing nextPlayer to constructor lets the object know who the opponent is
	// (i.e. the Computer) in order to access their uncovered squares.
	Computer * helper = new Computer(true, nextPlayer);
	// the board should be initialized with the same board state as the human.
	helper->initialize_board(squares, currentPlayer->get_board_state());

	vector<int> suggested_moves = helper->choose_a_move(value, moving_possibility, false);
	// clearing the heap right after we have no use for this
	delete helper;
}

/* *********************************************************************
Function Name: give_advantage
Purpose: To calculate which square should be covered to provide advantage
		 to the decided player, and then cover it.
Parameters:
			player, a Player type pointer variable. It points to the memory
			location of a player who has to be given the advantage.
			score, an integer passed by value. It holds the winning score of
			a round.
			squares, an integer passed by value. It holds the total number of
			squares in a row the player chose to play with.
Return Value: none (void)
Algorithm:
			1) Add every digit of score. If the digit is equal or more than the
			number of squares, then repeat the process until it passes that condition.
			2) The result should be passsed into the player's cover
			function to cover that square.
Assistance Received: none
********************************************************************* */
void Round::give_advantage(Player * player, int score, int squares) {
	int q = score, r = 0;
	int sum = 0;
	while (q > 0) {
		r = q % 10;
		q = q / 10;
		sum += r;
		if (q <= 0 && sum > squares) {
			q = sum + score;
			sum = 0;
			if (q < 0) {
				cout << q << endl;
				break;
			}
		}
	}
	if (sum > 0 && sum <= squares) {
		cout << player->get_name() << " was given the advantage." << endl;
		cout << "The square " << sum << " will be covered beforehand." << endl;
		player->cover(sum);
		if (!(player->set_advantage_square(sum))) {
			cerr << "Error in setting advantage square" << endl;
			exit(1);
		}
	}
	else {
		cout << "Advantange could not be given." << endl;
	}
	system("pause");
}

/* *********************************************************************
Function Name: player_won
Purpose: To check if a player has won the game after they make a move.
Parameters:
			first_turn, a boolean passed by value and squares, an integer
			passed by value. first_turn is true if it is the first turn
			of a round, and false if it is not. squares holds the number of
			squares in a row the player chose to play with.
Return Value: true if player has won, false if the player hasn't won
Algorithm:
			1) Get all covered squares from the current player and all uncovered
			squares from the next player.
			2) If the size of current player's covered squares is equal to the total
			number of squares i.e. all squares are covered, then the currentplayer
			has won.
			3) If the size of next player's uncovered squares is equal to the total
			number of seuares i.e. all squares are uncovered, unless it is the first
			turn of the round, the current player has won.
Assistance Received: none
********************************************************************* */
bool Round::player_won(bool first_turn, int squares) {
	vector<int> currentPlayer_covered_squares = currentPlayer->get_covered_squares();
	vector<int> nextPlayer_uncovered_squares = nextPlayer->get_uncovered_squares();
	if (currentPlayer_covered_squares.size() == squares) {
		cout << "Covered squares: " << currentPlayer_covered_squares.size() << endl;
		winner = currentPlayer->get_name();
		for (auto it = nextPlayer_uncovered_squares.begin(); it != nextPlayer_uncovered_squares.end(); ++it) {
			winning_score += *it;
		}
		return true;
	}

	// if it is the first turn, do not check uncovered squares
	if (!first_turn) {
		if (nextPlayer_uncovered_squares.size() == squares) {
			winner = currentPlayer->get_name();
			for (auto it = currentPlayer_covered_squares.begin(); it != currentPlayer_covered_squares.end(); ++it) {
				winning_score += *it;
			}
			return true;
		}
	}

	return false;
}

/* *********************************************************************
Function Name: save_game
Purpose: To save the player state and game state
Parameters:
			first_turn_round, a boolean variable passed by value. It holds true
			if it is the first turn of the round and false if vice versa.
			squares, an integer passed by value. It holds the number of
			squares in a row the player chose to play with.
Return Value: none (void)
Algorithm:
			1) Get all required player and game states.
			   (players board state, players names, next turn, first turn, and
			   loaded dice rolls, if any)
			2) Write them to a text file.
Assistance Received: none
********************************************************************* */
void Round::save_game(bool first_turn_round, int squares) {

	if (!fs::is_directory("saved_games") || !fs::exists("saved_games")) {
		fs::create_directory("saved_games");
	}

	int * player1_board = currentPlayer->get_board_state();
	int * player2_board = nextPlayer->get_board_state();
	string player1_name = currentPlayer->get_name();
	string player2_name = nextPlayer->get_name();
	int player1_score = currentPlayer->get_score();
	int player2_score = nextPlayer->get_score();

	string first_turn = currentPlayer->had_first_turn() ? currentPlayer->get_name() : nextPlayer->get_name();
	string next_turn = currentPlayer->get_name();

	ofstream myfile;

	string filename;
	bool valid_filename = false;
	while (!valid_filename) {
		valid_filename = true;
		cout << "Enter a name for this game " << "\n"
			<< "(Press N or n for default name: <player1-name> vs <player2-name>) : ";

		cin >> filename;

		// filename cannot be blank or have /\?<>:*"| in it
		if (filename == "N" || filename == "n") {
			filename = currentPlayer->get_name() + " vs " + nextPlayer->get_name();
		}
		if (filename.length() == 0) {
			valid_filename = false;
		}
		else {
			for (int i = 0; i < filename.length(); i++) {
				if (filename[i] == '?' ||
					filename[i] == '/' ||
					filename[i] == '\\' ||
					filename[i] == '<' ||
					filename[i] == '>' ||
					filename[i] == ':' ||
					filename[i] == '\"' ||
					filename[i] == '|' ||
					filename[i] == '*') {
					valid_filename = false;
					break;
				}
			}
		}
		if (!valid_filename) {
			cout << "Invalid filename : You cannot have a blank filename or the characters /\\?<>:*\" in it." << endl;
		}
	}

	myfile.open(fs::current_path().u8string() + "\\saved_games\\" + filename + ".txt");

	if (!myfile) {
		cerr << "File name" << endl;
		cout << "Game could not be saved." << endl;
		system("pause");
	}

	myfile << player1_name + ":" + "\n";
	myfile << "    Squares: ";
	for (int i = 0; i < squares; i++) {
		player1_board[i] == 0 ? myfile << to_string(i + 1) + " " : myfile << +"* ";
	}
	myfile << "\n";
	myfile << "    Score: ";
	myfile << to_string(player1_score) + "\n\n";
	myfile << player2_name + ":" + "\n";
	myfile << "    Squares: ";
	for (int i = 0; i < squares; i++) {
		player2_board[i] == 0 ? myfile << to_string(i + 1) + " " : myfile << +"* ";
	}
	myfile << "\n";
	myfile << "    Score: ";
	myfile << to_string(player2_score) + "\n\n";
	first_turn_round ? myfile << "First  turn: " : myfile << "First turn: ";
	myfile << first_turn + "\n";
	myfile << "Next turn: " + next_turn + "\n\n";

	if (l_dice_rolls != NULL) {
		myfile << "Dice: \n";

		size_t size = l_dice_rolls->size();
		for (auto i = 0; i < size; i++) {
			myfile << "   ";
			vector<int> dice_roll = l_dice_rolls->front();
			for (auto& iit : dice_roll) {
				cout << iit << " ";
				myfile << iit << " ";
			}
			myfile << "\n";
			l_dice_rolls->pop();
		}
	}
	myfile.close();

	cout << "Game saved." << endl;
}
