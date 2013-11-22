//System libraries
#include <iostream>
#include <iomanip>
#include <fstream>

//Author's libraries
#include "mines.h"

using namespace std;

//No global constants

/*******************************************************************
** Function: Constructor for class Minesweeper
** Description: Takes in values for x, y, and p and applies those to
**	priBoard and seeBoard
*******************************************************************/
Minesweeper::Minesweeper(int a, int b, int g, int f, int t, bool oFile){
	x = new int;
	*x = a;
	y = new int;
	*y = b;
	p = g;
	opFi = oFile;
	flagged = f;
	totFl = t;
	//one dimensional arrays initialized and treated as 2-D arrays
    priBoard = new int[a*b];
	seeBoard = new int[a*b];
}
/*******************************************************************
** Function: Copy constructor for class Minesweeper
** Description: Copies priBoard and seeBoard
*******************************************************************/
Minesweeper::Minesweeper(const Minesweeper &obj){
	priBoard = new int[*x**y];
	*priBoard = *obj.priBoard;
	seeBoard = new int [*x**y];
	*seeBoard = *obj.seeBoard;
}
/*******************************************************************
** Function: Destructor for class Minesweeper
** Description: Deallocates memory
*******************************************************************/
Minesweeper::~Minesweeper(){
	delete [] priBoard;
	delete [] seeBoard;
}
/*******************************************************************
** Function: setVis
** Description: Initializes the values in the board available to 
**	the user. All values are set to zero.
*******************************************************************/
void Minesweeper::setVis(){
	//Rows and columns numbered first to give user a coordinate system
	if (opFi == true){
		visFile(3);
		return;
	}
	for (int m = 0; m < *x; m++){
		int n = 0;
		seeBoard[m**x + n] = m;
	}
	for (int n = 0; n < *x; n++){
		int m = 0;
		seeBoard[m**x + n] = n;
	}
	//Board values set to -1
	for (int i = 1; i < *x; i++){
		for (int j = 1; j < *y; j++){
		seeBoard[i**x + j] = -1;
		}
	}
	return;
}
/*******************************************************************
** Function: visFile
** Description: Takes over the role of setVis when data from a 
**	saved game is used. This function pulls data from the save file
**	and applies it to the board available to the user.
*******************************************************************/
void Minesweeper::visFile(int p){
	//open file
	fstream svGame;
	float q;
	//confirm that file opened properly
	svGame.open ("saved.txt", fstream::in | fstream::out);
	if (!svGame.is_open()){
		std::cout << "Error loading save game file." << endl;
		exit(EXIT_FAILURE);
	}
	//move cursor past first five data points
	svGame >> p >> p >> p >> p >> p;
	svGame >> q;
	//copy appropriate file data to seeBoard
	for (int i = 0; i < *x; i++){
		for (int j = 0; j < *y; j++){
			svGame >> seeBoard[i**x + j];
		}
	} 
	//close file
	svGame.close();
	//back to setVis
	return;
}
/*******************************************************************
** Function: setMines
** Description: Sets the positions of the mines in the board that is
**	unavailable to the user
*******************************************************************/
int Minesweeper::setMines (){
	if (opFi == true){
		numMines = minFile(*x**y+5);
		return numMines;
	}
	//initializes the private board to an array of zeros
	for (int i = 0; i < *x; i++){
		for (int j = 0; j < *y; j++){
			priBoard[i**x + j] = 0;
		}
	}
	//sets the number of mines based on p
	numMines = (*x-1) * (*y-1) * float(p)/100;
	//if the percent of the board filled with mines is too low, the function returns 0
	if (numMines == 0){
		return numMines;
	}
	//the random number seed is used to put mines on the board
	for ( int a = 0; a < numMines; a++ ){
		//x and y coordinates each chosen randomly and applied to board
		int rX = (rand() % (*x-1))+1;
		int rY = (rand() % (*y-1))+1;
		//if that random coordinate has already been used, the counter is decremented and the function tries again
		if (priBoard[rX**x + rY] == 9)
			--a;
		else priBoard[rX**x + rY] = 9;
	}

	for (int i = 0; i < *x; i++){
		for (int j = 0; j < *y; j++){
			/* The following section of code checks each coordinate on the board for
			** a mine and increments the positions around each mine*/

			if (priBoard [i**x + j] == 9){
				if (priBoard [(i+1)**x + (j+1)] >= 0 && priBoard [(i+1)**x + (j+1)] < 9){
					++priBoard [(i+1)**x + (j+1)];		//1
				}
				if (priBoard [(i+1)**x + j] >= 0 && priBoard [(i+1)**x + j] < 9){
					++priBoard [(i+1)**x + j];			//2
				}
				if (priBoard [i**x + (j+1)] >= 0 && priBoard [i**x + (j+1)] < 9){
					++priBoard [i**x + (j+1)];			//3
				}
				if (priBoard [(i+1)**x + (j-1)] >= 0 && priBoard [(i+1)**x + (j-1)] < 9){
					++priBoard [(i+1)**x + (j-1)];		//4
				}
				if (priBoard [(i-1)**x + (j+1)] >= 0 && priBoard [(i-1)**x + (j+1)] < 9){
					++priBoard [(i-1)**x + (j+1)];		//5
				}
				if (priBoard [(i-1)**x + (j-1)] >= 0 && priBoard [(i-1)**x + (j-1)] < 9){
					++priBoard [(i-1)**x + (j-1)];		//6
				}
				if (priBoard [(i-1)**x + j] >= 0 && priBoard [(i-1)**x + j] < 9){
					++priBoard [(i-1)**x + j];			//7
				}
				if (priBoard [i**x + (j-1)] >= 0 && priBoard [i**x + (j-1)] < 9){
					++priBoard [i**x + (j-1)];			//8
				}
			}
		}
	}	
	return numMines;
}
/*******************************************************************
** Function: mineFile
** Description: Pulls data from the saved game file, taking the role
**	of setMines. This function applies data to the hidden board.
*******************************************************************/
float Minesweeper::minFile (int w){
	//declare variables
	int z;
	int r;
	float q;
	fstream svGame;
	//open file
	svGame.open ("saved.txt", fstream::in | fstream::out);
	//confirm that file has opened successfully
	if (!svGame.is_open()){
		std::cout << "Error loading save game file." << endl;
		exit(EXIT_FAILURE);
	}
	//move cursor to appropriate location in file
	svGame >> z >> z >> z;
	svGame >> r >> r;
	svGame >> q;
	for (int u = 0; u < *x**y; u++) svGame >> w;
	//apply data to the hidden board
	for (int i = 0; i < *x; i++){
		for (int j = 0; j < *y; j++){
			svGame >> priBoard[i**x + j];
		}
	} 
	//close file
	svGame.close();
	//return number of mines to setMines
	return z;
}
/*******************************************************************
** Function: upBoard
** Description: Updates the board based on the coordinate entered by
**	the user - checks for a mine at that point and updates the board
**	when a mine is not hit.
*******************************************************************/
int Minesweeper::upBoard (int a, int b){
	//declare variables
	int g = 0;
	int ret = 0;		//tells main whether the user has finished the game
	
	//checks if the coordinate chosen by the user is at a mine
	if (priBoard[a**x +b] == 9) {
		cout << "Ouch! You hit a mine!" << endl;
		//If the user hit a mine, they lose and the rest of the board and mines are displayed
		for (int i = 1; i < *x; i++){
			for (int j = 1; j < *y; j++){
				//Mines are '9' in the code, but displayed as "x" to the user
				if ( priBoard[i**x + j] == 9 ){
					cout << "x  ";
				}
				else 
					cout << priBoard[i**x + j] << "  ";
			}
			cout << endl;
		}
		//game over
		ret = 2;
	}
	//if the coordinate is not a mine, its value is made available to the user
	else seeBoard[a**x + b] = priBoard [a**x +b];

	/* If the coordinate's value is zero, the spaces are filled in around it until it reaches 
	** nonzero values (located adjacent to mines). The code moves from left to right and down the 
	** rows from the coordinate entered by the user until it reaches a zero. It then updates all 
	** of the relevant spaces past the new zero on the board. The second loop starts checking for 
	** zeros at the coordinate entered by the user and moves right to left and up the rows so 
	** that it can update all of the spaces around that coordinate not covered in the first loop. 
	** As there are 8 spaces around any inter-board coordinate, and each one is accounted for and 
	** noted below*/

	if (seeBoard[a**x + b] == 0){
		for (int k = a; k < *x; k++){
			for (int l = b; l < *y; l++){
				if (seeBoard[k**x + l] == 0){
					if (k < 10 && l < 10)	seeBoard [(k+1)**x + (l+1)] = priBoard [(k+1)**x + (l+1)];		//1
					if (k < 10)				seeBoard [(k+1)**x + l] = priBoard [(k+1)**x + l];				//2
					if (l < 10)				seeBoard [k**x + (l+1)] = priBoard [k**x + (l+1)];				//3
					if (l > 1 && k < 10)	seeBoard [(k+1)**x + (l-1)] = priBoard [(k+1)**x + (l-1)];		//4
					if (k > 1 && l < 10)	seeBoard [(k-1)**x + (l+1)] = priBoard [(k-1)**x + (l+1)];		//5
					if (k > 1 && l > 1)		seeBoard [(k-1)**x + (l-1)] = priBoard [(k-1)**x + (l-1)];		//6
					if (k > 1)				seeBoard [(k-1)**x + l] = priBoard [(k-1)**x + l];				//7
					if (l > 1)				seeBoard [k**x + (l-1)] = priBoard [k**x + (l-1)];				//8
					else					seeBoard [k**x + l] = priBoard [k**x + l];
				}
			}
		}
		for (int m = a; m > 0; m--){
			for (int n = b; n > 0; n--){
				if (seeBoard[m**x + n] == 0){
					if (m < 10 && n < 10)	seeBoard [(m+1)**x + (n+1)] = priBoard [(m+1)**x + (n+1)];		//1
					if (m < 10)				seeBoard [(m+1)**x + n] = priBoard [(m+1)**x + n];				//2
					if (n < 10)				seeBoard [m**x + (n+1)] = priBoard [m**x + (n+1)];				//3
					if (n > 1 && m < 10)	seeBoard [(m+1)**x + (n-1)] = priBoard [(m+1)**x + (n-1)];		//4
					if (m > 1 && n < 10)	seeBoard [(m-1)**x + (n+1)] = priBoard [(m-1)**x + (n+1)];		//5
					if (m > 1 && n > 1)		seeBoard [(m-1)**x + (n-1)] = priBoard [(m-1)**x + (n-1)];		//6
					if (m > 1)				seeBoard [(m-1)**x + n] = priBoard [(m-1)**x + n];				//7
					if (n > 1)				seeBoard [m**x + (n-1)] = priBoard [m**x + (n-1)];				//8
					else					seeBoard [m**x + n] = priBoard [m**x + n];
				}
			}
		}
	}
	//counts the spaces on the board still unavailable to the user
	for (int k = 1; k < *x; k++){
		for (int l = 1; l < *y; l++){
			if (seeBoard[k**x + l] == -1)
				g++;
		}
	}
	//if the user reaches only mines on the board
	if ( g <= numMines ){
		cout << "You won!" << endl;
		ret = 1;
	}
	//return game completion status
	return ret;
}
/*******************************************************************
** Function: flags
** Description: Allows user to choose whether they want to add or
**	remove a flag - gets this code out of main
*******************************************************************/
int Minesweeper::flags(){
	//variables
	int flag, x, y;
	int st = 0;
	cout << "Would you like to (1) add or (2) remove a flag?" << endl;
	cin >> flag;
	//get coordinates and pass to appropriate funtion
	if (flag == 1){
		cout << "Please enter a coordinate you wish to flag." << endl;
		cout << "x: ";
		cin >> x;
		cout << "y: ";
		cin >> y;
		int st = flagMin(y,x);
	}
	else if (flag == 2){
		cout << "Please enter the coordinates of a flag you wish to remove." << endl;
		cout << "x: ";
		cin >> x;
		cout << "y: ";
		cin >> y;
		remFlag(y,x);
	}
	return st;
}

