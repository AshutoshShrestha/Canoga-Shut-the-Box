#include "Game.h"
#include "Human.h"
#include "Computer.h"
#include "Round.h"
#include "Board.h"
#include <iostream>
#include <string>

// default constructor
Game::Game() {
	// default mode player1 vs player2
	mode = 2;
	is_new_game = true;
	next_turn = "";
	squares = 0;
	player1 = NULL;
	player2 = NULL;
	current_round = NULL;
}

// parameterized constructor
Game::Game(int game_mode) {
	mode = game_mode;
	is_new_game = true;
	next_turn = "";
	squares = 0;
	player1 = NULL;
	player2 = NULL;
	current_round = NULL;
}

// parameterized constructor
Game::Game(Player * p1, Player * p2, string next, int sq, queue<vector<int>> * dice_rolls, int game_mode) {
	mode = game_mode;
	player1 = p1;
	player2 = p2;
	is_new_game = false;
	next_turn = next;
	squares = sq;
	loaded_dice_rolls = dice_rolls;
	current_round = new Round(p1, p2);
}

// destructor
Game::~Game() {
	if (loaded_dice_rolls != NULL) {
		size_t size = loaded_dice_rolls->size();
		for (int i = 0; i < size; i++) {
			loaded_dice_rolls->front().clear();
			loaded_dice_rolls->pop();
		}
	}
	delete loaded_dice_rolls;
	delete current_round;
	delete player1;
	delete player2;
}

/* *********************************************************************
Function Name: start_game
Purpose: To stimulate starting a game and initializing the players
Parameters: none
Return Value: none (void)
Algorithm:
			1) Check if the user wanted to play player vs computer or
			player vs player.
			2) If the user wanted to play player vs computer, ask for the
			name of the player and initialize both Human and Computer players.
			3) If the user wanted to play player vs player, ask for the names
			of both the players and initialize both Human players.
Assistance Received: none
********************************************************************* */
void Game::start_game() {
	bool quit = false;
	Player * p1, *p2;
	while (!quit) {
		quit = true;
		switch (getMode()) {
		case 1:
		{
			if (player1 == NULL && player2 == NULL) {
				// initialize player
				string player_name;
				cout << "Enter player name: ";
				cin >> player_name;

				if (player_name == "Computer") {
					cout << "Sorry, this name is reserved by the opponent. Pick a different name." << endl;
					quit = false;
				}
				else {
					p1 = new Human(player_name);
					p2 = new Computer(p1);

					initialize(p1, p2);
				}
			}
			else {
				initialize(player1, player2);
			}
			break;
		}
		case 2:
		{
			if (player1 == NULL && player2 == NULL) {
				// initialize players
				string player1_name, player2_name;
				cout << "Enter player 1 name: ";
				cin >> player1_name;
				cout << "Enter player 2 name: ";
				cin >> player2_name;

				if (player1_name == "Computer" || player2_name == "Computer") {
					cout << "Sorry, \"Computer\" is a reserved name. Pick a different name." << endl;
					quit = false;
				}
				else {
					p1 = new Human(player1_name);
					p2 = new Human(player2_name);

					initialize(p1, p2);
				}
			}
			else {
				initialize(player1, player2);
			}
			break;
		}
		case 3:
			break;
		default:
			quit = false;
			cout << "Invalid option. Try again." << endl;
			break;
		}
	}
}

/* *********************************************************************
Function Name: initialize
Purpose: To initialize subsequent rounds once a game starts.
Parameters:
			p1 and p2, two Player type pointer variables. They holds memory location
			of the players who will play in the game.
Return Value: none (void)
Algorithm:
			1) Assign the passed pointer locations to the member pointers of the game
			class.
			2) Start a round by initializing the round by passing the player's pointers
			as constructor parameters.
			3) Set player turns if the game has been loaded from a file.
			4) Call the function to start the round.
			5) Check if the user decided to quit the game or finish the game.
Assistance Received: none
********************************************************************* */
void Game::initialize(Player *p1, Player *p2) {
	this->player1 = p1;
	this->player2 = p2;

	bool end_game = false;
	while (!end_game) {
		// initialize the round
		if (is_new_game) {
			current_round = new Round(p1, p2);
		}
		Player *curr_player, *next_player;
		if (!is_new_game) {
			if (p1->get_name() == next_turn) {
				curr_player = p1;
				next_player = p2;
				current_round->set_player_turns(curr_player, next_player, squares);
			}
			else if (p2->get_name() == next_turn) {
				curr_player = p2;
				next_player = p1;
				current_round->set_player_turns(curr_player, next_player, squares);
			}
			is_new_game = true;
		}
		if (loaded_dice_rolls != NULL) {
			current_round->set_l_dice_rolls(loaded_dice_rolls);
		}
		current_round->start_round();
		if (current_round->end_round()) {
			end_game = true;
		}
		delete current_round;
		player1->clear_board();
		player2->clear_board();
	}
}

/* *********************************************************************
Function Name: end_game
Purpose: To declare the winner and print out the final scores.
Parameters: none
Return Value: none (void)
Algorithm:
			1) Print the game over header.
			2) Print each player's name with their final scores.
			3) Declare the winner. State that it was a draw if points are equal.
Assistance Received: none
********************************************************************* */
void Game::end_game() {
	cout << "----------GAME---OVER------------" << endl;
	cout << "----------Total Points-----------" << endl;
	cout << "	" << player1->get_name() << ": " << player1->get_score() << endl;
	cout << "	" << player2->get_name() << ": " << player2->get_score() << endl;
	cout << "---------------------------------" << endl;
	string winner = "";
	if (player1->get_score() == player2->get_score()) {
		cout << "The game was a draw." << endl;
	}
	else {
		winner = player1->get_score() > player2->get_score() ? player1->get_name() : player2->get_name();
		cout << "Winner: " << winner << endl;
	}
}