#pragma once

#include "Lane.h"
#include "Employee.h"

#define SEP_CHAR '_'

typedef struct
{
	char* branchName;
	char* branchAddress;
	int			branchSerialNum;
	Employee** employeeArr;
	int			numOfEmployees;
	Lane** lanesArr;
	int			numOfLanes;
} Branch;

int		initBranch(Branch* pBranch, Branch** pBranchArr, int numOfElememnts);
int		initOfficeBranch(Branch* pBranch);

int		getBranchSerialNum(Branch* pBranch);
int		getBranchUniqueSerialNum(Branch* pBranch, Branch** pBranchArr, int numOfElememnts);
int		isBranchUnique(int serialNum, Branch** pBranchArr, int numOfElememnts);

int		getBranchNumOfEmployees(Branch* pBranch);
int		getBranchEmployees(Branch* pBranch); //.
int		getBranchNumOfLanes(Branch* pBranch);
int		getBranchLanes(Branch* pBranch);

int		addEmployee(Branch* pBranch);
int		removeEmployee(Branch* pBranch);

int		addLane(Branch* pBranch);
int		removeLane(Branch* pBranch);

int		addGameToLane(Branch* pBranch);

int		changeBranchName(Branch* pBranch);
int		changeBranchAddress(Branch* pBranch);

int		compareBranchBySerialNumber(const void* branch1, const void* branch2);
int		compareBranchByNumOfEmployees(const void* branch1, const void* branch2);
int		compareBranchByNumOfLanes(const void* branch1, const void* branch2);
int 	compareBranchesBySNEmployeesLanes(int, int);

void	printEmployees(const Branch* pBranch);
void	printLanes(const Branch* pBranch);
void	printLanesCodesWithoutGames(const Branch* pBranch);
void	printBranch(const Branch* pBranch);
void	printBranchV(const void* val);

void	freeBranch(Branch* pBranch);
void	freeBranchV(void* val);