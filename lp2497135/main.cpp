/*******************************************************************
** Program: Minesweeper
** Assignment: CSC 17A Project 1
** Author: Lauren Perry
** Date Due: October 25, 2013
** Description: This program mimics the classic version of the 
**	puzzle game, Minesweeper. Users use number clues to look for
**	all of the mines hidden throughout the board.
*******************************************************************/

//System libraries
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>

//Author's libraries
#include "mines.h"

//Function declarations
float gamePly(int, int, int, bool, clock_t);
float getSave();
void setHiSc(float);
void getHiSc();

using namespace std;

//No global constants

//Execution starts here
int main(){
	//Create random number seed
	srand(static_cast<unsigned int>(time(0)));	
	//Declare variables
	bool runAgain = true;
	char saved[5];
	int h = 0, v = 0, m = 0;
	float time;
	cout << "Welcome to Minesweeper.\n" << endl;
	//Open saved game file
	fstream svGame;
	svGame.open ("saved.txt", fstream::in | fstream::out);
	//Confirm file opened successfully
	if (!svGame.is_open()){
		std::cout << "Error loading save game file. Continued gameplay will not be saved." << endl;
	}
	svGame.seekg(0,ios::end);	//get final character in file
	int emt = svGame.tellg();	//save final character
	svGame.close();				//close file
	//if the file has data in it
	if (emt != 0){
		cout << "Would you like to continue your last saved game?" << "\nPlease enter 'yes' or 'no'." << endl;
		cin >> saved;
		if (saved[0] == 'y' || saved[0] == 'Y'){
			//call function to get saved game
			time = getSave();
		}
	}

	while (runAgain == true) {
		//variables declared
		bool s = true;
		int opt;

		cout << "\nWould you like to set up your own board or use the preset size?" << endl
			<< "Press '1' for preset, '2' to set up your own board," << endl << "'3' to view high scores, and '0' to quit." << endl;
		cin >> opt;
		do{
			if (opt == 0){
				runAgain = false;
				cout << "\nThanks for playing!" << endl;
				return 0;
			}
			if (opt == 2){
				cout << "What would you like the size of your board to be?" << endl << "(Sizes range from 5-25.)" << endl;
				cin >> v;
				//check for valid size input
				if ( v < 5 || v > 25 ){
					cout << "Invalid size" << endl;
					s = false;
				}
				//create board of dimensions size x size
				h = v;
				cout << "What percentage of the board would you like to be mines?" << endl << "(12-20% is a good range.)" << endl;
				cin >> m;
			}
			if (opt == 3){
				getHiSc();
				break;
			}
			else if (opt == 1) {
				//default board size
				h = 10;
				v = 10;
				m = 12;
			}
			//call gameplay function (where false indicates that a saved game is not being opened)
			clock_t start = clock();
			time = gamePly (h, v, m, false, start);
		}while (s == false);
	}
	//execution ends here
	cin.ignore();
	cin.get();
	return 0;
}
/*******************************************************************
** Function: gamePly
** Description: Holds the game part of the application - the user
**	interacts primarily with this function. User enters coordinates
**	and has the option to save their game here.
*******************************************************************/
float gamePly(int h, int v, int m, bool oFile, clock_t start){
	//Declare variables
	int q = 0;
	int a, b;
	float duration = 300.0;
	//Create and set up instance of class Minesweeper
	Minesweeper defBoard(h+1,v+1,m,oFile);
	int o = defBoard.setMines();		
	defBoard.setVis();
	//Check that the board actually has mines on it
	if (o != 0){
		cout << "\nThere are " << o << " mines on the board. Try to find them all!\n" << endl;
	}
	else if (o == 0){
		cout <<"\nOops! Your percentage/board size combination was unsuccessful. There are no mines on this board!";
	}
	//while user has not hit a mine
	while (q == 0 && o > 0) {
		//print the public board (board available to the user throughout gameplay)
		defBoard.printBoard();
		//get mine coordinate from user
		cout << "Pick a coordinate to check for a mine" << endl
			<< "enter 0 to add or remove a flag where a mine might be, " << endl
			<< "or enter -1 to quit and save your game." << endl;
		cout << "x: ";
		cin >> a;
		//user quits and saves
		if (a == -1){
			//call function to save the game
			defBoard.saveGame(start);
			cout << "\nThanks for playing!" << endl;
			//head back to main
			return 300;
		}
		//go to flag functions and continue with next iteration of while loop
		if (a == 0){
			defBoard.flags();
			continue;
		}
		cout << "y: ";
		cin >> b;
		//user decides to quit at second part of coordinate, separated to make application more user-friendly
		if (b == -1){
			//call function to save game
			defBoard.saveGame(start);
			cout << "\nThanks for playing!" << endl;
			//head back to main
			return 300;
		}
		//flags & next while loop
		if (b == 0){
			defBoard.flags();
			continue;
		}
		cout << endl;
		//update board and check if user has hit a mine
		q = defBoard.upBoard(b, a);
		//game status: won, save possible high scores
		if ( q == 1 ){
			clock_t end = clock();
			clock_t ticks = end - start;
			duration = ticks / (float) CLOCKS_PER_SEC;
			setHiSc(duration);
		}
	}
	//head back to main
	return duration;
}
/*******************************************************************
** Function: getSave
** Description: Pulls the saved data necessary to declare a class 
**	variable of type Minesweeper and passes this data to the 
**	gameplay function
*******************************************************************/
float getSave(){
	//Declare variables
	int h, v, m;
	float t;
	fstream svGame;
	//Open save game file
	svGame.open ("saved.txt", fstream::in);
	//Confirm that file has opened properly
	if (!svGame.is_open()){
		std::cout << "Error loading save game file." << endl;
		exit(EXIT_FAILURE);
	}
	//Grab data
	svGame >> h >> v >> m;
	svGame >> t;
	//Close file
	svGame.close();
	//start timer
	clock_t start = clock();
	//Pass values to gameplay function ("true" indicates that data has been pulled from a file)
	float duration = gamePly(h-1, v-1, m, true, start);
	//Return to main (current time plus time from saved game, if applicable)
	return duration+t;
}
/*******************************************************************
** Function: setHiSc
** Description: Pulls high score data and compares user time to it
**	before inserting updated data back into a save file.
*******************************************************************/
void setHiSc(float time){
	//variables
	fstream hiSc;
	float nxtLn;
	float highs[10];
	bool x = true;
	int cond = 0;	//default condition
	//open high score file and confirm that it opened properly
	hiSc.open ("hiScore.txt", fstream::in);
	if (!hiSc.is_open()){
		std::cout << "Error loading high scores file." << endl;
		exit(EXIT_FAILURE);
	}
	//pull high score data and add new data, if applicable - array is kept to 10 spaces to minimize high score data
	for (int i = 0; i < 10; i++){
		hiSc >> nxtLn;
		//x is true while saved times are better than user's newest score 
		if (x == true && time < nxtLn){
			highs[i] = time;
			cond = 1;
			x = false;
		}
		//if no new data is added
		if (cond = 0 && i < 10) 
			highs[i] = nxtLn;
		//if new data is added i is incremented further to fit new data
		else if (cond = 1 && i < 9) 
			highs[i+1] = nxtLn;
		else break;
	}
	//close file
	hiSc.close();
	//reopen file for output and delete its contents
	hiSc.open ("hiScore.txt", fstream::out, fstream::trunc);
	//confirm proper opening
	if (!hiSc.is_open()){
		std::cout << "Error loading high scores file." << endl;
		exit(EXIT_FAILURE);
	}
	//insert updated high score array into file
	for (int a = 0; a < 10; a++){
		hiSc << highs[a] << " ";
	}
	//close file
	hiSc.close();

	if (cond = 1){
		cout << "New high score!";
		getHiSc();
	}
	return;
}
/*******************************************************************
** Function: getHiSc
** Description: Pulls high score data and prints it
*******************************************************************/
void getHiSc(){
	//variables
	char resp[4];
	float high;
	fstream scores;
	//open file
	scores.open ("hiScore.txt", fstream::in);
	//confirm file opening
	if (!scores.is_open()){
		std::cout << "Error loading high scores file." << endl;
		exit(EXIT_FAILURE);
	}
	//display high scores
	cout << "Best times (seconds):" << endl;
	for (int i = 0; i < 10; i++){
		scores >> high;
		cout << setw(4);
		cout << i+1 << ") " << high << endl;
	}
	//close file
	scores.close();
	return;
}