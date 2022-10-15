#include <iostream>
#include "TicTacToe.h"

int board[9] = {};	//-1=X, 0=Blank, 1=O

int rowWin[3] = { 0,0,0 };	//For checking if a player won
int columnWin[3] = { 0,0,0 };
int diagonalWin[3] = { 0,0,0 };
int invDiagonalWin[3] = { 0,0,0 };

int makeMove(int row, int column, int player)
{
	int index = column + row * 3;
	if (board[index])	//If board[index]!=0
	{
		return 2;
	}
	board[index] = player;
	rowWin[column] += player;
	columnWin[row] += player;

	int dCR = column - row;	//For calculating diagonals
	int diCR = (2 - column) - row;
	if (dCR < 0)dCR += 3;
	if (diCR < 0)diCR += 3;

	diagonalWin[dCR] += player;
	invDiagonalWin[diCR] += player;
	return rowWin[column] / 3 + columnWin[row] / 3 + diagonalWin[dCR] / 3 + invDiagonalWin[diCR] / 3;	//Returns winning team, or 0 if no-one won
}

int* getBoard()
{
	return board;
}

void printBoard()
{
	std::cout << "--0---1---2--" << '\n';
	for (int x = 0; x < 3; x++)
	{
		std::cout << x << " ";
		for (int y = 0; y < 3; y++) {
			if (!board[x * 3 + y])
			{
				std::cout << "  | ";
			}
			else
			{
				std::cout << (board[x * 3 + y] == 1 ? "X " : "O ") << "| ";
			}
		}
		std::cout << '\n' << "-------------" << '\n';
	}
}