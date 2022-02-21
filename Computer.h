#pragma once

#include "Player.h"
#include <string>
#include <vector>
#include "GameUtils.h"

using namespace std;

class Computer : public Player {
public:
	// default constructor
	Computer();

	// parameterized constructor
	Computer(Player *);

	Computer(bool, Player *);

	// destructor
	~Computer();

	vector<int> choose_a_move(int, int, bool);
	bool roll_one();

	// mutator
	bool set_opponent(Player *);

	// selector
	inline string get_name() const { return "Computer"; }

private:
	vector<int>* find_best_move(vector<vector<int>*>, vector<int> *, int);
	bool cover_or_uncover();
	bool helper;
	Player * opponent;
};