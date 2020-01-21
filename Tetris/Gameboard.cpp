#include "Gameboard.h"
#include <iostream>
#include <iomanip>

Gameboard::Gameboard() {
	empty();
}

int Gameboard::getContent(Point pt) const {
	return grid[pt.getX()][pt.getY()];
}

int Gameboard::getContent(int x, int y) const {
	return grid[x][y];
}

void Gameboard::setContent(Point pt, int content) {
	grid[pt.getX()][pt.getY()] = content;
}

void Gameboard::setContent(int x, int y, int content) {
	grid[x][y] = content;
}

void Gameboard::setContent(std::vector<Point> locs, int content) {
	// Set the content of each point in the vector
	for (Point loc : locs) {
		setContent(loc, content);
	}
}

bool Gameboard::areLocsEmpty(std::vector<Point> locs) const {
	// Check each point in the vector
	for (Point loc : locs) {
		int x = loc.getX();
		int y = loc.getY();
		if (0 <= x && x < MAX_X && 0 <= y && y < MAX_Y) { // point index is a valid grid position
			if (grid[x][y] != EMPTY_BLOCK) { // not all points are empty
				return false;
			}
		} 
	}

	return true;
}

int Gameboard::removeCompletedRows() {
	// Get the completed rows to remove
	std::vector<int> completedRows = getCompletedRowIndices();

	// Remove each completed row
	removeRows(completedRows);

	return completedRows.size();
}

void Gameboard::empty() {
	// Fill each row with empty blocks
	for (int y = 0; y < MAX_Y; y++) {
		fillRow(y, EMPTY_BLOCK);
	}
}

Point Gameboard::getSpawnLoc() const { 
	return spawnLoc;
}

void Gameboard::printToConsole() const {
	// Step through each point of the grid
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			// Print the value at the current loc
			std::cout << std::setw(2) << grid[x][y];
		}
		// Next row
		std::cout << std::endl;
	}
}

bool Gameboard::isRowCompleted(int rowIndex) const {
	// Assume the row is complete
	bool isComplete = true;
	
	// Check each block in the row
	for (int x = 0; x < MAX_X; x++) {
		if (grid[x][rowIndex] == EMPTY_BLOCK) { // a part of the row is still empty
			isComplete = false;
			break;
		}
	}

	return isComplete;
}

std::vector<int> Gameboard::getCompletedRowIndices() const {
	// Create a vector to hold completed row indices
	std::vector<int> v;

	// Check each row of the grid
	for (int y = 0; y < MAX_Y; y++) {
		if (isRowCompleted(y)) { // row y is full
			v.push_back(y);
		}
	}

	return v;
}

void Gameboard::removeRow(int rowIndex) {
	// Start at the target rowIndex, and process each row up to the top
	for (int y = rowIndex; y >= 0; y--) {
		if (y != 0) { // row y is not the top row
			// Move the content above row y into row y
			copyRowIntoRow(y - 1, y);
		} else { // row y is the top row
			// Fill row y with empty blocks
			fillRow(y, EMPTY_BLOCK);
		}
	}
}

void Gameboard::removeRows(std::vector<int> rowIndices) {
	// Remove each row in the vector
	for (int row : rowIndices) {
		removeRow(row);
	}
}

void Gameboard::fillRow(int rowIndex, int content) {
	// Fill each block of the row with the given content
	for (int x = 0; x < MAX_X; x++) {
		setContent(x, rowIndex, content);
	}
}

void Gameboard::copyRowIntoRow(int sourceRowIndex, int targetRowIndex) {
	// For each block, copy the value from the source row to the target row
	for (int x = 0; x < MAX_X; x++) {
		setContent(x, targetRowIndex, getContent(x, sourceRowIndex));
	}
}