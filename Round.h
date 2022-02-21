#pragma once

#include "Player.h"
#include "Computer.h"
#include <string>
#include <vector>
#include <queue>
#include "BoardView.h"

class Round {
public:
	// default constructor
	Round();

	// parameterized constructor
	Round(Player*, Player*);

	// destructor
	~Round();

	// mutators
	bool set_player_turns(Player *, Player *, int);
	void set_l_dice_rolls(queue<vector<int>> *);
	bool set_players(Player *, Player *);

	void start_round();
	bool end_round();
private:
	string firstPlayer, winner;
	Player * nextPlayer, *currentPlayer;
	Player * player1, *player2;
	BoardView * view;
	bool game_over;
	int squares, winning_score;

	// loaded dice rolls
	queue<vector<int>> *l_dice_rolls;

	void decide_first_player(int);
	void give_advantage(Player *, int, int);
	void show_menu();
	void save_game(bool, int);

	bool player_won(bool, int);
	void provide_help(int, int, int);
};