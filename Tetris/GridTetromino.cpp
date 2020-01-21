#include "GridTetromino.h"


GridTetromino::GridTetromino() {
	setGridLoc(0, 0);
}

Point GridTetromino::getGridLoc() const {
	return gridLoc;
}

void GridTetromino::setGridLoc(int x, int y) {
	gridLoc = Point(x, y);
}

void GridTetromino::setGridLoc(const Point &pt) {
	gridLoc = pt;
}

void GridTetromino::move(int xOffset, int yOffset) {
	gridLoc = Point(gridLoc.getX() + xOffset, gridLoc.getY() + yOffset);
}

std::vector<Point> GridTetromino::getBlockLocsMappedToGrid() const {
	std::vector<Point> mappedBlocks;

	for (Point block : blockLocs) {
		mappedBlocks.push_back(Point(block.getX() + gridLoc.getX(), block.getY() + gridLoc.getY()));
	}

	return mappedBlocks;
}
