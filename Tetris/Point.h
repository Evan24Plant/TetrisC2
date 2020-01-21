#pragma once
#include <string>

class Point {
	// Friends
	friend class TestSuite;
public:
	// Constructors
	Point();
	Point(int newX, int newY);

	// Functions
	int getX() const;				// Get X
	int getY() const;				// Get Y

	void setX(int x);				// Set X
	void setY(int y);				// Set Y
	void setXY(int x, int y);		// Set X and Y
	void swapXY();					// Swap X and Y
	void multiplyX(int factor);		// Multiply X by some factor
	void multiplyY(int factor);		// Multiply Y by some factor

	std::string toString() const;	// Return a string in the form "[x, y]" to represent the state of the Point instance (for debugging)
private:
	// Variables
	int x;
	int y;
};
