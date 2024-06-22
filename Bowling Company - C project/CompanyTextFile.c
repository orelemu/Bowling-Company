#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CompanyTextFile.h"
#include "General.h"


int saveCompanyToFile(const BowlingCompany* pCompany, const char* fileName)
{
	FILE* fp = fopen(fileName, "w");
	if (!fp)
		return 0;

	// Company & founder names
	fputs(pCompany->name, fp); // fprintf(fp, "\n");
	fprintf(fp, "\n");
	fputs(pCompany->founderName, fp);
	fprintf(fp, "\n");

	// CEO
	if (!writeEmployeeToTextFile(fp, &pCompany->ceo))
	{
		fclose(fp);
		PRINT_ERROR_STR_RETURN_0("writing employee(ceo) to file")
	}
	// Office branch
	if (!writeBranchToTextFile(fp, pCompany->officeBranch))
	{
		fclose(fp);
		PRINT_ERROR_STR_RETURN_0("writing branch(office) to file")
	}
	// All branches (branch arr)
	fprintf(fp, "%d\n", pCompany->numOfBranches);
	for (int i = 0; i < pCompany->numOfBranches; i++)
	{
		if (!writeBranchToTextFile(fp, pCompany->branchArr[i]))
		{
			fclose(fp);
			PRINT_ERROR_STR_RETURN_0("writing branch to file")
		}
	}

	// sort type
	int sortType = pCompany->branchSortOpt;
	fprintf(fp, "%d\n", sortType);

	fclose(fp);
	return 1;
}
int initCompanyFromFile(BowlingCompany* pCompany, const char* fileName)
{
	//int numOfElements = 0;
	FILE* fp = fopen(fileName, "r");
	if (!fp)
		return 0;

	char tempName[MAX_STR_LEN] = { 0 }, tempFounderName[MAX_STR_LEN] = { 0 };

	// Company & founder names
	if (!myGetsFile(tempName, MAX_STR_LEN, fp)) return 0;
	if (!myGetsFile(tempFounderName, MAX_STR_LEN, fp)) return 0;
	pCompany->name = strdup(tempName);
	pCompany->founderName = strdup(tempFounderName);

	// CEO
	Employee* pEmp = (Employee*)malloc(sizeof(Employee));
	if (pEmp == NULL)
	{
		fclose(fp);
		ALLOC_ERROR_RETURN()
	}
	if (!readEmployeeFromTextFile(fp, pEmp))
	{
		fclose(fp);
		PRINT_ERROR_STR_RETURN_0("reading Employee(CEO) from file")
	}
	pCompany->ceo = *pEmp;


	// Office branch
	pCompany->officeBranch = (Branch*)malloc(sizeof(Branch));
	if (pCompany->officeBranch == NULL)
	{
		fclose(fp);
		ALLOC_ERROR_RETURN()
	}
	if (!readBranchFromTextFile(fp, pCompany->officeBranch))
	{
		fclose(fp);
		PRINT_ERROR_STR_RETURN_0("reading Branch(Office) from file")
	}

	// All branches (branch arr)
	if (fscanf(fp, "%d", &pCompany->numOfBranches) != 1)
		return 0;

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

		if (!readBranchFromTextFile(fp, pCompany->branchArr[i]))
		{
			fclose(fp);
			PRINT_ERROR_STR_RETURN_0("reading Branchs arr from file")
		}
	}

	// get sort type & sort
	int sortType = 0;
	if (fscanf(fp, "%d", &sortType) != 1)
		return 0;
	pCompany->branchSortOpt = (eSortOption)sortType;
	sortBranchesByType(pCompany, sortType);

	fclose(fp);
	return 1;
}

