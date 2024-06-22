#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CompanyBinFile.h"
#include "General.h"


int saveCompanyToBinFile(const BowlingCompany* pCompany, const char* fileName)
{
	FILE* fp = fopen(fileName, "wb");
	if (!fp)
	{
		perror("Error opening bin file");
		return 0;
	}
	// Company & founder names
	if (!writeStrToBinFile(fp, pCompany->name, 0)) // 0 == with \0
	{
		CLOSE_RETURN_ZERO(fp)
	}
	if (!writeStrToBinFile(fp, pCompany->founderName, 0))  // 0 == with \0
	{
		CLOSE_RETURN_ZERO(fp)
	}
	// CEO
	if (!writeEmployeeToBinFile(fp, &pCompany->ceo))
	{
		fclose(fp);
		PRINT_ERROR_STR_RETURN_0("writing employee to bin file")
	}
	// Office branch
	if (!writeBranchToBinFile(fp, pCompany->officeBranch))
	{
		fclose(fp);
		PRINT_ERROR_STR_RETURN_0("writing branch to bin file")
	}

	// All branches (branch arr)s
	if (fwrite(&pCompany->numOfBranches, sizeof(int), 1, fp) != 1)
		return 0;

	for (int i = 0; i < pCompany->numOfBranches; i++)
	{
		if (!writeBranchToBinFile(fp, pCompany->branchArr[i]))
		{
			fclose(fp);
			PRINT_ERROR_STR_RETURN_0("writing branch to bin file")
		}
	}
	// sort type
	int sortType = pCompany->branchSortOpt;
	if (fwrite(&sortType, sizeof(int), 1, fp) != 1)
		return 0;

	fclose(fp);
	return 1;
}
int initCompanyFromBinFile(BowlingCompany* pCompany, const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (!fp)
	{
		perror("Error opening file");
		return 0;
	}

	// Company & founder names
	char tempName[MAX_STR_LEN] = { 0 }, tempFounderName[MAX_STR_LEN] = { 0 };
	if (!readStrFromBinFile(fp, tempName))
	{
		fclose(fp);
		PRINT_ERROR_STR_RETURN_0("reading company name from bin file")
	}
	pCompany->name = strdup(tempName);


	if (!readStrFromBinFile(fp, tempName))
	{
		fclose(fp);
		PRINT_ERROR_STR_RETURN_0("reading founder name from bin file")
	}
	pCompany->founderName = strdup(tempName);

	// CEO
	Employee* pEmp = (Employee*)malloc(sizeof(Employee));
	if (!pEmp)
	{
		fclose(fp);
		ALLOC_ERROR_RETURN()
	}
	if (!readEmployeeFromBinFile(fp, pEmp))
	{
		fclose(fp);
		PRINT_ERROR_STR_RETURN_0("reading employee(ceo) from bin file")
	}
	pCompany->ceo = *pEmp;

	// Office branch
	pCompany->officeBranch = (Branch*)malloc(sizeof(Branch));
	if (!pCompany->officeBranch)
	{
		fclose(fp);
		ALLOC_ERROR_RETURN()
	}
	if (!readBranchFromBinFile(fp, pCompany->officeBranch))
	{
		fclose(fp);
		PRINT_ERROR_STR_RETURN_0("reading branch(office) from bin file")
	}

	// All branches (branch arr)
	if (fread(&pCompany->numOfBranches, sizeof(int), 1, fp) != 1)
	{
		CLOSE_RETURN_ZERO(fp)
	}
	pCompany->branchArr = (Branch**)malloc(sizeof(Branch*) * pCompany->numOfBranches);
	if (!pCompany->branchArr)
	{
		fclose(fp);
		ALLOC_ERROR_RETURN()
	}
	for (int i = 0; i < pCompany->numOfBranches; i++)
	{
		pCompany->branchArr[i] = (Branch*)malloc(sizeof(Branch));
		if (!pCompany->branchArr[i])
		{
			fclose(fp);
			ALLOC_ERROR_RETURN()
		}
		if (!readBranchFromBinFile(fp, pCompany->branchArr[i]))
		{
			fclose(fp);
			PRINT_ERROR_STR_RETURN_0("reading branch(office) from bin file")
		}
	}

	// sort type
	int sortType = 0;
	if (fread(&sortType, sizeof(int), 1, fp) != 1)
	{
		CLOSE_RETURN_ZERO(fp)
	}
	pCompany->branchSortOpt = (eSortOption)sortType;
	sortBranchesByType(pCompany, sortType);

	fclose(fp);
	return 1;
}

