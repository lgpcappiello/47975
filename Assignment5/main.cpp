/************************************************************************************************
** Author: Lauren Perry
** Date: November, 25, 2013
** Assignment 5: 4 problems from Chapter 16 
** All problems from Tony Gaddis's "Starting Out with C++", Chapter 16
** 3. Minimum/Maximum Templates
** 4. Absolute Value Template
** 6. IntArray Class Exception
** 7. TestScores Class
************************************************************************************************/
#include <cmath>
#include <iostream>

#include"TestScores.h"
#include"IntArray.h"

using namespace std;

template <class T1>
T1 minimum(T1 x, T1 y){
	if (x > y) return y;
	else return x;
}
template <class T2>
T2 maximum(T2 x, T2 y){
	if (x > y) return x;
	else return y;
}
template <class T3>
T3 absval(T3 a){
	return abs(a);
}

int main(){
	const int SIZE = 10;
	int x = 10;
	int *scores = new int[x];
		scores[0] = 23;
		scores[1] = -6;
		scores[2] = 73;
		scores[3] = 14;
		scores[4] = 98;
		scores[5] = 87;
		scores[6] = 67;
		scores[7] = 66;
		scores[8] = 45;
		scores[9] = 92;

	cout << "The minimum of 3 and -5 is: " << minimum(3, -5) << endl;
	cout << "The minimum of 2.7 and 1.28 is: " << minimum(2.7, 1.28) << endl;
	cout << "The minimum of 'a' and 'z' is: " << minimum('a', 'z') << endl;
	cout << "The maximum of 1 and 16 is: " << maximum(1, 16) << endl;
	cout << "The maximum of 1.27 and -3.0 is: " << maximum(1.27, -3.0) << endl;
	cout << "The maximum of 'b' and 'f' is: " << maximum('b', 'f') << endl;
	cout << "The absolute value of 2.39 is: " << absval(2.39) << endl;
	cout << "The absolute value of -6 is: " << absval(-6) << endl;
	cout << "The absolute value of -9.378 is: " << absval(-9.378) << endl << endl;

	
	IntArray table1(SIZE);
	try{
		for (int i = 0; i < SIZE; i++){
			table1[i] = (i*2);
		}
		for (int j = 0; j < SIZE; j++){
			cout << table1[j] << " ";
		}
	}
	catch (char* excStr2){
		cout << excStr2;
	}
	cout << endl << endl;

	IntArray table2(SIZE);
	try{
		for (int k = 0; k < SIZE+2; k++){
			table2[k] = (k*2);
		}
		for (int l = 0; l < SIZE+2; l++){
			cout << table2[l] << " ";
		}
	}
	catch (char* excStr2){
		cout << excStr2;
	}
	
	cout << endl << endl;

	TestScores scoreSet(scores,x);
	try{
		scoreSet.average();
	}
	catch (char* excStr){
		cout << excStr;
		scoreSet.print();
	}
	delete[] scores;
	
	cin.get();
	return 0;
}



