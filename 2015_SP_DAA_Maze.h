/*
 * 2015_SP_DAA_Maze.h
 *
 *  Created on: Jan 17, 2015
 *      Author: kachimma
 */

#ifndef _2015_SP_DAA_MAZE_H_
#define _2015_SP_DAA_MAZE_H_

/* the real "maze" is a 2D array of squares.
 * each square can have a wall to either side (right or left)
 * or to the top and bottom.  To represent this in a matrix, we're
 * alternating rows and columns.  For example, the border of the maze
 * is all walls (except for the entrance and exit).
 * then, each alternating row is used for either squares of the
 * real maze, or walls.  Some walls will be removed to allow passage
 * between the squares.  Some walls will be left standing (wouldn't be
 * much of a maze if there were no walls in it).
 *
 * So, the matrix is about double the size of the actual maze.
 *
 * **** Please use MATRIX_SIZE in your programs
 *
 * If you'd like to test your
 * program with larger (or smaller) mazes, then please change
 * MAZE_SIZE (MATRIX_SIZE will change proportionately).  My maze
 * generator uses MAZE_SIZE.
 */
#define MAZE_SIZE 20
#define MATRIX_SIZE (2 * MAZE_SIZE + 1)

extern int maze[MATRIX_SIZE][MATRIX_SIZE];


#endif /* _2015_SP_DAA_MAZE_H */
