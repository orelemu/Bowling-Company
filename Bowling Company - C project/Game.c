#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "General.h"
#include "Game.h"


int	initGame(Game* pGame)
{
	Date date;
	getCorrectDate(&date);
	pGame->date = date;
	pGame->winner = NULL;

	if (!getPlayers(pGame))
	{
		ALLOC_ERROR_RETURN()
	}
	getWinner(pGame);

	return 1;
}
int	getPlayers(Game* pGame)
{
	int numOfPlayers = 0;

	if (!L_init(&pGame->playersList))
	{
		ALLOC_ERROR_RETURN()
	}

	do
	{
		printf("Enter number of players between 1 and %d: ", MAX_PLAYERS);
		scanf("%d", &numOfPlayers);

		// Check if the input is within the specified range
		if (numOfPlayers <= 0 || numOfPlayers > 6)
			printf("Invalid input. Please enter an integer between 1 and 6.\n");

	} while (numOfPlayers <= 0 || numOfPlayers > 6);
	pGame->numOfPlayers = numOfPlayers;

	NODE* pNode = &pGame->playersList.head;
	for (int i = 0; i < numOfPlayers; i++)
	{
		Player* pPlayer = (Player*)calloc(1, sizeof(Player));
		if (!pPlayer)
		{
			ALLOC_ERROR_RETURN()
		}
		initPlayer(pPlayer);
		if (i)
			pNode = pNode->next;
		if (!L_insert(pNode, pPlayer))
		{
			ALLOC_ERROR_RETURN();
		}
	}
	return 1;
}
Player* getWinner(Game* pGame)
{
	NODE* pNode = &(pGame->playersList.head);
	NODE* pMaxNode = L_findMax(pNode, comparePlayers);
	pGame->winner = (Player*)(pMaxNode->key);
	return pGame->winner;
}

int	compareGamesByDate(Date date1, Date date2)
{
	return compareDate(&date1, &date2);
}

void printGame(const Game* pGame)
{
	printGameSummary(pGame);
	printGameScoreboard(pGame);
}
void printGameSummary(const Game* pGame)
{
	printf("\nGame happened at ");
	printDate(&(pGame->date));
	printf("\nScoring summary:");

	L_print(&pGame->playersList, printPlayer);

	printf("The winner is %s with a score of %d!\n", pGame->winner->playerName, pGame->winner->finalScore);
}
void printGameScoreboard(const Game* pGame)
{
	printf("\nGame happened at ");
	printDate(&(pGame->date));
	printf("Scoreboard:");

	L_print(&pGame->playersList, printPlayerScoreboard);

	printf("The winner is %s with a score of %d !", pGame->winner->playerName, pGame->winner->finalScore);
}

void freeGame(Game* pGame)
{
	L_free(&pGame->playersList, freePlayer);  // can make error maybe?

	IF_NOT_NULL_FREE(pGame);
}
void freeGameV(void* ppGame)
{
	freeGame(*((Game**)ppGame));
}
