#pragma once
#include <vector>
#include <iostream>
#include "Point.h"

enum TetColor {
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	BLUE_LIGHT,
	BLUE_DARK,
	PURPLE,
	CLEAR,
	TetColorCount
};

enum TetShape {
	SHAPE_S,
	SHAPE_Z,
	SHAPE_L,
	SHAPE_J,
	SHAPE_O,
	SHAPE_I,
	SHAPE_T,
	TetShapeCount
};

class Tetromino {
	// Friends
	friend class TestSuite;
public:
	// Constructors
	Tetromino();
	Tetromino(TetShape shape);

	// Functions
	TetColor getColor() const;			// Get the color
	TetShape getShape() const;			// Get the shape

	void setGhost();  // Ghost the shape
	void setShape(TetShape shape);	// Set the shape
	void rotateCW();				// Rotate the shape clockwise
	void printToConsole() const;	// Print out the shape on a grid of points

	static TetShape getRandomShape();	// Get a random shape

private:
	// Variables
	TetColor color;
	TetShape shape;
protected:
	// Variables
	std::vector<Point> blockLocs;
};