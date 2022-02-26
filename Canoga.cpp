/************************************************************
* Name:  Ashutosh Shrestha								   *
* Project : Canoga 1								   *
* Class : <number> OPL				                       *
* Date : 2/22/2022				                           *
************************************************************/

#include <stdio.h>
#include <iostream>
#include "Game.h"
#include "Player.h"
#include "Human.h"
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>
// resolves the cin.ignore(numeric_limits<streamsize>::max(), '\n') error
#define NOMINMAX
#include <Windows.h>

namespace fs = std::experimental::filesystem;

// function definitions
void new_game();
bool load_game();
void show_menu();

/* *********************************************************************
Function Name: main
Purpose: To handle the user's request of starting a new game or loading
		 a previously saved game or quitting the game.
Parameters: none
Return Value: An integer variable which is 0 if the program ran successfully
			  and 1 if something went wrong.
Algorithm:
			1) Get the user's action.
			2) Handle different cases according to user's choice.
Assistance Received: none
********************************************************************* */
int main() {
	int action;

	// quit boolean will be true only when the user wants to quit
	bool quit = false;
	// ask user to choose multiplayer or singleplayer

	while (!quit) {
		quit = true;
		system("cls");
		show_menu();
		std::cin >> action;
		switch (action) {
		case 1:
			new_game();
			break;
		case 2:
			quit = load_game();
			break;
		case 3:
			break;
		default:
			// clearing cin to avoid error in future inputs
			std::cout << "Invalid choice." << endl;
			std::cin.clear();
			std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
			quit = false;
			break;
		}
	}

	system("pause");
	return 1;
}


/* *********************************************************************
Function Name: show_menu
Purpose: To display the options - new game, load game or quit - to the user.
Parameters: none
Return Value: none (void)
Algorithm:
			1) Print out the menu.
Assistance Received: none
********************************************************************* */
void show_menu() {
	std::cout << "1 => Play new game" << endl;
	std::cout << "2 => Load previously saved game" << endl;
	std::cout << "3 => Quit" << endl;
	std::cout << "Choice: ";
}


