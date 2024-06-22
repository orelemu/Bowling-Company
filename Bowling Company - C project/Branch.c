#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "Branch.h"
#include "General.h"
//#include "fileHelper.h"

int	initBranch(Branch* pBranch, Branch** pBranchArr, int numOfElememnts)
{
	pBranch->branchName = getStrExactName("Please enter the branch name:");
	pBranch->branchAddress = getStrExactName("Please enter the branch address:");
	getBranchUniqueSerialNum(pBranch, pBranchArr, numOfElememnts);
	getBranchEmployees(pBranch);
	getBranchLanes(pBranch);
	return 1;
}
// offices have no lanes and employees
int	initOfficeBranch(Branch* pBranch)
{
	pBranch->branchName = getStrExactName("Please enter the office name:");
	pBranch->branchAddress = getStrExactName("Please enter the office address:");
	getBranchSerialNum(pBranch);
	return 1;
}

int getBranchSerialNum(Branch* pBranch)
{
	pBranch->branchSerialNum = -1;
	do
	{
		printf("Enter branch Serial Number(positive integer): ");
		if (scanf("%d", &(pBranch->branchSerialNum)) != 1)
		{
			// If scanf fails, clear the input buffer
			while (getchar() != '\n');
		}

		// Check if the input is within the specified range
		if (pBranch->branchSerialNum < 0)
			printf("Invalid input. Please enter a positive integer .\n");
	} while (pBranch->branchSerialNum < 0);

	return 1;
}
int getBranchUniqueSerialNum(Branch* pBranch, Branch** pBranchArr, int numOfElememnts)
{
	do
	{
		getBranchSerialNum(pBranch);
		// Check if SN is unique
	} while (!isBranchUnique(pBranch->branchSerialNum, pBranchArr, numOfElememnts));
	return 1;
}
int isBranchUnique(int serialNumToCheck, Branch** pBranchArr, int numOfElememnts)
{
	for (int i = 0; i < numOfElememnts; i++)
	{
		if (serialNumToCheck == pBranchArr[i]->branchSerialNum)
		{
			PRINT_ERROR_STR_RETURN_0("serial number isn't unique, please try again")
		}
	}
	return 1;
}

int	getBranchNumOfEmployees(Branch* pBranch)
{
	pBranch->numOfEmployees = -1;
	do
	{
		printf("Enter how many employees you want to hire for the Branch? (positive integer, not including manager): ");
		if (scanf("%d", &(pBranch->numOfEmployees)) != 1)
		{
			// If scanf fails, clear the input buffer
			while (getchar() != '\n');
		}
		// Check if the input is within the specified range
		if (pBranch->numOfEmployees < 0)
			printf("Invalid input. Please enter a positive integer .\n");
	} while (pBranch->numOfEmployees < 0);
	pBranch->numOfEmployees++;
	return 1;
}
int	getBranchNumOfLanes(Branch* pBranch)
{
	do
	{
		printf("Enter how many lanes you want to build in the Branch(positive integer, greater than 1): ");
		if (scanf("%d", &(pBranch->numOfLanes)) != 1)
		{
			// If scanf fails, clear the input buffer
			while (getchar() != '\n');
		}
		if (pBranch->numOfLanes < 2) // <1
			printf("Invalid input. Please enter a positive integer that is greater than 1.\n"); // than 0
	} while (pBranch->numOfLanes < 2); // <1
	return 1;
}
int	getBranchEmployees(Branch* pBranch)
{
	getBranchNumOfEmployees(pBranch);
	pBranch->employeeArr = (Employee**)malloc((pBranch->numOfEmployees) * sizeof(Employee*));

	if (!(pBranch->employeeArr))
	{
		ALLOC_ERROR_RETURN()
	}

	// the first employee in a branch is a manager
	Employee* pManagerEmp = (Employee*)malloc(sizeof(Employee));
	if (!(pManagerEmp))
	{
		ALLOC_ERROR_RETURN()
	}

	initManagerEmployee(pManagerEmp);
	pBranch->employeeArr[0] = pManagerEmp;

	for (int i = 1; i < pBranch->numOfEmployees; i++)
	{
		Employee* pEmp = (Employee*)malloc(sizeof(Employee));
		if (!(pEmp))
		{
			ALLOC_ERROR_RETURN()
		}

		initEmployee(pEmp, pBranch->employeeArr, 1);// only manager in array
		pBranch->employeeArr[i] = pEmp;
	}
	return 1;
}
int	getBranchLanes(Branch* pBranch)
{
	getBranchNumOfLanes(pBranch);
	pBranch->lanesArr = (Lane**)malloc((pBranch->numOfLanes) * sizeof(Lane*));

	if (!(pBranch->lanesArr))
	{
		ALLOC_ERROR_RETURN()
	}

	for (int i = 0; i < pBranch->numOfLanes; i++)
	{
		Lane* pLane = (Lane*)malloc(sizeof(Lane));
		if (!pLane)
		{
			ALLOC_ERROR_RETURN()
		}
		initLane(pLane, pBranch->lanesArr, 0);
		pBranch->lanesArr[i] = pLane;
	}
	return 1;
}

