/*******************************************************************
** Program: Minesweeper
** Assignment: CSC 17A Project 1
** Author: Lauren Perry
** Date Due: October 25, 2013
** Description: This program mimics the classic version of the 
**	puzzle game, Minesweeper. Users use number clues to look for
**	all of the mines hidden throughout the board.
** Total lines of code: 1067
*******************************************************************/

//System libraries
#include <iostream>
#include <cstdlib>
#include <ctime>
//#include <fstream>
//#include <iomanip>

//Author's libraries
#include "mines.h"
#include "saved.h"

//Function declarations
void gamePly(int, int, int, int, bool, SavedData);

using namespace std;

//No global constants

//Execution starts here
int main(){
	//Create random number seed
	srand(static_cast<unsigned int>(time(0)));	
	//Declare variables
	bool runAgain = true;
	int h = 0, v = 0, m = 0, g = 0;	//horizontal, vertical, mines
	float *info;
	SavedData data;

	cout << "Welcome to Minesweeper.\n" << endl;
	info = data.openSv();
	if (info != 0){
		h = *(info + 0)-1;
		v = *(info + 1)-1;
		m = *(info + 2);
		g = *(info + 3);
		if (h == 10 && v == 10 && m == 12) data.setStatus(true);
		gamePly(h, v, m, g, true, data);
	}

	while (runAgain == true) {
		//more variables
		bool s = true;
		int opt;
		cout << "\nWould you like to set up your own board or use the preset size?" << endl
			<< "Press '1' for preset, '2' to set up your own board," << endl << "'3' to view high scores, and '0' to quit." << endl;
		cin >> opt;
		do{
			//quit
			if (opt == 0){
				runAgain = false;
				cout << "\nThanks for playing!" << endl;
				return 0;
			}
			//use default board (default sizes)
			else if (opt == 1) {
				h = 10;
				v = 10;
				m = 12;
				data.setStatus(true);
				cout << "There are 12 mines on the board. Try to find them all! " << endl;
			}
			//customize board
			else if (opt == 2){
				cout << "What would you like the size of your board to be?" << endl << "(Sizes range from 5-25.)" << endl;
				cin >> v;
				//check for valid size input
				if ( v < 5 || v > 25 ){
					cout << "Invalid size input." << endl;
					s = false;
					continue;
				}
				//create board of dimensions size x size
				h = v;
				cout << "What percentage of the board would you like to be mines?" << endl << "(12-20% is a good range.)" << endl;
				cin >> m;
				if ((int)(v*v*float(m)/100) == 0){
					cout << "Oops! There are no mines on this board." << endl;
					s = false;
					continue;
				}
				else{
					cout << "There are " << (int)(v*v*float(m)/100) << " mines on the board. Try to find them all! " << endl;
				}
			}
			//view high scores
			else if (opt == 3){
				data.getHiSc();
				continue;
			}
			//check for bad input
			else {
				cout << "Invalid response" << endl;
				continue;
			}
			data.begClck();
			//call gameplay function (where 0 && false indicate that a saved game is not being opened)
			gamePly (h, v, m, g, false, data);
		}while (s == false);
	}
	cin.ignore();
	cin.get();
	//execution ends here
	delete []info;
	return 0;
}
/*******************************************************************
** Function: gamePly
** Description: Holds the game part of the application - the user
**	interacts primarily with this function. User enters coordinates
**	and has the option to save their game here.
*******************************************************************/
void gamePly(int h, int v, int m, int g, bool oFile, SavedData data){
	//Declare variables
	int q = 0;
	int a, b;
	float duration = 300.0;
	//Create and set up instance of class Minesweeper
	Minesweeper defBoard(h+1,v+1,m,g,oFile);
	defBoard.setVis();
	defBoard.setMines();		
	//while user has not hit a mine
	while (q == 0) {
		//print the public board (board available to the user throughout gameplay)
		defBoard.printBoard();
		cout << "Time: " << data.getTime() << "s " << endl;
		cout << "Pick a coordinate to check for a mine" << endl
			<< "enter 0 to (un)flag or (un)mark a potential mine " << endl
			<< "enter -1 to quit and save your game," << endl
			<< "enter -2 to quit without saving." << endl;
		//get mine coordinate from user
		a = defBoard.getX(2);
		//user quits game
		if (a == -1 || a == -2){
			//quit with save
			if (a == -1){
				duration = data.endClck();
				data.saveGame(defBoard.getDim(), defBoard.getDim(), defBoard.getNum(), defBoard.getMrk(), 
					defBoard.getPri(), defBoard.getPub(), duration);
			}
			cout << "\nThanks for playing!" << endl;
			//head back to main
			return;
		}
		//go to flag functions and continue with next iteration of while loop
		else if (a == 0){
			defBoard.flgORmrk();
			//head to next iteration of loop
			continue;
		}
		b = defBoard.getY();
		//update board and check if user has hit a mine
		q = defBoard.upBoard(b, a);
		//game status: won
		if ( q == 1 ){
			duration = data.endClck();
			data.setHiSc(duration);
		}
	}
	//head back to main
	return;
}