int writeBranchToBinFile(FILE* fp, const Branch* pBranch)
{
	// Branch name
	if (!writeStrToBinFile(fp, pBranch->branchName, 0)) // 0 == with \0
	{
		PRINT_ERROR_STR_RETURN_0("writing branch name to bin file")
	}
	// Branch address
	if (!writeStrToBinFile(fp, pBranch->branchAddress, 0)) // 0 == with \0
	{
		PRINT_ERROR_STR_RETURN_0("writing branch address to bin file")
	}
	// Branch serial num
	if (fwrite(&pBranch->branchSerialNum, sizeof(int), 1, fp) != 1)
		return 0;

	// All employees (employee arr)
	if (fwrite(&pBranch->numOfEmployees, sizeof(int), 1, fp) != 1)
		return 0;
	for (int i = 0; i < pBranch->numOfEmployees; i++)
	{
		if (!writeEmployeeToBinFile(fp, pBranch->employeeArr[i]))
		{
			PRINT_ERROR_STR_RETURN_0("writing employee to bin file")
		}
	}

	// All lanes (lane arr)
	if (fwrite(&pBranch->numOfLanes, sizeof(int), 1, fp) != 1)
		return 0;
	for (int i = 0; i < pBranch->numOfLanes; i++)
	{
		if (!writeLaneToBinFile(fp, pBranch->lanesArr[i]))
		{
			PRINT_ERROR_STR_RETURN_0("writing lane to bin file")
		}
	}

	return 1;
}
int readBranchFromBinFile(FILE* fp, Branch* pBranch)
{
	char tempName[MAX_STR_LEN] = { 0 }, tempAddress[MAX_STR_LEN] = { 0 };
	// Branch name
	if (!readStrFromBinFile(fp, tempName))
	{
		PRINT_ERROR_STR_RETURN_0("reading branch name from bin file")
	}
	pBranch->branchName = strdup(tempName);

	// Branch address
	if (!readStrFromBinFile(fp, tempAddress))
	{
		PRINT_ERROR_STR_RETURN_0("reading branch name from bin file")
	}
	pBranch->branchAddress = strdup(tempAddress);

	// Branch serial num
	if (fread(&pBranch->branchSerialNum, sizeof(int), 1, fp) != 1)
	{
		PRINT_ERROR_STR_RETURN_0("reading branch serial number from bin file")
	}

	// All employees (employee arr)
	if (fread(&pBranch->numOfEmployees, sizeof(int), 1, fp) != 1)
	{
		PRINT_ERROR_STR_RETURN_0("reading branch number of employees from bin file")
	}
	pBranch->employeeArr = (Employee**)malloc(sizeof(Employee*) * pBranch->numOfEmployees);
	if (!pBranch->employeeArr)
	{
		ALLOC_ERROR_RETURN()
	}
	for (int i = 0; i < pBranch->numOfEmployees; i++)
	{
		pBranch->employeeArr[i] = (Employee*)malloc(sizeof(Employee));
		if (!pBranch->employeeArr[i])
		{
			ALLOC_ERROR_RETURN()
		}
		if (!readEmployeeFromBinFile(fp, pBranch->employeeArr[i]))
		{
			PRINT_ERROR_STR_RETURN_0("reading employee from bin file")
		}
	}

	// All lanes (lane arr)
	if (fread(&pBranch->numOfLanes, sizeof(int), 1, fp) != 1)
	{
		PRINT_ERROR_STR_RETURN_0("reading branch number of lanes from bin file")
	}
	pBranch->lanesArr = (Lane**)malloc(sizeof(Lane*) * pBranch->numOfLanes);
	if (!pBranch->lanesArr)
	{
		ALLOC_ERROR_RETURN()
	}
	for (int i = 0; i < pBranch->numOfLanes; i++)
	{
		pBranch->lanesArr[i] = (Lane*)malloc(sizeof(Lane));
		if (!pBranch->lanesArr[i])
		{
			ALLOC_ERROR_RETURN()
		}
		if (!readLaneFromBinFile(fp, pBranch->lanesArr[i]))
		{
			PRINT_ERROR_STR_RETURN_0("reading lane from bin file")
		}
	}

	return 1;
}

