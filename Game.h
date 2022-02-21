#include "Player.h"
#include "Round.h"
#include <queue>
#include <vector>

class Game {
public:
	// default constructor
	Game();

	// parameterized constructor
	Game(int);
	Game(Player *, Player *, string, int, queue<vector<int>> *, int);

	// destructor
	~Game();

	void start_game();
	void initialize(Player *, Player *);
	void end_game();

	// selector
	inline int getMode() const { return mode; }

private:
	// 1 - player vs player
	// 2 - player vs computer
	int mode, squares;
	Player * player1, *player2;
	bool is_new_game;
	string next_turn;
	Round *current_round;
	std::queue<std::vector<int>> * loaded_dice_rolls;
};