#include "Player.h"
#include <string>

class BoardView {
public:
	// parameterized constructor
	BoardView(Player *, Player *);

	// default constructor
	BoardView();

	// destructor
	~BoardView();

	void display_boards();
	void display_scores(std::string, int);

private:
	Player * player1, *player2;
};