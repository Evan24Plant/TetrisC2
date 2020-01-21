#include "TetrisGame.h"
#include <iostream>
#include <iomanip>

TetrisGame::TetrisGame(sf::RenderWindow *window, sf::Sprite *blockSprite, Point gameboardOffset, Point nextShapeOffset) {
	// Setup the game parameters
	pWindow = window;
	pBlockSprite = blockSprite;
	this->gameboardOffset = gameboardOffset;
	this->nextShapeOffset = nextShapeOffset;

	// Start a new game
	reset();

	// Setup the score text
	scoreFont.loadFromFile("fonts/RedOctober.ttf");
	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(435, 325);
}

TetrisGame::~TetrisGame() {
	pWindow = nullptr;
	pBlockSprite = nullptr;
}

void TetrisGame::draw() {
	// Draw the game 
	drawGameboard();
	drawTetromino(currentShape, gameboardOffset);
	drawGhost(currentShape, gameboardOffset);
	drawTetromino(nextShape, nextShapeOffset);
	pWindow->draw(scoreText);
}

void TetrisGame::onKeyPressed(sf::Event event) {
	switch (event.key.code) {
		case sf::Keyboard::Up:
			attemptRotate(currentShape);
			break;
		case sf::Keyboard::Left:
			attemptMove(currentShape, -1, 0);
			break;
		case sf::Keyboard::Right:
			attemptMove(currentShape, 1, 0);
			break;
		case sf::Keyboard::Down:
			if (!attemptMove(currentShape, 0, 1)) {
				lock(currentShape);
				shapePlacedSinceLastGameLoop = true;
			}
			break;
		case sf::Keyboard::Space:
			drop(currentShape);
			lock(currentShape);
			shapePlacedSinceLastGameLoop = true;
			break;
	}
}

void TetrisGame::processGameLoop(float secondsSinceLastLoop) {
	secondsSinceLastTick += secondsSinceLastLoop;

	if (secondsSinceLastTick > secsPerTick) {
		tick();
		secondsSinceLastTick -= secsPerTick;
	}

	if (shapePlacedSinceLastGameLoop) {
		if (spawnNextShape()) {
			pickNextShape();
			int newScore = score + board.removeCompletedRows();
			setScore(newScore);
			determineSecsPerTick();
		} else {
			reset();
		}
	}
	
	shapePlacedSinceLastGameLoop = false;
}

void TetrisGame::tick() {
	// Move the current shape down
	if (!attemptMove(currentShape, 0, 1)) { // the shape can't move any further
		// Place the shape on the gameboard
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
	}
}

void TetrisGame::reset() {
	// Setup a new game 
	setScore(0);
	determineSecsPerTick();
	board.empty();
	resetShapeSet();

	// Spawn the currentShape and get nextShape ready
	pickNextShape();
	spawnNextShape();
	pickNextShape();
}
 
void TetrisGame::pickNextShape() {
	// Reset the shape set if needed
	resetShapeSet();

	int s = shapeSet.back();
	shapeSet.pop_back();
	nextShape.setShape(static_cast<TetShape>(s));
}

bool TetrisGame::spawnNextShape() {
	// Grab the next shape, and put it on the gameboard
	currentShape = nextShape;
	currentShape.setGridLoc(board.getSpawnLoc());

	// Make sure there is room for the next shape
	return isPositionLegal(currentShape);
}

bool TetrisGame::attemptRotate(GridTetromino &shape) {
	// Test the rotation on a new shape
	GridTetromino testShape = shape;
	testShape.rotateCW();
	if (isPositionLegal(testShape)) { // the rotation is valid
		// Rotate the real shape
		shape.rotateCW();
		return true;
	} else { // the rotation is invalid
		return false;
	}
}

bool TetrisGame::attemptMove(GridTetromino &shape, int x, int y) {
	// Test the move on a new shape
	GridTetromino testShape = shape;
	testShape.move(x, y);
	if (isPositionLegal(testShape)) { // the move is valid
		// Move the real shape
		shape.move(x, y);
		return true;
	} else { // the move is invalid
		return false;
	}
}