int writeBranchToTextFile(FILE* fp, const Branch* pBranch)
{
	// Branch name
	fputs(pBranch->branchName, fp);
	fprintf(fp, "\n");
	// Branch address
	fputs(pBranch->branchAddress, fp);
	fprintf(fp, "\n");
	// Branch serial num
	fprintf(fp, "%d\n", pBranch->branchSerialNum);

	// All employees (employee arr)
	fprintf(fp, "%d\n", pBranch->numOfEmployees);
	for (int i = 0; i < pBranch->numOfEmployees; i++)
	{
		if (!writeEmployeeToTextFile(fp, pBranch->employeeArr[i]))
		{
			PRINT_ERROR_STR_RETURN_0("writing employee to file")
		}
	}
	// All lanes (lane arr)
	fprintf(fp, "%d\n", pBranch->numOfLanes);
	for (int i = 0; i < pBranch->numOfLanes; i++)
	{
		if (!writeLaneToTextFile(fp, pBranch->lanesArr[i]))
		{
			PRINT_ERROR_STR_RETURN_0("writing lane to file")
		}
	}

	return 1;
}
int readBranchFromTextFile(FILE* fp, Branch* pBranch)
{
	// Branch name
	char tempName[MAX_STR_LEN] = { 0 }, tempAddress[MAX_STR_LEN] = { 0 };
	if (!myGetsFile(tempName, MAX_STR_LEN, fp)) return 0;
	pBranch->branchName = strdup(tempName);

	// Branch address
	if (!myGetsFile(tempAddress, MAX_STR_LEN, fp)) return 0;
	pBranch->branchAddress = strdup(tempAddress);

	// Branch serial num
	int serialNum = 0;
	if (fscanf(fp, "%d", &serialNum) != 1)
		return 0;
	pBranch->branchSerialNum = serialNum;

	// All employees (employee arr)
	if (fscanf(fp, "%d", &pBranch->numOfEmployees) != 1)
		return 0;

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

		if (!readEmployeeFromTextFile(fp, pBranch->employeeArr[i]))
		{
			PRINT_ERROR_STR_RETURN_0("reading Employee from file")
		}
	}

	// All Lanes (lane arr)
	if (fscanf(fp, "%d", &pBranch->numOfLanes) != 1)
		return 0;

	pBranch->lanesArr = (Lane**)malloc(sizeof(Lane*) * pBranch->numOfLanes);
	if (!pBranch->lanesArr)
	{
		ALLOC_ERROR_RETURN()
	}

	for (int i = 0; i < pBranch->numOfLanes; i++)
	{
		pBranch->lanesArr[i] = (Lane*)malloc(sizeof(Lane));
		// branch need to have atleast 2 lanes and gets initilized with 1 lane
		if (!pBranch->lanesArr[i])
		{
			ALLOC_ERROR_RETURN()
		}
		if (pBranch->numOfLanes == 1) // <=1
		{
			pBranch->lanesArr[0] = NULL;
			return 1;
		}
		if (!readLaneFromTextFile(fp, pBranch->lanesArr[i]))
		{
			PRINT_ERROR_STR_RETURN_0("reading Lane from file")
		}
	}

	return 1;
}

int writeEmployeeToTextFile(FILE* fp, const Employee* pEmployee)
{
	// Employee name
	fputs(pEmployee->empName, fp);
	fprintf(fp, "\n");
	// Employee id
	fprintf(fp, "%d\n", pEmployee->empID);
	// Employee salary
	fprintf(fp, "%lf\n", pEmployee->salary);

	// Employee type
	int empType = pEmployee->empType;
	fprintf(fp, "%d\n", empType);

	return 1;
}
int readEmployeeFromTextFile(FILE* fp, Employee* pEmployee)
{
	// Employee name
	char tempName[MAX_STR_LEN] = { 0 };
	if (!myGetsFile(tempName, MAX_STR_LEN, fp)) return 0;
	pEmployee->empName = strdup(tempName);

	int empID = 0, empType = 0;
	double empSalary = 0;
	// Employee id
	if (fscanf(fp, "%d", &empID) != 1)
		return 0;
	pEmployee->empID = empID;

	// Employee salary
	if (fscanf(fp, "%lf", &empSalary) != 1)
		return 0;
	pEmployee->salary = empSalary;

	// Employee type
	if (fscanf(fp, "%d", &empType) != 1)
		return 0;
	pEmployee->empType = (eEmpType)empType;

	return 1;
}