int writeEmployeeToBinFile(FILE* fp, const Employee* pEmployee)
{
	// Employee name
	if (!writeStrToBinFile(fp, pEmployee->empName, 0)) // 0 == with \0
	{
		PRINT_ERROR_STR_RETURN_0("writing employee name to bin file")
	}
	// Employee id
	if (fwrite(&pEmployee->empID, sizeof(int), 1, fp) != 1)
		return 0;
	// Employee salary
	if (fwrite(&pEmployee->salary, sizeof(double), 1, fp) != 1)
		return 0;

	// Employee type
	int empType = pEmployee->empType;
	if (fwrite(&empType, sizeof(int), 1, fp) != 1)
		return 0;

	return 1;
}
int readEmployeeFromBinFile(FILE* fp, Employee* pEmployee)
{
	// Employee name
	char tempName[MAX_STR_LEN] = { 0 };
	if (!readStrFromBinFile(fp, tempName))
	{
		PRINT_ERROR_STR_RETURN_0("reading employee name from bin file")
	}
	pEmployee->empName = strdup(tempName);

	// Employee id
	if (fread(&pEmployee->empID, sizeof(int), 1, fp) != 1)
	{
		PRINT_ERROR_STR_RETURN_0("reading employee id from bin file")
	}
	// Employee salary
	if (fread(&pEmployee->salary, sizeof(double), 1, fp) != 1)
	{
		PRINT_ERROR_STR_RETURN_0("reading employee salary from bin file")
	}
	// Employee type
	int empType = 0;
	if (fread(&empType, sizeof(int), 1, fp) != 1)
	{
		PRINT_ERROR_STR_RETURN_0("reading employee type from bin file")
	}
	pEmployee->empType = (eEmpType)empType;

	return 1;
}

int writeLaneToBinFile(FILE* fp, const Lane* pLane)
{
	// Lane code
	if (!writeCodeStrToBinFile(fp, pLane->laneCode))
	{
		PRINT_ERROR_STR_RETURN_0("writing lane code to bin file")
	}
	// Lane best score
	if (fwrite(&pLane->bestScore, sizeof(int), 1, fp) != 1)
		return 0;

	// Lane games (game arr)
	if (fwrite(&pLane->numOfGames, sizeof(int), 1, fp) != 1)
		return 0;
	for (int i = 0; i < pLane->numOfGames; i++)
	{
		if (!writeGameToBinFile(fp, pLane->games[i]))
		{
			PRINT_ERROR_STR_RETURN_0("writing game to bin file")
		}
	}

	return 1;
}
int readLaneFromBinFile(FILE* fp, Lane* pLane)
{
	// Lane code
	if (!readCodeStrFromBinFile(fp, pLane->laneCode))
	{
		PRINT_ERROR_STR_RETURN_0("reading lane code from bin file")
	}
	// Lane best score
	if (fread(&pLane->bestScore, sizeof(int), 1, fp) != 1)
	{
		PRINT_ERROR_STR_RETURN_0("reading lane best score from bin file")
	}
	// Lane games (game arr)
	if (fread(&pLane->numOfGames, sizeof(int), 1, fp) != 1)
	{
		PRINT_ERROR_STR_RETURN_0("reading lane number of games from bin file")
	}
	pLane->games = (Game**)malloc(sizeof(Game*) * pLane->numOfGames);
	if (!pLane->games)
	{
		ALLOC_ERROR_RETURN()
	}
	for (int i = 0; i < pLane->numOfGames; i++)
	{
		pLane->games[i] = (Game*)malloc(sizeof(Game));
		if (!pLane->games[i])
		{
			ALLOC_ERROR_RETURN()
		}
		if (!readGameFromBinFile(fp, pLane->games[i]))
		{
			PRINT_ERROR_STR_RETURN_0("reading game from bin file")
		}
	}

	return 1;
}

