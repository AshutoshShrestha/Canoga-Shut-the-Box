#include "Game.h"
#include "Human.h"
#include "Computer.h"
#include "Round.h"
#include "Board.h"
#include <iostream>
#include <string>

/* *********************************************************************
Function Name: Game
Purpose: To construct a default Game object
Parameters: none
Return Value: a Game object
Algorithm:
			1) Set the default values for all members of game object
Assistance Received: none
********************************************************************* */
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

/* *********************************************************************
Function Name: Game
Purpose: To construct a Game object
Parameters: 
			game_mode, an integer variable to indicate if the user chose
			single player or multiplayer mode. 
Return Value: a Computer object
Algorithm:
			1) Set the parameter as the object's member and set some
			default values for other members.
Assistance Received: none
********************************************************************* */
Game::Game(int game_mode) {
	mode = game_mode;
	is_new_game = true;
	next_turn = "";
	squares = 0;
	player1 = NULL;
	player2 = NULL;
	current_round = NULL;
}

/* *********************************************************************
Function Name: Game
Purpose: To construct a Game object while loading from a file
Parameters: 
			p1, p2, two Player pointers that hold the memory location to
			the two players playing the game. 
			next, a string variable passed by value. It holds the name
			of the player who should be the current player in the round.
			sq, an integer variable passed by value. It holds the number 
			of squares the player player chose to play with.
			dice_rolls, a pointer Queue of vectors of int. It holds the 
			dice rolls that have been loaded from the file and should be
			played by the player in the subsequent turns.
			game_mode, an integer variable to indicate if the user chose
			single player or multiplayer mode.
Return Value: a Game object
Algorithm:
			1) Set corresponding parameters with the internal members of
			the Game object.
Assistance Received: none
********************************************************************* */
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

/* *********************************************************************
Function Name: ~Game
Purpose: To destroy/cleanup a Game object
Parameters: none
Return Value: 
Algorithm:
			1) Check if there is anything in loaded_dice_rolls. If it has
			any vectors in it, clear up the vectors and pop it off the queue.
			2) Delete the loaded_dice_rolls queue pointer and the player 
			pointers.
Assistance Received: none
********************************************************************* */
Game::~Game() {
	if (loaded_dice_rolls != NULL) {
		size_t size = loaded_dice_rolls->size();
		for (int i = 0; i < (int)size; i++) {
			loaded_dice_rolls->front().clear();
			loaded_dice_rolls->pop();
		}
	}
	delete loaded_dice_rolls;
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
	// will be true if game should be over
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

	// will be true when the game should be finished
	bool end_game = false;
	while (!end_game) {
		// checking if this is a new game or a loaded game
		if (is_new_game) {
			// initialize the round
			current_round = new Round(p1, p2);
		}

		// Player pointers to hold the memory addresses of the two players
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
		// clearing the heap
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