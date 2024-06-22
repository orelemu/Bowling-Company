#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>

#include "main.h"
#include "General.h"
#include "BowlingCompany.h"

#include "Game.h"

#define COMPANY_TEXT_FILE_NAME_SRC "BowlingCompanyText.txt"
#define COMPANY_BIN_FILE_NAME_SRC "BowlingCompanyBin.bin"

#define COMPANY_TEXT_FILE_NAME_DST "BowlingCompanyText_out.txt"
#define COMPANY_BIN_FILE_NAME_DST "BowlingCompanyBin_out.bin"


int main()
{
	srand(time(NULL));

	BowlingCompany	company;
	Branch* pBranch;

	// Instruction for using files:
	int readFromFile = 0;	// 0 to init without reading from any file
	//readFromFile = 1;	// change to 1 to read from text
	//readFromFile = 2;	// change to 2 to read from bin

	// if init from file didnt work will init without reading from files.

	if (!initCompany(&company, readFromFile))
	{
		PRINT_ERROR_STR_RETURN_0("init company")
	}

	int option;
	int optionBranch;
	int stop = 0;
	int stopBranch = 0;

	do
	{
		option = menu();
		switch (option)
		{
		case eAddBranch:
			if (!addBranch(&company))
				printf("Error adding branch\n");
			break;

		case eRemoveBranch:
			if (!removeBranch(&company))
				printf("Error remove branch\n");
			break;

		case eSortBranches:
			sortBranches(&company);
			break;

		case eFindBranch:
			findBranch(&company);
			break;

		case ePrintCompany:
			printBowlingCompany(&company);
			printBranches(&company);
			break;

		case eChangeOfficeName:
			if (!changeBranchName(company.officeBranch))
				printf("Error changing office name\n");
			break;

		case eChangeOfficeAddress:
			if (!changeBranchAddress(company.officeBranch))
				printf("Error changing office address\n");
			break;

		case eBranchMenu:
			printBranches(&company);
			sortBranches(&company);
			pBranch = findBranch(&company);
			if (pBranch) {
				do {
					optionBranch = branchMenu();
					switch (optionBranch)
					{
					case ePlayGame:
						if (!addGameToLane(pBranch))
							printf("Error Playing Game\n");
						break;
					case eAddEmployee:
						if (!addEmployee(pBranch))
							printf("Error adding employee to branch\n");
						break;
					case eRemoveEmployee:
						if (!removeEmployee(pBranch))
							printf("Error removing employee from branch\n");
						break;
					case ePrintEmployees:
						printEmployees(pBranch);
						break;
					case eAddLane:
						if (!addLane(pBranch))
							printf("Error adding lane to branch\n");
						break;
					case eRemoveLane:
						if (!removeLane(pBranch))
							printf("Error removing lane from branch\n");
						break;
					case ePrintLanes:
						printLanes(pBranch);
						break;
					case eChangeBranchName:
						if (!changeBranchName(pBranch))
							printf("Error changing branch name\n");
						break;
					case eChangeBranchAddress:
						if (!changeBranchAddress(pBranch))
							printf("Error changing branch address\n");
						break;
					case ePrintBranch:
						printBranch(pBranch);
						break;

					case EXIT:
						printf("Going back to Bowling Company menu.\n");
						stopBranch = 1;
						break;

					default:
						printf("Wrong option\n");
						break;
					}
				} while (!stopBranch);
			}
			stopBranch = 0;
			break;
		case EXIT:
			stop = 1;
			// save to text
			if (!saveCompanyToFile(&company, COMPANY_TEXT_FILE_NAME_DST))
			{
				printf("The company has not been saved to text file");
			}
			// save to bin
			if (!saveCompanyToBinFile(&company, COMPANY_BIN_FILE_NAME_DST))
			{
				printf("The company has not been saved to bin file");
			}
			freeBowlingCompany(&company);
			printf("Bye bye\n");
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	printf("before dump\n");
	_CrtDumpMemoryLeaks();  // from HW3 project
	return 1;
}

int initCompany(BowlingCompany* pComp, int initType)
{
	// 1 == read from text
	if (initType == 1)
	{
		if (!initCompanyFromFile(pComp, COMPANY_TEXT_FILE_NAME_SRC))
		{
			printf("Error reading company from text file");
			initType = 0;
			freeBowlingCompany(pComp); // free all company before new init from scrach
		}
	}
	// 2 == read from bin
	if (initType == 2)
	{
		if (!initCompanyFromBinFile(pComp, COMPANY_BIN_FILE_NAME_SRC))
		{
			printf("Error reading company from text file");
			initType = 0;
			freeBowlingCompany(pComp); // free all company before new init from scrach
		}
	}
	// 0 == init company
	if (!initType)
		return initBowlingCompany(pComp);

	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofOptions; i++)
		printf("%d - %s\n", i, strCompanyMenu[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

int branchMenu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofBranchOptions; i++)
		printf("%d - %s\n", i, strBranchMenu[i]);
	printf("%d - Quit Branch Menu\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}
