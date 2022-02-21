#pragma once

#include "Player.h"
#include <vector>
#include <string>

class Human : public Player {
public:
	// default constructor 
	Human();

	// parameterized constructor
	Human(std::string);

	// destructor
	~Human();

	// selector
	inline std::string get_name() const { return name; }

	std::vector<int> choose_a_move(int, int, bool);
	bool roll_one();

private:
	std::string name;
};