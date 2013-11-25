/* Write a class named TestScores. The class constructOr should accept an array of test
** scores as its argument. The class should have a member function that returns the average
** of the test scores. [f any test score in the array is nega tive or greater than 100, the
** class should throw an exception. Demonstrate the class in a program.*/

#ifndef TESTSCORES_H_
#define TESTSCORES_H_

class TestScores{
private:
	int *scores;
	int total;
public:
	TestScores(int*, int);
	~TestScores();
	float average();
	void print();
};

#endif