int writeGameToBinFile(FILE* fp, const Game* pGame)
{
	// winner player
	if (!writePlayerToBinFile(fp, pGame->winner))
	{
		PRINT_ERROR_STR_RETURN_0("writing player(winner) to bin file")
	}

	// All players (player list)
	if (fwrite(&pGame->numOfPlayers, sizeof(int), 1, fp) != 1)
		return 0;
	if (!writeNodeToBinFile(fp, &(pGame->playersList.head)))
	{
		PRINT_ERROR_STR_RETURN_0("writing player node to bin file")
	}

	// game date
	if (!writeDateToBinFile(fp, &pGame->date))
	{
		PRINT_ERROR_STR_RETURN_0("writing date to bin file")
	}
	return 1;
}
int readGameFromBinFile(FILE* fp, Game* pGame)
{
	// winner player
	pGame->winner = (Player*)malloc(sizeof(Player));
	if (!pGame->winner)
	{
		ALLOC_ERROR_RETURN()
	}
	if (!readPlayerFromBinFile(fp, pGame->winner))
	{
		PRINT_ERROR_STR_RETURN_0("reading Player(winner) from bin file")
	}

	// All players (player list)
	if (!L_init(&pGame->playersList))
	{
		ALLOC_ERROR_RETURN()
	}
	if (!readNodesFromBinFile(fp, &pGame->playersList, &pGame->numOfPlayers))
	{
		PRINT_ERROR_STR_RETURN_0("reading Players Nodes from bin file")
	}

	// game date
	if (!readDateFromBinFile(fp, &pGame->date))
	{
		PRINT_ERROR_STR_RETURN_0("reading Date from bin file")
	}
	return 1;
}

int writeNodeToBinFile(FILE* fp, const NODE* pNode)
{
	// pNode = header
	NODE* current = pNode->next;
	while (current != NULL)
	{
		writePlayerToBinFile(fp, (Player*)(current->key));
		current = current->next;
	}
	return 1;
}
int readNodesFromBinFile(FILE* fp, LIST* pPlayerList, int* pCount)
{
	NODE* pNode = &(pPlayerList->head);
	if (fread(pCount, sizeof(int), 1, fp) != 1)
	{
		PRINT_ERROR_STR_RETURN_0("reading player count from bin file")
	}

	for (int i = 0; i < *pCount; i++)
	{
		Player* pPlayer = (Player*)malloc(sizeof(Player));
		if (!pPlayer)
		{
			L_free(pPlayerList, freePlayer);
			return 0;
		}
		if (!readPlayerFromBinFile(fp, pPlayer))
		{
			L_free(pPlayerList, freePlayer);
			return 0;
		}
		pNode = L_insert(pNode, pPlayer);
		if (pNode == NULL)
			return 0;
	}
	return 1;
}

int writePlayerToBinFile(FILE* fp, const Player* pPlayer)
{
	// Player name
	if (!writeStrToBinFile(fp, pPlayer->playerName, 0)) // 0 == with \0
	{
		PRINT_ERROR_STR_RETURN_0("writing player name to bin file")
	}
	// Player turns (Turn arr |10|)
	for (int i = 0; i < TURNS; i++)
	{
		//if (!writeTurnToBinFile(fp, &pPlayer->turns[i]))
		if (!writeTurnToCompressedBinFile(fp, &pPlayer->turns[i]))
		{
			PRINT_ERROR_STR_RETURN_0("writing turn to bin file")
		}
	}
	// Player final score
	if (fwrite(&pPlayer->finalScore, sizeof(int), 1, fp) != 1)
		return 0;

	return 1;
}
int readPlayerFromBinFile(FILE* fp, Player* pPlayer)
{
	// Player name
	char tempName[MAX_STR_LEN] = { 0 };
	if (!readStrFromBinFile(fp, tempName))
	{
		PRINT_ERROR_STR_RETURN_0("reading player name from bin file")
	}
	pPlayer->playerName = strdup(tempName);

	// Player turns (Turn arr |10|)
	for (int i = 0; i < TURNS; i++)
	{
		//if (!readTurnFromBinFile(fp, &pPlayer->turns[i]))
		if (!readTurnFromCompressedBinFile(fp, &pPlayer->turns[i]))
		{
			PRINT_ERROR_STR_RETURN_0("reading turn from bin file")
		}
	}
	// Player final score
	if (fread(&pPlayer->finalScore, sizeof(int), 1, fp) != 1)
	{
		PRINT_ERROR_STR_RETURN_0("reading player final score from bin file")
	}
	return 1;
}

