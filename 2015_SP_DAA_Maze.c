/*
 * 2015_SP_DAA_Maze.c
 *
 *  Created on: Jan 17, 2015
 *      Author: kachimma
 */

#include <stdio.h>
#include <stdlib.h>

#include "2015_SP_DAA_Maze.h"

int solveMazeRec(int row, int col);
void solveMazeIt(int row, int col);


int maze[MATRIX_SIZE][MATRIX_SIZE];
#define WALL (1 << 30)
#define MARK (1 << 29)
#define MASK (((-1) ^ WALL) ^ MARK)
int start_col; // the starting column for the maze;

void printMaze(void) {
	unsigned r, c;
	for (r = 0; r < MATRIX_SIZE; r += 1) {
		for (c = 0; c < MATRIX_SIZE; c += 1) {
			switch (maze[r][c] ) {
			case 0:
				putchar(' ');
				break;
			case 1:
				putchar('#');
				break;
			case 2: // bread crumb
				putchar('o');
				break;
			default: // error!
				putchar('@');
				break;
			}
		}
		putchar('\n');
	}
}

void printCodedMaze(void) {
	unsigned r, c;
	for (r = 0; r < MATRIX_SIZE; r += 1) {
		for (c = 0; c < MATRIX_SIZE; c += 1) {
			switch(maze[r][c] & WALL) {
			case WALL:
				putchar('#');
				break;
			case 0:
				putchar(' ');
				break;
			}
		}
		putchar('\n');
	}
}

enum Directions {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

void interpretDir(int* prow, int* pcol, int dir) {
	switch (dir) {
		case UP: *prow = *prow - 1; break;
		case DOWN: *prow = *prow + 1; break;
		case LEFT: *pcol = *pcol - 1; break;
		case RIGHT: *pcol = *pcol + 1; break;
	}
}

void clearWall(int row, int col, int dir) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;
	interpretDir(&r, &c, dir);
	maze[r][c] &= ~WALL;
}

void markCell(int row, int col) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;

	maze[r][c] |= MARK;
}

int isMarked(int row, int col) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;

	return (maze[r][c] & MARK);
}


/*
 * return an integer that uniquely identifies each cell
 */
int cellID(int row, int col) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;
	return r * MATRIX_SIZE + c;
}

/*
 * reverse the cellID to compute the row and col numbers
 */
void findCell(int cell, int* prow, int* pcol) {
	int r = cell / MATRIX_SIZE;
	int c = cell % MATRIX_SIZE;
	*prow = r / 2;
	*pcol = c / 2;
}

/*
 * store some annotation information in a cell
 * (the annotation must be an int less than 25 bits (i.e., smaller than 32 million)
 */
void annotateCell(int row, int col, int annotation) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;

	maze[r][c] &= ~MASK;
	maze[r][c] |= annotation;
}

int annotation(int row, int col) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;

	return maze[r][c] & MASK;
}

void clearCellMark(int row, int col) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;

	maze[r][c] &= ~MARK;
}

void clearCell(int row, int col) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;

	maze[r][c] = 0;
}

void makeAllWalls(void) {
	unsigned r, c;
	unsigned row, col;
	for (r = 0; r < MATRIX_SIZE; r += 1) {
		for (c = 0; c < MATRIX_SIZE; c += 1) {
			maze[r][c] = WALL;
		}
	}
	for (row = 0; row < MAZE_SIZE; row += 1) {
		for (col = 0; col < MAZE_SIZE; col += 1) {
			clearCell(row, col);
		}
	}
}

/*
 * this function makes a random maze with exactly one path between
 * any two points in the maze
 *
 * If you're curious about the algorithm, come talk to me.  It's not very
 * complicated (although the code might be confusing)
 */
void makeMaze() {
	int num_visited = 1;
	int first;
	int finish_col;
	makeAllWalls();
	markCell(0, 0);  // mark the first cell as visited

	/* add the first cell (row 0, col 0) to the linked list of visited cells */
	first = cellID(0, 0);
	annotateCell(0, 0, 0);

	while(num_visited < MAZE_SIZE * MAZE_SIZE) {
		int visit = rand() % num_visited;
		int cell = first;
		int row, col;
		int d;
		int nrow, ncol;

		findCell(cell, &row, &col);
		while (visit > 0) {
			cell = annotation(row, col);
			findCell(cell, &row, &col);
			visit -= 1;
		}
		d = rand() % 4;
		nrow = row; // row of neighbor cell
		ncol = col; // col of neighbor cell
		interpretDir(&nrow, &ncol, d);
		if (nrow >= 0 && nrow < MAZE_SIZE
			&& ncol >= 0 && ncol < MAZE_SIZE
			&& !isMarked(nrow, ncol)) {
			clearWall(row, col, d);
			num_visited += 1;
			markCell(nrow, ncol);
			annotateCell(nrow, ncol, first);
			first = cellID(nrow, ncol);
		}
	}

	start_col = rand() % MAZE_SIZE;
	start_col = 2 * start_col + 1;
	maze[0][start_col] &= ~WALL;
	finish_col = rand() % MAZE_SIZE;
	maze[MATRIX_SIZE - 1][2 * finish_col + 1] &= ~WALL;
}