int writeLaneToTextFile(FILE* fp, const Lane* pLane)
{
	// Lane code
	fputc(*pLane->laneCode, fp);
	fprintf(fp, "\n");
	// Lane best score
	fprintf(fp, "%d\n", pLane->bestScore);

	// Lane games (game arr)
	fprintf(fp, "%d\n", pLane->numOfGames);
	for (int i = 0; i < pLane->numOfGames; i++)
	{
		if (!writeGameToTextFile(fp, pLane->games[i]))
		{
			PRINT_ERROR_STR_RETURN_0("writing game to file")
		}
	}


	return 1;
}
int readLaneFromTextFile(FILE* fp, Lane* pLane)
{
	// lne code
	char tempCode[MAX_STR_LEN] = { 0 };
	if (!myGetsFile(tempCode, MAX_STR_LEN, fp)) return 0;
	pLane->laneCode[0] = *tempCode;
	pLane->laneCode[1] = '\0';

	// Lane best score
	int laneBestScore = 0;
	if (fscanf(fp, "%d", &laneBestScore) != 1)
		return 0;
	pLane->bestScore = laneBestScore;

	// All games (game arr)
	if (fscanf(fp, "%d", &pLane->numOfGames) != 1)
		return 0;

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
		if (!readGameFromTextFile(fp, pLane->games[i]))
		{
			PRINT_ERROR_STR_RETURN_0("reading Game from file")
		}
	}

	return 1;
}

int writeGameToTextFile(FILE* fp, const Game* pGame)
{
	// winner player
	if (!writePlayerToTextFile(fp, pGame->winner))
	{
		PRINT_ERROR_STR_RETURN_0("writing player(winner) to file")
	}

	// All players (player list)
	fprintf(fp, "%d\n", pGame->numOfPlayers);
	if (!writeNodeToTextFile(fp, &(pGame->playersList.head)))
	{
		PRINT_ERROR_STR_RETURN_0("writing player node to file")
	}

	// game date
	if (!writeDateToTextFile(fp, &pGame->date))
	{
		PRINT_ERROR_STR_RETURN_0("writing date to file")
	}

	return 1;
}
int readGameFromTextFile(FILE* fp, Game* pGame)
{
	// winner player
	pGame->winner = (Player*)malloc(sizeof(Player));
	if (pGame->winner == NULL)
	{
		ALLOC_ERROR_RETURN()
	}
	if (!readPlayerFromTextFile(fp, pGame->winner))
	{
		PRINT_ERROR_STR_RETURN_0("reading Player(winner) from file")
	}

	// All players (player list)
	if (!L_init(&pGame->playersList))
	{
		ALLOC_ERROR_RETURN()
	}
	if (!readNodesFromTextFile(fp, &pGame->playersList, &pGame->numOfPlayers))
	{
		PRINT_ERROR_STR_RETURN_0("reading Players Nodes from file")
	}

	// game date
	if (!readDateFromTextFile(fp, &pGame->date))
	{
		PRINT_ERROR_STR_RETURN_0("reading Date from file")
	}

	return 1;
}

