#pragma once

#include <vector>

class GameUtils {
public:
	GameUtils();
	~GameUtils();
	bool check_availability(std::vector<int>, std::vector<int>, std::vector<int>);
	bool roll_one_available(std::vector<int>);
	bool check_unique(std::vector<int>);
	void findCombinationsUtil(std::vector<std::vector<int> *> *, int[], int, int, int);
	void findCombinations(std::vector<std::vector<int> *> *, int);
	int check_valid(int, int, std::vector<std::vector<int>*>, std::vector<std::vector<int>*>);

	std::vector<std::vector<int>*> all_possible_moves(int);
	std::vector<std::vector<int>*> all_available_cover_moves(std::vector<std::vector<int>*>, std::vector<int>);
	std::vector<std::vector<int>*> all_available_uncover_moves(std::vector<std::vector<int>*>, std::vector<int>, int);
};