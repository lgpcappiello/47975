#ifndef MINES_H__
#define MINES_H__

//Minesweeper class created
class Minesweeper {
//initialize private variables and functions called only by member functions
private:
	int x, y;
	int numMines; 
	int goodMrk;
	bool opFi;
	int *priBoard;
	int *seeBoard;
	void showMines();
	void visFile();
	int minFile();
	void flags();
	void marks();
	void flagMin(int, int);
	void remFlag(int, int);
	void markMin(int, int);
	void remMark(int, int);

public:
//initialize constructor, copy constructor, and destructor
	Minesweeper (int, int, int, int, bool);
	Minesweeper (const Minesweeper &obj);
	~Minesweeper();
//initialize functions called by main
	void setNumM(int, bool);
	int* getPri();
	int* getPub();
	int getDim();
	int getNum();
	int getMrk();
	void setVis();
	void setMines();
	int getX(int);
	int getY();
	int upBoard(int, int);
	void flgORmrk();
	void printBoard();
	void contGame();
};

#endif