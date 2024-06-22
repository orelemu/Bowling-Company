#pragma once

#include <stdio.h>
#include "BowlingCompany.h"

typedef unsigned char BYTE;

int saveCompanyToBinFile(const BowlingCompany* pCompany, const char* fileName);
int initCompanyFromBinFile(BowlingCompany* pCompany, const char* fileName);

int writeBranchToBinFile(FILE* fp, const Branch* pBranch);
int readBranchFromBinFile(FILE* fp, Branch* pBranch);

int writeEmployeeToBinFile(FILE* fp, const Employee* pEmployee);
int readEmployeeFromBinFile(FILE* fp, Employee* pEmployee);

int writeLaneToBinFile(FILE* fp, const Lane* pLane);
int readLaneFromBinFile(FILE* fp, Lane* pLane);

int writeGameToBinFile(FILE* fp, const Game* pGame);
int readGameFromBinFile(FILE* fp, Game* pGame);

int writeNodeToBinFile(FILE* fp, const NODE* pNode);
int readNodesFromBinFile(FILE* fp, LIST* portList, int* pCount);

int writePlayerToBinFile(FILE* fp, const Player* pPlayer);
int readPlayerFromBinFile(FILE* fp, Player* pPlayer);

int writeTurnToBinFile(FILE* fp, const Turn* pTurn);
int readTurnFromBinFile(FILE* fp, Turn* pTurn);

int writeDateToBinFile(FILE* fp, const Date* pDate);
int readDateFromBinFile(FILE* fp, Date* pDate);

int writeStrToBinFile(FILE* fp, char* str, int isCode);
int readStrFromBinFile(FILE* fp, char* str);

int writeCodeStrToBinFile(FILE* fp, const char* str);
int readCodeStrFromBinFile(FILE* fp, char* str);

int	writeTurnToCompressedBinFile(FILE* fp, const Turn* pTurn);
int readTurnFromCompressedBinFile(FILE* fp, Turn* pTurn);