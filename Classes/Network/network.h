#pragma once
#include"BeforeBattle/Chess.h"

const int maxChessNum = 20;

struct ChessInfoInBattle 
{

	string chessCareer;
	int chessName;
	int chessLevel;
	int chessBoardPosX;
	int chessBoardPosY;
};


struct Packet 
{
	int totalChessNum = 0;
	ChessInfoInBattle chesses[maxChessNum];
};