/*
 * recode the maze so that all walls are exactly 1 and all
 * other cells are exactly zero
 */
void recodeMaze(void) {
	int r, c;
	for (r = 0; r < MATRIX_SIZE; r += 1) {
		for (c = 0; c < MATRIX_SIZE; c += 1) {
			maze[r][c] = ((maze[r][c] & WALL) == WALL);
		}
	}
}

int main(void) {
	const int magic_number = 13017;

	/* test maze */
	srand(magic_number);
	makeMaze();
	recodeMaze();
	printf("recursive solution to the maze\n");
	solveMazeRec(0, start_col);
	printMaze();
	printf("\n\n\n");

	printf("iterative solution to the maze\n");
	srand(magic_number);
	makeMaze();
	recodeMaze();
	solveMazeIt(0, start_col);
	printMaze();

	return 0;
}


/*
 * the two dimensional array maze[MATRIX_SIZE][MATRIX_SIZE] contains a maze
 * Each square is (initially) either a 1 or a zero.  Each 1 represents a wall
 * (you cannot walk through walls, so you can't go into any square where the
 * value is 1).  Each 0 represents an open space.
 *
 * Write an recursive solution to find your way out of the maze
 * Your starting point is at row and col. (params to this function)
 * Your exit point is somewhere in the last row (which is: MATRIX_SIZE - 1)
 *
 * There is a relatively easy recursive solution to this problem, the trick is
 * to think of the solution in the following terms:
 *   "Does the shortest path to the exit go through the current square
 *   (indicated by row and col)?"
 * With that hint, the base case should be pretty obvious.  In fact,
 * I'd suggest you consider the possibility that the base case is "Yup, the
 * shortest path to the exit goes through here, I know 'cause I'm already at
 * the exit!"
 *
 * If you're not at the base case, then ask the following question about each
 * of your four adjacent cells:
 *     Does the shortest path to the exit go through there?
 * If the answer were yes, then leaving the maze would be easy, just go to that
 * square.
 * So, the recursive maze solving problem has a boolean (true/false) return
 * value.  It returns true if the shortest path to the exit goes through
 * the square (row, col) and returns false otherwise.  When row and col
 * are the starting point, the answer will always be true (this function
 * always return true to main).  However, some of the recursive calls will
 * return false.
 *
 * There is one tricky part to this decomposition.  You need to make certain
 * that the problem is getting smaller.  The "bigness" or "smallness" of this
 * problem is the number of squares that have not yet been tested.  Each time
 * you test an adjacent square (making a recursive call to decide if the
 * shortest path to the exit goes through that square), you'll be reducing
 * the number of squares that have not yet been tested.  Eventually, you must
 * have tested all the squares and so you'll have to have found a way to the
 * exit.
 *
 * The easy way to deal with the tricky part is to leave "bread crumbs" behind.
 * Before you recursively check any (or all) of your neighbors to see if you
 * can find the exit from there -- drop a bread crumb in your current square.
 * Then, don't ever check to see if you can find the exit using a square
 * with a breadcrumb in it (you've already checked that square, or at least are
 * in the process of checking it).
 *
 * If you're trying to see if you can find an exit from some square, and all
 * the adjacent squares are either walls, or have bread crumbs in them, then
 * you already know the answer -- "The shortest path to the exit does NOT go
 * through this square".  Pick up your bread crumb and return false.
 *
 * You can set the value of the current square to "2" to indicate that a bread
 * crumb has been dropped.  Set the square back to 0 when you want to pick
 * the bread crumb back up.
 * be sure not to change any of the squares that are 1 (the walls).
 *
 * for partial credit, you can leave all your bread crumbs on the floor.
 * for full credit, you must pick up all the bread crumbs EXCEPT those
 * along the shortest path to the exit.
 */

int solveMazeRec(int row, int col) {
	return 0;
}


