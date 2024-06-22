#include <stdio.h>
#include <stdlib.h>

#include "BowlingCompany.h"
#include "General.h"

int initBowlingCompany(BowlingCompany* pComp)
{
	printf("-----------  Init Bowling Company\n");
	pComp->name = getStrExactName("Enter bowling company name: ");
	pComp->founderName = getStrExactName("Enter bowling company founder name: ");
	Branch* pBranch = (Branch*)calloc(1, sizeof(Branch));
	if (!pBranch)
	{
		ALLOC_ERROR_RETURN()
	}
	initOfficeBranch(pBranch);
	pComp->officeBranch = pBranch;
	Employee* pCeo = (Employee*)malloc(sizeof(Employee));
	if (!pCeo)
	{
		ALLOC_ERROR_RETURN()
	}

	initCEOEmployee(pCeo);
	pComp->ceo = *pCeo;
	pComp->branchArr = NULL;
	pComp->numOfBranches = 0;
	return 1;
}
int	addBranch(BowlingCompany* pComp)
{
	Branch* pBranch = (Branch*)calloc(1, sizeof(Branch));
	if (!pBranch)
	{
		ALLOC_ERROR_RETURN()
	}

	initBranch(pBranch, pComp->branchArr, pComp->numOfBranches);

	pComp->branchArr = (Branch**)realloc(pComp->branchArr, (pComp->numOfBranches + 1) * sizeof(Branch*));
	if (!pComp->branchArr)
	{
		free(pBranch);
		return 0;
	}
	pComp->branchArr[pComp->numOfBranches] = pBranch;
	pComp->numOfBranches++;
	pComp->branchSortOpt = eNone;
	return 1;
}
int	removeBranch(BowlingCompany* pComp)
{
	if (!pComp->numOfBranches)
	{
		PRINT_ERROR_STR_RETURN_0("No branches to remove!")
	}
	sortBranches(pComp);
	Branch* pBranch = findBranch(pComp);
	if (!pBranch)
		return 0;
	int i = 0;
	for (i = 0; i < pComp->numOfBranches; i++)  // find index of pBranch
		if (pBranch == pComp->branchArr[i])
			break;
	freeBranch(pBranch);
	pComp->branchArr[i] = pComp->branchArr[pComp->numOfBranches - 1];
	pComp->numOfBranches--;
	pComp->branchArr = (Branch**)realloc(pComp->branchArr, (pComp->numOfBranches) * sizeof(Branch*));
	if (!pComp->branchArr && pComp->numOfBranches)
		return 0;

	return 1;
}

Branch* findBranch(const BowlingCompany* pComp)
{
	int(*compare)(const void* branch1, const void* branch2) = NULL;
	Branch b = { 0 };
	Branch* pBranch = &b;

	switch (pComp->branchSortOpt)
	{
	case eSN:
		printf("%s\t", "Serial Number:");
		getBranchSerialNum(pBranch);
		compare = compareBranchBySerialNumber;
		break;

	case eEmp:
		printf("%s\t", "Number of Employees:");
		getBranchNumOfEmployees(pBranch);
		compare = compareBranchByNumOfEmployees;
		break;

	case eLane:
		printf("%s\t", "Number of Lanes:");
		getBranchNumOfLanes(pBranch);
		compare = compareBranchByNumOfLanes;
		break;

	}

	if (compare != NULL)
	{
		Branch** pB = bsearch(&pBranch, pComp->branchArr, pComp->numOfBranches, sizeof(Branch*), compare);
		if (pB == NULL)
			printf("Branch was not found\n");
		else
		{
			printf("Branch found, ");
			printBranch(*pB);
			return *pB;
		}
	}
	else
	{
		printf("The search cannot be performed, array not sorted\n");
	}
	return NULL;
}
void sortBranches(BowlingCompany* pComp)
{
	pComp->branchSortOpt = showSortMenu();
	sortBranchesByType(pComp, pComp->branchSortOpt);
}
void sortBranchesByType(BowlingCompany* pComp, int sortType)
{
	int(*compare)(const void* branch1, const void* branch2) = NULL;
	switch (sortType)
	{
	case eSN:
		compare = compareBranchBySerialNumber;
		break;
	case eEmp:
		compare = compareBranchByNumOfEmployees;
		break;
	case eLane:
		compare = compareBranchByNumOfLanes;
		break;
	}

	if (compare != NULL)
		qsort(pComp->branchArr, pComp->numOfBranches, sizeof(Branch*), compare);
	printBranches(pComp);
}
eSortOption showSortMenu()
{
	int opt;
	printf("Base on what field do you want to sort?\n");
	do
	{
		for (int i = 1; i < eNofSortOpt; i++)
			printf("Enter %d for %s\n", i, sortOptStr[i]);
		scanf("%d", &opt);
	} while (opt < 0 || opt >= eNofSortOpt);

	return (eSortOption)opt;
}

void printBranches(const BowlingCompany* pComp)
{
	if (!pComp->numOfBranches)
		printf("No branches to display!\t Add branches via menu first.\n");
	else
	{
		for (int i = 0; i < pComp->numOfBranches; i++)
			printBranch(pComp->branchArr[i]);
	}
}
void printBowlingCompany(const BowlingCompany* pComp)
{
	printf("Bowling company name:%-20s\t founder name:%-20s\t Number of branches: %d\n\n", pComp->name, pComp->founderName, pComp->numOfBranches);
	printf("CEO info:\n");
	printEmployee(&(pComp->ceo));
	printf("\nCompany office info:\n");
	printBranch(pComp->officeBranch);
	printf("\n");
}
void freeBowlingCompany(BowlingCompany* pComp)
{
	generalArrayFunction(pComp->branchArr, pComp->numOfBranches, sizeof(Branch*), freeBranchV);
	free(pComp->branchArr);
	free(pComp->founderName);
	free(pComp->name);
	freeEmployee(&(pComp->ceo));
	freeBranch(pComp->officeBranch);
}