int writeTurnToBinFile(FILE* fp, const Turn* pTurn)
{
	//int flag = (int)fwrite...;
	if (!fwrite(pTurn, sizeof(Turn), 1, fp))
	{
		PRINT_ERROR_STR_RETURN_0("writing turn to bin file")
	}
	return 1;
}
int readTurnFromBinFile(FILE* fp, Turn* pTurn)
{
	if (!fread(pTurn, sizeof(Turn), 1, fp))
	{
		PRINT_ERROR_STR_RETURN_0("reading turn to bin file")
	}
	return 1;
}

int writeDateToBinFile(FILE* fp, const Date* pDate)
{
	if (!fwrite(pDate, sizeof(Date), 1, fp))
	{
		PRINT_ERROR_STR_RETURN_0("writing date to bin file")
	}
	return 1;
}
int readDateFromBinFile(FILE* fp, Date* pDate)
{
	if (!fread(pDate, sizeof(Date), 1, fp))
	{
		PRINT_ERROR_STR_RETURN_0("reading date to bin file")
	}
	return 1;
}


int writeStrToBinFile(FILE* fp, char* str, int isCode)
{
	int len = (int)strlen(str);
	if (!isCode) // if not code its name and name need '\0'
		len++;
	if (fwrite(&len, sizeof(int), 1, fp) != 1)
		return  0;
	if (fwrite(str, sizeof(char), len, fp) != len)
		return 0;
	return 1;
}
// get a char* str (need to free if something failed outside after this func' was used)
// no back slash
int readStrFromBinFile(FILE* fp, char* str)
{
	int len = 0;
	if (fread(&len, sizeof(int), 1, fp) != 1)
		return  0;
	if (len == 0)
	{
		PRINT_ERROR_STR_RETURN_0("no String to read from bin file (len == 0)")
	}
	if (fread(str, sizeof(char), len, fp) != len)
	{
		PRINT_ERROR_STR_RETURN_0("readaing String from bin file")
	}
	return 1;
}

// char str[1] instead of char*
int writeCodeStrToBinFile(FILE* fp, const char* str)
{
	if (fwrite(str, sizeof(char), 1, fp) != 1)
		return 0;
	return 1;
}
int readCodeStrFromBinFile(FILE* fp, char* str)
{
	if (fread(str, sizeof(char), 1, fp) != 1)
		return 0;
	*(str + 1) = '\0';
	return 1;
}

////	////	////

// compressed Turn:
int	writeTurnToCompressedBinFile(FILE* fp, const Turn* pTurn)
{
	BYTE data[3] = { 0 };

	data[0] = pTurn->attempts[2] & 0xF;
	data[1] = ((pTurn->attempts[1] & 0xF) << 4) | (pTurn->attempts[0] & 0xF);
	data[2] = ((pTurn->finalTurnScore & 0x1F) << 3) | ((pTurn->isStrike & 0x1) << 2) | ((pTurn->isSpare & 0x1) << 1) | (pTurn->isLastTurn & 0x1);

	if (fwrite(&data, sizeof(BYTE), 3, fp) != 3)
		return 0;

	return 1;
}

int readTurnFromCompressedBinFile(FILE* fp, Turn* pTurn)
{
	BYTE data[3];
	if (fread(&data, sizeof(BYTE), 3, fp) != 3)
		return 0;

	pTurn->attempts[2] = data[0] & 0xF;
	pTurn->attempts[1] = (data[1] >> 4) & 0xF;
	pTurn->attempts[0] = data[1] & 0xF;
	pTurn->finalTurnScore = (data[0] >> 3) & 0x1F;
	pTurn->isStrike = (data[0] >> 2) & 0x1;
	pTurn->isSpare = (data[0] >> 1) & 0x1;
	pTurn->isLastTurn = data[0] & 0x1;

	return 1;
}