int	addEmployee(Branch* pBranch)
{
	Employee* pEmp = (Employee*)malloc(sizeof(Employee));
	if (!(pEmp))
	{
		ALLOC_ERROR_RETURN()
	}

	if (!initEmployee(pEmp, pBranch->employeeArr, pBranch->numOfEmployees))
	{
		PRINT_ERROR_STR_RETURN_0("Employee initialization")

	}
	pBranch->employeeArr[pBranch->numOfEmployees] = pEmp;
	pBranch->numOfEmployees++;
	return 1;
}
int	removeEmployee(Branch* pBranch)
{
	if (!pBranch->numOfEmployees)
	{
		PRINT_ERROR_STR_RETURN_0("No employees to remove!")
	}
	printf("Choose a number from the following %d employees, or 0 to cancel:\n", pBranch->numOfEmployees);
	for (int i = 0; i < pBranch->numOfEmployees; i++)
	{
		printf("Employee No. %d: ", i + 1);
		printEmployee(pBranch->employeeArr[i]);
	}
	int selection = 0;
	printf("Enter your selection or 0 to cancel: ");
	scanf("%d", &selection);  //FREE BUFFER
	if (!selection)
	{
		PRINT_ERROR_STR_RETURN_0("remove employee from branch canceled!")
	}
	if (pBranch->numOfEmployees < selection || 0 > selection)
	{
		PRINT_ERROR_STR_RETURN_0("Invalid input. Canceling operation.")
	}
	--selection;
	free(pBranch->employeeArr[selection]);

	// Shift the elements after the removed item to fill the gap
	for (int i = selection; i < pBranch->numOfEmployees - 1; i++)
	{
		pBranch->employeeArr[i] = pBranch->employeeArr[i + 1];
	}
	Employee* pTemp = NULL;
	pBranch->employeeArr[pBranch->numOfEmployees - 1] = pTemp;

	pBranch->numOfEmployees--;
	if (!pBranch->employeeArr && pBranch->numOfEmployees)
		return 0;

	return 1;
}

int	addLane(Branch* pBranch)
{
	Lane* pLane = (Lane*)malloc(sizeof(Lane));
	if (!(pLane))
	{
		ALLOC_ERROR_RETURN()
	}
	if (!initLane(pLane, pBranch->lanesArr, pBranch->numOfLanes))
	{
		PRINT_ERROR_STR_RETURN_0("Lane initialization")
	}
	pBranch->lanesArr[pBranch->numOfLanes] = pLane;
	pBranch->numOfLanes++;

	return 1;
}
int	removeLane(Branch* pBranch)
{
	if (pBranch->numOfLanes == 1)
	{
		PRINT_ERROR_STR_RETURN_0("There must be at least one lane in branch!")
	}
	printf("Choose a number from the following %d lanes, or 0 to cancel:\n", pBranch->numOfLanes);
	for (int i = 0; i < pBranch->numOfLanes; i++)
	{
		printf("Lane No. %d: ", i + 1);
		printLane(pBranch->lanesArr[i]);
	}
	int selection = 0;
	printf("Enter your selection or 0 to cancel: ");
	scanf("%d", &selection);  //FREE BUFFER
	if (!selection)
	{
		PRINT_ERROR_STR_RETURN_0("remove lane from branch canceled!")
	}
	if (pBranch->numOfLanes < selection || 0 > selection)
	{
		PRINT_ERROR_STR_RETURN_0("Invalid input. Canceling operation.")
	}
	selection--;
	freeLane(pBranch->lanesArr[selection]);
	pBranch->lanesArr[selection] = pBranch->lanesArr[pBranch->numOfLanes - 1];
	pBranch->numOfLanes--;

	pBranch->lanesArr = (Lane**)realloc(pBranch->lanesArr, (pBranch->numOfLanes) * sizeof(Lane*));
	if (!pBranch->lanesArr)
		return 0;

	return 1;
}

