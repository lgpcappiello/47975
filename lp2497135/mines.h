#ifndef MINES_H__
#define MINES_H__

//Minesweeper class created
class Minesweeper {
//initialize private variables and functions called only by member functions
private:
	int *x, *y, p, numMines;
	bool opFi;
	int *priBoard;
	int *seeBoard;
	void showMines();
	void visFile(int);
	float minFile(int);
	int flagged;
	int totFl;
	int flagMin(int, int);
	void remFlag(int, int);

public:
//initialize constructor, copy constructor, and destructor
	Minesweeper (int, int, int, int, int, bool);
	Minesweeper (const Minesweeper &obj);
	~Minesweeper();
//initialize functions called by main
	void setVis();
	int setMines();
	int upBoard(int, int);
	int flags();
	void printBoard();
	void contGame();
	void saveGame(float);
};

#endif