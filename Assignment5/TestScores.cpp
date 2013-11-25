#include<array>
#include<iostream>

#include"TestScores.h"

using namespace std;

TestScores::TestScores(int *newScrs, int x){
	total = x;
	scores = new int[total];
	for (int i = 0; i < total; i++){
		scores[i] = newScrs[i];
	}
}
TestScores::~TestScores(){
	delete []scores;
}
float TestScores::average(){
	int sum = 0, tot = 0;
	for (int i = 0; i < total; i++){
		if (scores[i] < 0 || scores[i] > 100) throw "ERROR: At least one score is less than 0 or greater than 100\n";
		sum += scores[i];
	}
	return (float)sum/total;
}
void TestScores::print(){
	cout << "Test Scores: ";
	for (int i = 0; i < total; i++){
		cout << scores[i] << " ";
	}
	cout << endl;
	return;
}