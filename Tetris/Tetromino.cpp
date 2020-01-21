#include "Tetromino.h"

Tetromino::Tetromino() {
	// Initialize a random shape
	setShape(getRandomShape());
}

Tetromino::Tetromino(TetShape shape) {
	// Initialize a specific shape
	if (shape >= 0 && shape < TetShapeCount) {
		setShape(shape);
	} else { // Initialize a random shape
		setShape(getRandomShape());
	}
}

TetColor Tetromino::getColor() const {
	return color;
}

TetShape Tetromino::getShape() const {
	return shape;
}

void Tetromino::setGhost() {
	color = CLEAR;
}

void Tetromino::setShape(TetShape shape) {
	// Update shape & color variables
	this->shape = shape;
	this->color = static_cast<TetColor>(shape);

	// Adjust the blockLocs based on the new shape
	switch (shape) {
		case SHAPE_S:
			blockLocs = { Point(-1, 0), Point(), Point(0, 1), Point(1, 1) };
			break;
		case SHAPE_Z:
			blockLocs = { Point(-1, 1), Point(0, 1), Point(), Point(1, 0) };
			break;
		case SHAPE_L:
			blockLocs = { Point(0, 1), Point(), Point(0, -1), Point(1, -1) };
			break;
		case SHAPE_J:
			blockLocs = { Point(-1, -1), Point(0, -1), Point(), Point(0, 1) };
			break;
		case SHAPE_O:
			blockLocs = { Point(0, 1), Point(1, 1), Point(), Point(1, 0) };
			break;
		case SHAPE_I:
			blockLocs = { Point(0, 2), Point(0, 1), Point(), Point(0, -1) };
			break;
		case SHAPE_T:
			blockLocs = { Point(-1, 0), Point(), Point(1, 0), Point(0, -1) };
			break;
		default:
			blockLocs = { Point(), Point(), Point(), Point() };
	}
}

void Tetromino::rotateCW() {
	for (int i = 0; i < blockLocs.size(); i++) {
		blockLocs[i].swapXY();
		blockLocs[i].multiplyY(-1);
	}
}

void Tetromino::printToConsole() const {
	// Loop through the printed x, y grid points 
	for (int y = 3; y >= -3; y--) {
		for (int x = -3; x <= 3; x++) {
			// Set s to an empty point
			std::string s = " . ";
			// Check to see if a blockLocs point matches the current point on the grid
			for (Point point : blockLocs) {
				if ((point.getX() == x) && (point.getY() == y)) { // Points match, break to print a block
					s = "[_]";
					break;
				} 
			}
			// Print a block or empty point
			std::cout << s; 
		}
		// Move on to the next line
		std::cout << std::endl; 
	}
}

TetShape Tetromino::getRandomShape() {
	TetShape shape = static_cast<TetShape>(rand() % static_cast<int>(TetShapeCount));	
	return shape;
}