#pragma once
#include "Employee.h"
#include "Branch.h"

typedef enum {
	eNone, eSN, eEmp, eLane, eNofSortOpt
} eSortOption;

static const char* sortOptStr[eNofSortOpt] = {
	"None","Serial Number", "Number of Employees", "Number of Lanes" };


typedef struct
{
	char* name;
	char* founderName;
	Employee	ceo;
	Branch** branchArr;
	int			numOfBranches;
	Branch* officeBranch;
	eSortOption branchSortOpt;
} BowlingCompany;

int		initBowlingCompany(BowlingCompany* pComp);
int		addBranch(BowlingCompany* pComp);
int		removeBranch(BowlingCompany*);

Branch* findBranch(const BowlingCompany* pComp);
void		sortBranches(BowlingCompany* pComp);
void		sortBranchesByType(BowlingCompany* pComp, int sortType);
eSortOption	showSortMenu();

void		printBranches(const BowlingCompany* pComp);
void		printBowlingCompany(const BowlingCompany* pComp);
void		freeBowlingCompany(BowlingCompany* pComp);
