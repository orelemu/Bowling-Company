#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

#include "General.h"
#include "Player.h"

// TODO: calculate scores accurately and write takeTurn function code
void initPlayer(Player* pPlayer)
{
	pPlayer->playerName = getStrExactName("Please enter the player's name:");
	pPlayer->finalScore = 0;
	for (int i = 0; i < TURNS; i++)
	{
		Turn turn;
		initTurn(&turn, i + 1);
		takeSingleTurn(&turn);
		pPlayer->turns[i] = turn;
	}
	calculateScore(pPlayer);
}

int comparePlayers(void* pPlayer1, void* pPlayer2)
{
	Player* p1 = (Player*)pPlayer1;
	Player* p2 = (Player*)pPlayer2;
	return comparePlayersByScore(p1->finalScore, p2->finalScore);
}
int	comparePlayersByScore(int score1, int score2)
{
	if (score1 == score2)
		return 0;
	else if (score1 > score2)
		return 1;
	return -1;
}
int	calculateScore(Player* pPlayer)
{
	int sum = 0;
	for (int i = 0; i < TURNS - 2; i++)
		sum += calculateFinalTurnScore(&(pPlayer->turns[i]), &(pPlayer->turns[i + 1]), &(pPlayer->turns[i + 2]));

	sum += calculateFinalTurnScore(&(pPlayer->turns[TURNS - 2]), &(pPlayer->turns[TURNS - 1]), NULL);
	sum += calculateFinalTurnScore(&(pPlayer->turns[TURNS - 1]), NULL, NULL);
	pPlayer->finalScore = sum;
	return sum;
}

void printPlayer(const Player* pPlayer)
{
	printf("Player name: %s, Final Score: %d\n", pPlayer->playerName, pPlayer->finalScore);
}
void printPlayerScoreboard(const Player* pPlayer)
{
	printf("%s|", pPlayer->playerName);
	for (int i = 0; i < TURNS; i++)
		printTurnAttempts(&(pPlayer->turns[i]));
	printf("\n    |");
	int sum = 0;
	for (int i = 0; i < TURNS; i++)
	{
		sum += pPlayer->turns[i].finalTurnScore;
		printf("%d| ", sum);
	}
	printf("Final Score: %d\n", sum);
}

void freePlayer(Player* pPlayer)
{
	free(pPlayer->playerName);
	pPlayer->playerName = NULL;
	IF_NOT_NULL_FREE(pPlayer)
}
void freePlayerV(void* pPlayer)
{
	freePlayer((Player*)pPlayer);
}

