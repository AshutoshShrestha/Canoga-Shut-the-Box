#include "BoardView.h"
#include "Board.h"
#include <iostream>
#include <string>

/* *********************************************************************
Function Name: BoardView
Purpose: To construct a BoardView object
Parameters: 
			player1, player2, two Player pointer variables. They hold the 
			memory locations of the two players playing in a round in a 
			game.
Return Value: a BoardView object
Algorithm:
			1) Set the player pointers as the parameters passed
Assistance Received: none
********************************************************************* */
BoardView::BoardView(Player * player1, Player * player2) {
	this->player1 = player1;
	this->player2 = player2;
}

// program will crash if default constructor is called because player1 and player2 won't have been initialized
/* *********************************************************************
Function Name: BoardView
Purpose: To construct a default BoardView object (program will crash if called
because players will have been initiailzed with this constructor)
Parameters: none
Return Value: a BoardView object
Algorithm:
Assistance Received: none
********************************************************************* */
BoardView::BoardView() {}

/* *********************************************************************
Function Name: ~BoardView
Purpose: To destroy/cleanup a BoardView object
Parameters: none
Return Value: a BoardView object
Algorithm:
Assistance Received: none
********************************************************************* */
BoardView::~BoardView() {}

/* *********************************************************************
Function Name: display_boards
Purpose: To display the board states of both players along with their names
Parameters: none
Return Value: none (void)
Algorithm:
			1) Print the game headers.
			2) Print each player's name and score in two consecutive lines.
			3) Print the name of the player who played the first turn.
Assistance Received: none
********************************************************************* */
void BoardView::display_boards() {
	std::cout << "--------------------------------------C--A--N--O--G--A-------------------------------------" << std::endl;
	std::cout << "===========================================================================================" << std::endl;
	std::cout << "||    ";
	player1->show_board();
	std::cout << "    ||";
	std::cout << "	" << player1->get_name() << "'s board" << "  ||||  First Turn played by -> ";
	if (player1->had_first_turn()) std::cout << player1->get_name() << std::endl;
	else if (player2->had_first_turn()) std::cout << player2->get_name() << std::endl;

	std::cout << "||    ";
	player2->show_board();
	std::cout << "    ||";
	std::cout << "	" << player2->get_name() << "'s board" << std::endl;

	std::cout << "===========================================================================================" << std::endl;
}

/* *********************************************************************
Function Name: display_scores
Purpose: To display each player's scores
Parameters:
			winner, a string passed by value. It holds the name of the
			winner of the round.
			winning_score, an integer passed by value. It holds the score
			of the winner of the round for that round only.
Return Value: none (void)
Algorithm:
			1) Print the winner of the round and the winning score.
			2) Print the total scores of both players.
Assistance Received: none
********************************************************************* */
void BoardView::display_scores(std::string winner, int winning_score) {
	if (winner != "" && winning_score > 0) {
		std::cout << "Winner of this round: " << winner << std::endl;
		std::cout << winner << " has gained " << winning_score << " points this round." << std::endl;
		std::cout << "===========================================================================================" << std::endl;
		std::cout << "----------------------------------------Total Scores---------------------------------------" << std::endl;
		std::cout << "		" << player1->get_name() << ": " << player1->get_score() << std::endl;
		std::cout << "		" << player2->get_name() << ": " << player2->get_score() << std::endl;
		std::cout << "===========================================================================================" << std::endl;
	}
	else {
		std::cout << "Winner was not decided." << std::endl;
	}
}