/* *********************************************************************
Function Name: new_game
Purpose: To initialize a new game and launch it and handle ending it.
Parameters: none
Return Value: none (void)
Algorithm:
			1) Ask the user whether they would like to play player vs
			computer or player vs player.
			2) Initialize an instance of Game and set the game mode
			according to what was chosen by the user.
			3) Start the game by callig start_game.
			4) Handle ending the game by calling end_game.
Assistance Received: none
********************************************************************* */
void new_game() {
	bool valid_choice = false;
	while (!valid_choice) {
		valid_choice = true;
		system("cls");
		int play_mode;
		std::cout << "1 => Single player (player vs computer)" << endl;
		std::cout << "2 => Multi player (player1 vs player2)" << endl;
		std::cout << "Choice: ";
		std::cin >> play_mode;
		if (play_mode == 1 || play_mode == 2) {
			Game newGame(play_mode);
			newGame.start_game();
			newGame.end_game();
		}
		else {
			// clearing cin to avoid error in future inputs
			std::cout << "Invalid choice." << endl;
			valid_choice = false;
			std::cin.clear();
			std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

/* *********************************************************************
Function Name: load_game
Purpose: To load a game from a file.
Parameters: none
Return Value: a boolean variable that is true when the game successfully loads
			  and false when there is error loading the game
Algorithm:
			1) Read the saved_games directory for any saved games.
			2) Display the file names to the user and allow them to pick one.
			3) Assign the player names and board states from the file to
			some variables.
			4) Iniitialize the players and initialize their boards.
			5) Print out the information to the user to confirm if this is
			the game that they want to load.
			6) Instantiate a Game class and pass all the information obtained
			from the file.
			7) Start the game by calling start_game.
			8) Handle ending the game by calling end_game.
Assistance Received: none
********************************************************************* */
bool load_game() {
	std::cout << "Loading previously saved games..." << std::endl;
	std::string path = " ";
	path = fs::current_path().u8string() + "\\saved_games";

	if (!fs::is_directory(path) ||
		!fs::exists(fs::current_path())) {
		std::cout << "There are no saved games." << std::endl;
		Sleep(3000);
		return false;
	}

	std::vector<fs::path> file_names;
	std::string filename;

	bool valid_file = false;
	while (!valid_file) {
		valid_file = true;
		int count = 0;
		for (const auto & entry : fs::directory_iterator(fs::path(path))) {
			count++;
			std::string filepath = entry.path().u8string();
			filename = filepath.substr(filepath.find("saved_games\\") + 12, filepath.find(".txt") + 4);
			file_names.push_back(filename);
			std::cout << count << ". " << filename << std::endl;
		}
		if (count == 0) {
			std::cout << "There are no saved games. Returning back to the main menu..." << std::endl;
			Sleep(5000);
			return false;
		}

		std::cout << count + 1 << ". Cancel and go back." << std::endl;

		std::cout << "Which game would you like to load? (Enter the corresponding file number) " << std::endl;

		int choice;
		std::cout << "Choose file number: ";
		std::cin >> choice;

		if (choice > count || choice < 1) {
			// because there is no need to clear cin 
			if (choice == count + 1) {
				return false;
			}
			std::cout << "Invalid file." << std::endl;
			// clearing cin to avoid error in future inputs
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			valid_file = false;
		}
		else {
			filename = file_names.at(choice - 1).u8string();
		}
	}

	std::cout << filename << std::endl;

	std::ifstream input_file(fs::current_path().u8string() + "\\saved_games\\" + filename);
	if (!input_file.is_open()) {
		std::cerr << "Could not open the file - '"
			<< filename << "'" << std::endl;
		std::cerr << "Loading game failed." << std::endl;
		system("pause");
		return false;
	}

	std::vector<std::string> lines;
	std::queue<std::vector<int>> * dice_rolls = new std::queue<std::vector<int>>();
	std::string line;
	int i = 0;
	bool dice_rolls_line = false;
	while (getline(input_file, line))
	{
		cout << line << endl;
		if (line.find("Dice:") != string::npos) {
			dice_rolls_line = true;
			i++;
			continue;
		}
		if (dice_rolls_line) {
			// dice rolls that are about to be loaded from the file
			vector<int> dice_roll;
			for (int j = 0; j < line.length(); j++) {
				if (isdigit(line[j])) {
					// converting char to int and pushing in vector
					dice_roll.push_back(line[j] - '0');
				}
			}
			dice_rolls->push(dice_roll);
		}
		else {
			std::istringstream iss(line);
			lines.push_back(line);
		}
		i++;
	}
	input_file.close();

	std::cout << "Is this what you're looking for? (Y/N)";

	char user_choice;
	std::cin >> user_choice;

	if (user_choice == 'Y' || user_choice == 'y') {
		std::string player1_name, player2_name, first_turn, next_turn;
		int player1_score, player2_score;

		player1_name = lines[0].substr(0, lines[0].find(":"));
		player2_name = lines[4].substr(0, lines[4].find(":"));

		std::istringstream(lines[2].substr(lines[2].find(":") + 2, -1)) >> player1_score;
		std::istringstream(lines[6].substr(lines[6].find(":") + 2, -1)) >> player2_score;

		first_turn = lines[8].substr(lines[8].find(":") + 2, -1);
		next_turn = lines[9].substr(lines[9].find(":") + 2, -1);

		if ((first_turn != player1_name && first_turn != player2_name) ||
			(next_turn != player1_name && next_turn != player2_name)) {
			cout << "Something went wrong. File could not be loaded" << endl;
			return false;
		}

		std::string player1_board_state, player2_board_state;
		player1_board_state = lines[1].substr(lines[1].find(":") + 2, -1);
		player2_board_state = lines[5].substr(lines[5].find(":") + 2, -1);

		// players whose internal members will be set using the information from the saved file
		Player * p1, *p2;

		// play_mode 1 = single player, 2 = multi player
		int play_mode = 2;

		if (player1_name != "Computer") {
			p1 = new Human(player1_name);
			if (player2_name == "Computer") {
				p2 = new Computer(p1);
			}
			else {
				p2 = new Human(player2_name);
			}
		}
		else {
			p2 = new Human(player2_name);
			p1 = new Computer(p2);
			play_mode = 1;
		}

		if (!(p1->set_score(player1_score)) || !(p2->set_score(player2_score))) {
			cerr << "Error while setting score." << endl;
			exit(1);
		}

		// board dynamic int arrays whose elements will be set by player board state from the loaded file
		int * player1_board = new int[11], *player2_board = new int[11];

		// int variable to count the number of squares
		int index = 0;
		for (int i = 0; i < player1_board_state.length(); i++) {
			char square_state = player1_board_state[i];
			if (square_state == '*') {
				player1_board[index] = 1;
				++(index);
			}
			else if (isdigit(square_state)) {
				bool quit = false;
				while (!quit) {
					if (i < player1_board_state.length() && !isdigit(player1_board_state[i + 1])) {
						quit = true;
						continue;
					}
					i++;
				}
				player1_board[index] = 0;
				++(index);
			}
		}

		// resetting index
		index = 0;
		for (int i = 0; i < player2_board_state.length(); i++) {
			char square_state = player2_board_state[i];
			if (square_state == '*') {
				player2_board[index] = 1;
				++(index);
			}
			else if (isdigit(square_state)) {
				bool quit = false;
				while (!quit) {
					if (i < player2_board_state.length() && !isdigit(player2_board_state[i + 1])) {
						quit = true;
						continue;
					}
					i++;
				}
				player2_board[index] = 0;
				++(index);
			}
		}

		p1->initialize_board(index, player1_board);
		p2->initialize_board(index, player2_board);

		// checking if there was any advantage square that cannot be uncovered ( if its the first round )
		if (lines[8].substr(lines[8].find("First") + 5)[1] != 'T') {
			if (player1_name == next_turn && (p2->get_covered_squares().size() == 1)) {
				p2->set_advantage_square(p2->get_covered_squares()[0]);
			}
			else if (player2_name == next_turn && (p1->get_covered_squares().size() == 1)) {
				p1->set_advantage_square(p2->get_covered_squares()[0]);
			}
		}
		// booleans to check if the first player's name was assigned to first turn correctly
		bool b_p1 = true, b_p2 = true;
		if (first_turn == p1->get_name()) {
			b_p1 = p1->set_had_first_turn(true);
		}
		else {
			b_p2 = p2->set_had_first_turn(true);
		}
		if (!b_p1 || !b_p2) {
			cerr << "Error while setting first turn identifier." << endl;
			exit(1);
		}

		Game saved_game(p1, p2, next_turn, index, dice_rolls, play_mode);
		saved_game.start_game();
		saved_game.end_game();
		return true;
	}
	else {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return false;
	}
}