#pragma once

#include <stdio.h>
#include "BowlingCompany.h"


int initCompanyFromFile(BowlingCompany* pCompany, const char* fileName);
int saveCompanyToFile(const BowlingCompany* pCompany, const char* fileName);

int writeBranchToTextFile(FILE* fp, const Branch* pBranch);
int readBranchFromTextFile(FILE* fp, Branch* pBranch);

int writeEmployeeToTextFile(FILE* fp, const Employee* pEmployee);
int readEmployeeFromTextFile(FILE* fp, Employee* pEmployee);


int writeLaneToTextFile(FILE* fp, const Lane* pLane);
int readLaneFromTextFile(FILE* fp, Lane* pLane);

int writeGameToTextFile(FILE* fp, const Game* pGame);
int readGameFromTextFile(FILE* fp, Game* pGame);

int writeNodeToTextFile(FILE* fp, const NODE* pNode);
int readNodesFromTextFile(FILE* fp, LIST* portList, int* pCount);

int writePlayerToTextFile(FILE* fp, const Player* pPlayer);
int readPlayerFromTextFile(FILE* fp, Player* pPlayer);

int writeTurnToTextFile(FILE* fp, const Turn* pTurn);
int readTurnFromTextFile(FILE* fp, Turn* pTurn);

int writeDateToTextFile(FILE* fp, const Date* pDate);
int readDateFromTextFile(FILE* fp, Date* pDate);