// used for playersList
int writeNodeToTextFile(FILE* fp, const NODE* pNode)
{
	// pNode = header
	NODE* current = pNode->next;
	while (current != NULL)
	{
		writePlayerToTextFile(fp, (Player*)(current->key));
		current = current->next;
	}
	return 1;
}
int readNodesFromTextFile(FILE* fp, LIST* pPlayerList, int* pCount)
{
	NODE* pNode = &(pPlayerList->head);
	if (fscanf(fp, "%d", pCount) != 1)
		return 0;

	for (int i = 0; i < *pCount; i++)
	{
		Player* pPlayer = (Player*)malloc(sizeof(Player));

		if (!pPlayer)
		{
			L_free(pPlayerList, freePlayer);
			return 0;
		}
		if (!readPlayerFromTextFile(fp, pPlayer))
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

int writePlayerToTextFile(FILE* fp, const Player* pPlayer)
{
	// Player name
	fputs(pPlayer->playerName, fp);
	fprintf(fp, "\n");

	// Player turns (Turn arr |10|)
	fprintf(fp, "%d\n", TURNS);
	for (int i = 0; i < TURNS; i++)
	{
		if (!writeTurnToTextFile(fp, pPlayer->turns + i))
		{
			PRINT_ERROR_STR_RETURN_0("writing turn to file")
		}
	}

	// Player final score
	fprintf(fp, "%d\n", pPlayer->finalScore);

	return 1;
}
int readPlayerFromTextFile(FILE* fp, Player* pPlayer)
{
	// Player name
	char tempName[MAX_STR_LEN] = { 0 };
	if (!myGetsFile(tempName, MAX_STR_LEN, fp)) return 0;
	pPlayer->playerName = strdup(tempName);

	// Player turns (Turn arr |10|)
	int turns = 0;
	if (fscanf(fp, "%d", &turns) != 1)
		return 0;
	if (turns != TURNS)
	{
		PRINT_ERROR_STR_RETURN_0("reading turns from file")
	}

	for (int i = 0; i < TURNS; i++)
	{
		if (!readTurnFromTextFile(fp, pPlayer->turns + i))
		{
			PRINT_ERROR_STR_RETURN_0("reading turn from file")
		}
	}

	// Player final score
	if (fscanf(fp, "%d", &pPlayer->finalScore) != 1)
		return 0;

	return 1;
}

int writeTurnToTextFile(FILE* fp, const Turn* pTurn)
{
	// Attempts [3]
	fprintf(fp, "%d\n", ATTEMPTS);
	for (int i = 0; i < ATTEMPTS; i++)
	{
		fprintf(fp, "%d\n", pTurn->attempts[i]);
	}
	// turn final score
	fprintf(fp, "%d\n", pTurn->finalTurnScore);

	// isStrike, isSpare, isLastTurn
	fprintf(fp, "%d\n", pTurn->isStrike);
	fprintf(fp, "%d\n", pTurn->isSpare);
	fprintf(fp, "%d\n", pTurn->isLastTurn);


	return 1;
}
int readTurnFromTextFile(FILE* fp, Turn* pTurn)
{
	// Attempts [3]
	int attempts = 0;
	if (fscanf(fp, "%d", &attempts) != 1)
		return 0;
	if (attempts != ATTEMPTS)
		return 0;
	for (int i = 0; i < ATTEMPTS; i++)
	{
		if (fscanf(fp, "%d", pTurn->attempts + i) != 1)
			return 0;
	}
	// turn final score
	if (fscanf(fp, "%d", &pTurn->finalTurnScore) != 1)
		return 0;

	// isStrike, isSpare, isLastTurn
	if (fscanf(fp, "%d", &pTurn->isStrike) != 1)
		return 0;
	if (fscanf(fp, "%d", &pTurn->isSpare) != 1)
		return 0;
	if (fscanf(fp, "%d", &pTurn->isLastTurn) != 1)
		return 0;

	return 1;
}

int writeDateToTextFile(FILE* fp, const Date* pDate)
{
	fprintf(fp, "%d %d %d\n", pDate->day, pDate->month, pDate->year);
	return 1;
}
int readDateFromTextFile(FILE* fp, Date* pDate)
{
	// Employee i
	if (fscanf(fp, "%d %d %d", &pDate->day, &pDate->month, &pDate->year) != 3)
		return 0;

	return 1;
}