void TetrisGame::drop(GridTetromino &shape) {
	// Move the block down until it no longer can
	while (attemptMove(shape, 0, 1));
}

void TetrisGame::lock(const GridTetromino &shape) {
	// Copy the GridTetromino blocks to the board
	board.setContent(shape.getBlockLocsMappedToGrid(), shape.getColor());
}

// Graphics methods ==============================================

void TetrisGame::drawBlock(int x, int y, TetColor color, Point origin) {
	// Get the correct sprite texture
	pBlockSprite->setTextureRect(sf::IntRect(color * BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
	
	// Position the sprite on the gameboard
	pBlockSprite->setPosition(origin.getX() + (x * BLOCK_WIDTH), origin.getY() + (y * BLOCK_HEIGHT));

	// Draw the sprite
	pWindow->draw(*pBlockSprite);
}

void TetrisGame::drawGameboard() {
	// Cycle through each spot of the gameboard
	for (int y = 0; y < board.MAX_Y; y++) {
		for (int x = 0; x < board.MAX_X; x++) {
			// Grab the block value stored at the current spot
			int content = board.getContent(x, y);
			if (content != board.EMPTY_BLOCK) { // there is a block in this spot
				// Draw the block
				drawBlock(x, y, static_cast<TetColor>(content), gameboardOffset);
			}
		}
	}
}

void TetrisGame::drawTetromino(GridTetromino tetromino, Point origin) {
	// Draw each block of the tetromino
	for (Point block : tetromino.getBlockLocsMappedToGrid()) {
		drawBlock(block.getX(), block.getY(), tetromino.getColor(), origin);
	}
}

void TetrisGame::drawGhost(GridTetromino tetromino, Point origin) {
	// Create a ghost shape from the currentShape
	GridTetromino ghostShape = tetromino;
	ghostShape.setGhost();

	// Drop the ghost shape and draw it
	drop(ghostShape);
	drawTetromino(ghostShape, origin);
}

void TetrisGame::setScore(int score) {
	// Set the new score
	this->score = score;

	// Display the new score
	std::string scoreStr = "score: " + std::to_string(this->score);
	scoreText.setString(scoreStr);
}

// State & gameplay/logic methods ================================

bool TetrisGame::isPositionLegal(const GridTetromino &shape) {
	return (isShapeWithinBorders(shape) && !doesShapeIntersectLockedBlocks(shape));
}

bool TetrisGame::isShapeWithinBorders(const GridTetromino &shape) {
	// Check each block in the shpae
	for (Point block : shape.getBlockLocsMappedToGrid()) {
		// Get the block location
		int blockX = block.getX();
		int blockY = block.getY();

		// Check the location
		if (blockX < 0 || blockX >= board.MAX_X || blockY >= board.MAX_Y) { // the block is out of borders
			return false;
		}
	}
	return true;
}

bool TetrisGame::doesShapeIntersectLockedBlocks(const GridTetromino &shape) {
	return !board.areLocsEmpty(shape.getBlockLocsMappedToGrid());
}

void TetrisGame::determineSecsPerTick() {
	if (score == 0) { // game was reset
		secsPerTick = MAX_SECS_PER_TICK;
	} else if (secsPerTick > MIN_SECS_PER_TICK) {
		// Increase difficulty
		secsPerTick = (MAX_SECS_PER_TICK - (static_cast<double>(score) / 100));
		if (secsPerTick < MIN_SECS_PER_TICK) { // it's too difficult
			secsPerTick = MIN_SECS_PER_TICK;
		}
	}	
}

void TetrisGame::resetShapeSet() {
	if (shapeSet.empty()) { // No more shapes left
		// Create a new set of shapes
		for (int i = 0; i < TetShapeCount; i++) {
			shapeSet.push_back(i);
			shapeSet.push_back(i);
		}

		// Shuffle the shape order
		std::random_shuffle(shapeSet.begin(), shapeSet.end());
	}
}