int	addGameToLane(Branch* pBranch) {
	printf("Which lane do you want to play on?\n");
	printf("Choose a number from the following %d lanes, or -1 to cancel:\n", pBranch->numOfLanes);
	for (int i = 0; i < pBranch->numOfLanes; i++)
	{
		printf("Lane No. %d: ", i + 1);
		printLane(pBranch->lanesArr[i]);
	}
	int selection = 0;
	do
	{
		printf("Enter your selection or -1 to cancel: ");
		if (scanf("%d", &selection) != 1)
		{
			// If scanf fails, clear the input buffer
			while (getchar() != '\n');
		}
		if (selection == -1)
		{
			printf("Add game to lane cancelled.");
			return 0;
		}
		if (pBranch->numOfLanes < selection || -1 > selection || !selection)
			printf("Invalid input. Please enter a positive integer that is equal or less than %d or -1 to cancel.\n", pBranch->numOfLanes);
	} while (pBranch->numOfLanes < selection || -1 > selection || !selection);
	addGame(pBranch->lanesArr[selection - 1]);

	return 1;
}

int	changeBranchName(Branch* pBranch)
{
	free(pBranch->branchName);

	pBranch->branchName = getStrExactName("Please enter new branch name:");
	printf("Branch name changed to: %s", pBranch->branchName);

	return 1;
}
int	changeBranchAddress(Branch* pBranch)
{
	free(pBranch->branchAddress);
	printf("Enter new branch address: ");

	pBranch->branchAddress = getStrExactName("Please enter new branch address:");

	printf("Branch address changed to: %s", pBranch->branchAddress);
	return 1;
}

int	compareBranchBySerialNumber(const void* branch1, const void* branch2)
{
	const Branch* pBranch1 = *(const Branch**)branch1;
	const Branch* pBranch2 = *(const Branch**)branch2;
	return compareBranchesBySNEmployeesLanes(pBranch1->branchSerialNum, pBranch2->branchSerialNum);
}
int	compareBranchByNumOfEmployees(const void* branch1, const void* branch2)
{
	const Branch* pBranch1 = *(const Branch**)branch1;
	const Branch* pBranch2 = *(const Branch**)branch2;
	return compareBranchesBySNEmployeesLanes(pBranch1->numOfEmployees, pBranch2->numOfEmployees);
}
int	compareBranchByNumOfLanes(const void* branch1, const void* branch2)
{
	const Branch* pBranch1 = *(const Branch**)branch1;
	const Branch* pBranch2 = *(const Branch**)branch2;
	return compareBranchesBySNEmployeesLanes(pBranch1->numOfLanes, pBranch2->numOfLanes);
}
int	compareBranchesBySNEmployeesLanes(int n1, int n2)
{
	if (n1 == n2)
		return 0;
	else if (n1 > n2)
		return 1;
	return -1;
}

void printEmployees(const Branch* pBranch)
{
	printf("Employees(%d):\n", pBranch->numOfEmployees);
	generalArrayFunction(pBranch->employeeArr, pBranch->numOfEmployees, sizeof(Employee*), printEmployeeV);
}
void printLanes(const Branch* pBranch)
{
	printf("Lanes(%d):\n", pBranch->numOfLanes);
	generalArrayFunction(pBranch->lanesArr, pBranch->numOfLanes, sizeof(Lane*), printLaneV);
}
void printLanesCodesWithoutGames(const Branch* pBranch)
{
	for (int i = 0; i < pBranch->numOfLanes; i++)
		printf("Lane code: %c\n", pBranch->lanesArr[i]->laneCode[0]);
}
void printBranch(const Branch* pBranch)
{
	printf("\nBranch name:%-20s\t", pBranch->branchName);
	printf("Address: %-20s\t Serial Number:%d\n", pBranch->branchAddress, pBranch->branchSerialNum);
	printEmployees(pBranch);
	printLanes(pBranch);
	printf("\n");
}
void printBranchV(const void* val)
{
	const Branch* pBranch = (const Branch*)val;
	printBranch(pBranch);
}

void freeBranch(Branch* pBranch)
{
	generalArrayFunction(pBranch->lanesArr, pBranch->numOfLanes, sizeof(Lane*), freeLaneV);
	free(pBranch->lanesArr);
	generalArrayFunction(pBranch->employeeArr, pBranch->numOfEmployees, sizeof(Employee*), freeEmployeeV);
	free(pBranch->employeeArr);
	free(pBranch->branchName);
	free(pBranch->branchAddress);
	free(pBranch);
}
void freeBranchV(void* ppBranch)
{
	freeBranch(*((Branch**)ppBranch));
}


