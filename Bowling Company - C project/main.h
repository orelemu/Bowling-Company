#pragma once
#include "BowlingCompany.h"

#include "CompanyTextFile.h"
#include "CompanyBinFile.h"

typedef enum
{
	eAddBranch, eRemoveBranch, eSortBranches, eFindBranch, ePrintCompany, eChangeOfficeName, eChangeOfficeAddress, eBranchMenu, eNofOptions
} eCompanyOptions;

typedef enum
{
	ePlayGame, eAddEmployee, eRemoveEmployee, ePrintEmployees, eAddLane, eRemoveLane, ePrintLanes,
	eChangeBranchName, eChangeBranchAddress, ePrintBranch, eNofBranchOptions
} eBranchOptions;

const char* strCompanyMenu[eNofOptions] = { "Add Branch","Remove Branch","Sort Branches",
								"Find Branch", "Print Bowling Company","Change Office Name","Change Office Address", "Go To Branch Menu" };

const char* strBranchMenu[eNofBranchOptions] = { "Play Game","Add Employee To Branch","Remove Employee From Branch",
								"Print Employees", "Add Lane", "Remove Lane", "Print Lanes", "Change Branch Name",
	"Change Branch Address", "Print Branch" };

#define EXIT	-1
//#define MANAGER_FILE_NAME "airport_authority.txt"
//#define COMPANY_FILE_NAME "airline.bin"
#define COMPANY_FILE_NAME "bowling_authority.txt"

int initCompany(BowlingCompany* pComp, int initType);
int menu();
int branchMenu();