/*******************************************************************
** Function: flagMin
** Description: Allows the user to flag a space on the board as a
**	mine. 
*******************************************************************/
int Minesweeper::flagMin(int a, int b){
	int gamSt = 0;
	//Check that space hasn't already been touched by user
	if (seeBoard[a**x+b] == -1){
		seeBoard[a**x+b] = 42;		//42 represents a flag
		++totFl;
		if (priBoard[a**x+b] == 9){
			++flagged;
			if (totFl == flagged && flagged == numMines){
				cout << "All the mines have been flagged. You won!";
				gamSt = true;
			}
		}
	}
	else
		cout << "Invalid input: this space has already been cleared." << endl;
	return gamSt;
}
/*******************************************************************
** Function: remFlag
** Description: Removes a user-added flag from the board
*******************************************************************/
void Minesweeper::remFlag(int a, int b){
	//confirm that there is actually a flag at the given coordinate
	if (seeBoard[a**x+b] == 42){
		seeBoard[a**x+b] = -1;		//-1 represents untouched coordinate
		--totFl;
		if (priBoard[a**x+b] == 9){
			--flagged;
		}
	}
	else
		cout << "Invalid input: this space does not contain a flag." << endl;
	return;
}
/*******************************************************************
** Function: printBoard
** Description: Prints the board that the user sees throughout the
**	game. 
*******************************************************************/
void Minesweeper::printBoard (){

	for (int t = 0; t < *x; t++){
		for (int w = 0; w < *y; w++){
			cout << setw(3);
			//values not yet available to the user have been initialized to -1,
			//but the user sees these spaces as '.'
			if (seeBoard[t**x + w] == -1) cout << ".";
			else if (seeBoard[t**x + w] == 42) cout << "F";
			else cout << seeBoard[t**x + w];
		}
		cout << endl;
	}
	return;
}
/*******************************************************************
** Function: saveGame
** Description: Saves hidden and available board data to an external 
**	text file, allows user to return to a game after closing the 
**	application.
*******************************************************************/
void Minesweeper::saveGame(float start){
	//variables
	fstream sveGme;

	clock_t end = clock();
	clock_t ticks = end - start;
	float dur = ticks / (float) CLOCKS_PER_SEC;

	//open file and remove any existing contents
	sveGme.open ("saved.txt", fstream::out, fstream::trunc);
	//confirm that file has opened properly
	if (!sveGme.is_open()){
		cout << "Error loading save game file. Gameplay not saved." << endl;
	}
	//save board dimensions and the number of mines
	sveGme << *x << " " << *y << " " << numMines << endl;
	sveGme << dur << endl;
	//save the board available to the user
	for (int i = 0; i < *x; i++){
		for (int j = 0; j < *y; j++){
			sveGme << seeBoard[i**x + j] << " ";
		}
		sveGme << endl;
	}
	//save the hidden board
	for (int k = 0; k < *x; k++){
		for (int l = 0; l < *y; l++){
			sveGme << priBoard[k**x + l] << " ";
		}
		sveGme << endl;
	}
	//close file
	sveGme.close();
	//return to gamePly (located in main.cpp)
	return;
}