/**********************
 * adjacentCell and isOK are functions provided to help you write
 * solveMazeIt()
 */

/*
 * OK, we're currently at maze[row][col] and we're considering moving
 * in direction dir.
 * Set trow and tcol (local variables inside the calling function)
 * to the row and column that we would move to IF we moved in
 * that direction
 *
 * For example, there are two good ways to use this function.
 * 1. to actually move one step in a direction use:
 *       adjacentCell(row, col, dir, &row, &col);
 *    That will set row and col to new values.  The new values will
 *    be one square away from the old values.
 *
 * 2. to set trow and tcol to a square that is adjacent to row and col use:
 *       adjacentCell(row, col, dir, &trow, &tcol);
 *    That will not change row and col, but will change trow and tcol.
 *    This is useful if you aren't sure if you can actually move in this
 *    direction yet (e.g., maze[trow][tcol] may be a wall!).  So, you set
 *    trow and tcol, and then check to see if it's OK to move there
 */
void adjacentCell(int row, int col, int dir, int* trow, int* tcol) {
	*trow = row;
	*tcol = col;
	switch(dir) {
	case 0: // UP
		*trow = *trow - 1;
		break;
	case 1: // RIGHT
		*tcol = *tcol + 1;
		break;
	case 2: // DOWN
		*trow = *trow + 1;
		break;
	case 3: // LEFT
		*tcol = *tcol - 1;
		break;
	}
}


/*
 * return false if there is a wall in the square for row and col
 * return true if it's not a wall.
 */
int isOK(int row, int col) {
	return (row > 0 && row < MATRIX_SIZE
		&& col > 0 && col < MATRIX_SIZE
		&& maze[row][col] != 1);
}

/*
 * return the value of the direction that is one turn to the right
 */
int turnRight(int dir) {
	return (dir + 1) % 4;
}

/*
 * return the value of the direction that is one turn to the left
 */
int turnLeft(int dir) {
	return (dir + 3) % 4;
}

/*
 * the two dimensional array maze[MATRIX_SIZE][MATRIX_SIZE] contains a maze
 * Each square is (initially) either a 1 or a zero.  Each 1 represents a wall
 * (you cannot walk through walls, so you can't go into any square where the value
 * is 1).  Each 0 represents an open space.
 *
 * write an iterative solution to find your way out of the maze
 * Your starting point is at row and col. (params to this function)
 * Your exit point is somewhere in the last row (which is: MATRIX_SIZE - 1)
 * The maze can be solved by following the right hand wall.  In fact, there
 * is exactly one path between any two points in the maze (well, between any two
 * points that are not walls).
 *
 * The algorithm is not too bad, although it is certainly not trivial
 * Initially, you'll be headed DOWN (direction 2)
 * Each iteration do one of the following.  Note that only one of two cases
 * can possibly apply (the properties of the maze guarantee that).
 *    case 1: you can move in the current direction
 *       In this case, you should take one step in the current direction
 *       and then turn right.
 *    case 2: you cannot move in the current direction
 *       If there's a wall directly in front of you, then to follow the right
 *       hand wall, you'd need to turn left (placing your hand on the wall that
 *       used to be directly in front of you).  So, if you discover this case
 *       then turn left.  Don't move, just turn left.
 * If you were walking down a straight corridor (with walls on both sides)
 * then you'd alternate case 1 and case 2.  On the first iteration, you'd
 * take a step, and then turn right (case 1).  This causes you to walk
 * one position down the hallway but then turn to face the wall.
 * On the next iteration, you'll be facing the wall, so you can't walk
 * forward and therefore (case 2) you'll turn left.  On the third iteration
 * you'll be able to walk down the hallway again.
 *
 * If you follow those two cases, then you'll eventually find your way out
 * of the maze.  To confirm that you're making it out, leave a "bread crumb"
 * behind each square along the path to the exit.
 * For partial credit, place a bread crumb in every square you visit.
 * For full credit, pick up all the breadcrumbs when you backtrack.
 * Backtracking is when you go back to a square you've already been before.
 * If the square you're about to step into has a breadcrumb, then pick up
 * the bread crumb in the square you're at now.
 * You indicate "bread crumbs" by setting the square equal to "2"
 */
void solveMazeIt(int row, int col) {
	int dir = 2; // 0 is up, 1 is right, 2 is down, 3 is left.
	maze[row][col] = 2; // drop a bread crumb in the starting square
	while (row < MATRIX_SIZE - 1) { // the exit is the only open square
				// in the last row

		/* the rest of this loop is yours */

	}
}
