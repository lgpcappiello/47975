/* Chapter 14 presented an IntArray class that dynamically crea tes an array of integers
** and performs bounds checking on the array. If an invalid subscript is used with the
** class, it displays an error message and aborts the program. Modify the class so it
** throws an exception instead.*/

// Specification file for the IntArray class
#ifndef INTARRAY_H_
#define INTARRAY_H_

class IntArray{
private:
	int *aptr;					// Pointer to the array
	int arraySize;				// Holds the array size
	void subscriptError();		//Handles invalid subscripts

public :
	IntArray(int);
	IntArray(const IntArray &);
	~IntArray();
	int size() const
		{ return arraySize; }
	int &operator[](const int &); // Overloaded [] operator
};
#endif