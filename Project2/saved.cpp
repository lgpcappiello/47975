#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

#include "saved.h"

using namespace std;
/*******************************************************************
** Function: Constructor for class SavedData
*******************************************************************/
SavedData::SavedData(){
	info = new float[SIZE];
	save = false;
}
/*******************************************************************
** Function: Copy Constructor for class SavedData
*******************************************************************/
SavedData::SavedData(const SavedData &obj){
	info = new float[SIZE];
	*info = *obj.info;
}
/*******************************************************************
** Function: Destructor for class SavedData
*******************************************************************/
SavedData::~SavedData(){
	delete []info;
}
/*******************************************************************
** Function: saveGame
** Description: Saves hidden and available board data to an external 
**	text file, allows user to return to a game after closing the 
**	application.
*******************************************************************/
void SavedData::saveGame(int x, int y, int numMines, int goodMrk, int* priBoard, int* seeBoard, float dur){
	//variables
	fstream sveGme;

	//open file and remove any existing contents
	sveGme.open ("saved.txt", fstream::out, fstream::trunc);
	//confirm that file has opened properly
	if (!sveGme.is_open()){
		cout << "Error loading save game file. Gameplay not saved." << endl;
	}
	//save board dimensions, the number of mines, total marked flags, and correctly marked flags
	sveGme << x << " " << y << " " << numMines << " " << goodMrk << endl;
	sveGme << dur << endl;
	//save the board available to the user
	for (int i = 0; i < x; i++){
		for (int j = 0; j < y; j++){
			sveGme << seeBoard[i*x + j] << " ";
		}
		sveGme << endl;
	}
	//save the hidden board
	for (int k = 0; k < x; k++){
		for (int l = 0; l < y; l++){
			sveGme << priBoard[k*x + l] << " ";
		}
		sveGme << endl;
	}
	//close file
	sveGme.close();
	//return to gamePly (located in main.cpp)
	return;
}
/*******************************************************************
** Function: openSv
** Description: Checks for a saved game in saved.txt and asks the
**	user if they want to continue their game or start a new one.
*******************************************************************/
float* SavedData::openSv(){
	char saved[SIZE];
	bool input = true;
	fstream svGame;
	//Open saved game file
	svGame.open ("saved.txt", fstream::in | fstream::out);
	//Confirm file open
	if (!svGame.is_open()){
		std::cout << "Error loading save game file. Continued gameplay will not be saved." << endl;
	}
	svGame.seekg(0,ios::end);		//get final character in file
	int emt = (int)svGame.tellg();	//save final character
	svGame.close();					//close file
	//if the file has data in it
	do{
		if (emt != 0){
			save = true;
			cout << "Would you like to continue your last saved game?" << "\nPlease enter 'yes' or 'no'." << endl;
			cin >> saved;
			if (saved[0] == 'y' || saved[0] == 'Y'){
				info = getSave();
			}
			else if (saved[0] == 'n' || saved[0] == 'N'){
				return 0;
			}
			else{
				cout << "Invalid input." << endl;
				input = false;
			}
		}
	}while(input == false);
	return info;
}
/*******************************************************************
** Function: getSave
** Description: Pulls the saved data necessary to declare a class 
**	variable of type Minesweeper and passes this data to the 
**	gameplay function
*******************************************************************/
float* SavedData::getSave(){
	//Declare variables
	fstream svGame;
	//Open save game file
	svGame.open ("saved.txt", fstream::in);
	//Confirm file open
	if (!svGame.is_open()){
		std::cout << "Error loading save game file." << endl;
		exit(EXIT_FAILURE);
	}
	//Grab data
	for (int i = 0; i < SIZE; i++){
		svGame >> *(info + i);
	}
	//Close file
	svGame.close();
	return info;
}
/*******************************************************************
** Function: setHiSc
** Description: Pulls high score data and compares user time to it
**	before inserting updated data back into a save file.
*******************************************************************/
void SavedData::setHiSc(float time){
	//variables
	float nxtLn;
	float highs[10];
	bool x = true;
	fstream hiSc;
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
void SavedData::getHiSc(){
	//variables
	float high;
	fstream hiSc;
	//open file
	hiSc.open ("hiScore.txt", fstream::in);
	//confirm file opening
	if (!hiSc.is_open()){
		std::cout << "Error loading high scores file." << endl;
		exit(EXIT_FAILURE);
	}
	//display high scores
	cout << "Best times (seconds):" << endl;
	for (int i = 0; i < 10; i++){
		hiSc >> high;
		cout << setw(4);
		cout << i+1 << ") " << high << endl;
	}
	//close file
	hiSc.close();
	return;
}
/*******************************************************************
** Function: begClck
** Description: Starts clock time
*******************************************************************/
void SavedData::begClck(){
	//start timer
	start = clock();
}
/*******************************************************************
** Function: endClck
** Description: Stops clock time and returns uesr time data
*******************************************************************/
float SavedData::endClck(){
	float duration;
	//end timer
	clock_t end = clock();
	//calculate total time
	clock_t ticks = end - start;
	//convert to seconds
	if (save == true) 
		duration = (ticks / (float) CLOCKS_PER_SEC) + *(info + 4);
	else 
		duration = ticks / (float) CLOCKS_PER_SEC;
	//send data to high score function and return
	return duration;
}