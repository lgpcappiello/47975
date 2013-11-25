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
float gamePly(int, int, int, int, bool, clock_t);
float openSv();
float getSave();
void setHiSc(float);
void getHiSc();
clock_t begClck();
float endClck(clock_t);

using namespace std;

//No global constants

//Execution starts here
int main(){
	//Create random number seed
	srand(static_cast<unsigned int>(time(0)));	
	//Declare variables
	bool runAgain = true;
	int h = 0, v = 0, m = 0;	//horizontal, vertical, mines
	float time;

	cout << "Welcome to Minesweeper.\n" << endl;
	time = openSv();
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
				getHiSc();
				continue;
			}
			//check for bad input
			else {
				cout << "Invalid response" << endl;
				continue;
			}
			//start timer
			clock_t start = begClck();
			//call gameplay function (where 0 && false indicate that a saved game is not being opened)
			time = gamePly (h, v, m, 0, false, start);
		}while (s == false);
	}
	cin.ignore();
	cin.get();
	//execution ends here
	return 0;
}
/*******************************************************************
** Function: gamePly
** Description: Holds the game part of the application - the user
**	interacts primarily with this function. User enters coordinates
**	and has the option to save their game here.
*******************************************************************/
float gamePly(int h, int v, int m, int g, bool oFile, clock_t start){
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
		cout << "Pick a coordinate to check for a mine" << endl
			<< "enter 0 to (un)flag or (un)mark a potential mine " << endl
			<< "enter -1 to quit and save your game," << endl
			<< "enter -2 to quit without saving." << endl;
		//get mine coordinate from user
		a = defBoard.getX(2);
		//user quits game
		if (a == -1 || a == -2){
			//quit with save
			if (a == -1) defBoard.saveGame(start);
			cout << "\nThanks for playing!" << endl;
			//head back to main
			return 300;
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
			duration = endClck(start);
		}
	}
	//head back to main
	return duration;
}
/*******************************************************************
** Function: openSv
** Description: Checks for a saved game in saved.txt and asks the
**	user if they want to continue their game or start a new one.
*******************************************************************/
float openSv(){
	char saved[5];
	float time;
	bool input = true;
	//Open saved game file
	fstream svGame;
	svGame.open ("saved.txt", fstream::in | fstream::out);
	//Confirm file open
	if (!svGame.is_open()){
		std::cout << "Error loading save game file. Continued gameplay will not be saved." << endl;
	}
	svGame.seekg(0,ios::end);	//get final character in file
	int emt = (int)svGame.tellg();	//save final character
	svGame.close();				//close file
	//if the file has data in it
	do{
		if (emt != 0){
			cout << "Would you like to continue your last saved game?" << "\nPlease enter 'yes' or 'no'." << endl;
			cin >> saved;
			if (saved[0] == 'y' || saved[0] == 'Y'){
				//call function to get saved game
				time = getSave();
			}
			else if (saved[0] == 'n' || saved[0] == 'N'){
				time = 0;
			}
			else{
				cout << "Invalid input." << endl;
				input = false;
			}
		}
	}while(input == false);
	//send current time data back
	return time;
}
/*******************************************************************
** Function: getSave
** Description: Pulls the saved data necessary to declare a class 
**	variable of type Minesweeper and passes this data to the 
**	gameplay function
*******************************************************************/
float getSave(){
	//Declare variables
	int h, v, m, g;
	float t;
	fstream svGame;
	//Open save game file
	svGame.open ("saved.txt", fstream::in);
	//Confirm file open
	if (!svGame.is_open()){
		std::cout << "Error loading save game file." << endl;
		exit(EXIT_FAILURE);
	}
	//Grab data
	svGame >> h >> v >> m >> g;
	svGame >> t;
	//Close file
	svGame.close();
	//start timer
	clock_t start = begClck();
	//Pass values to gameplay function ("true" indicates that data has been pulled from a file)
	float duration = gamePly(h-1, v-1, m, g, true, start);
	//Return current time plus time from saved game, if applicable, to main
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

	//Give user current game data for comparison
	cout << "Your time was: " << time << " seconds." << endl;

	//open high score file and confirm file open
	hiSc.open ("hiScore.txt", fstream::in);
	if (!hiSc.is_open()){
		std::cout << "Error loading high scores file." << endl;
		exit(EXIT_FAILURE);
	}
	//pull high score data and add new data, if applicable
	//array is kept to 10 spaces to minimize number of scores saved
	for (int i = 0; i < 10; i++){
		hiSc >> nxtLn;
		//x is true while saved times are better (lower) than user's newest score 
		if (x == true && time < nxtLn){
			highs[i] = time;
			cond = 1;
			x = false;
		}
		//if no new data is added
		if (cond = 0 && i < 10) 
			highs[i] = nxtLn;
		//if new data is added, i is incremented to fit new data
		else if (cond = 1 && i < 9) 
			highs[i+1] = nxtLn;
		else break;
	}
	//close file
	hiSc.close();
	//reopen file for output and delete current contents
	hiSc.open ("hiScore.txt", fstream::out, fstream::trunc);
	//confirm open
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
	//new data added
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
/*******************************************************************
** Function: begClck
** Description: Starts clock time
*******************************************************************/
clock_t begClck(){
	//start timer
	clock_t start = clock();
	return start;
}
/*******************************************************************
** Function: endClck
** Description: Stops clock time and returns uesr time data
*******************************************************************/
float endClck(clock_t start){
	//end timer
	clock_t end = clock();
	//calculate total time
	clock_t ticks = end - start;
	//convert to seconds
	float duration = ticks / (float) CLOCKS_PER_SEC;
	//send data to high score function and return
	setHiSc(duration);
	return duration;
}