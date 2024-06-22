#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include "General.h"
#include "Player.h"

void initTurn(Turn* pTurn, int turnNum)
{
	for (int i = 0; i < ATTEMPTS; i++)
	{
		pTurn->attempts[i] = 0;
	}
	pTurn->isStrike = 0;
	pTurn->isSpare = 0;
	pTurn->finalTurnScore = 0;
	pTurn->isLastTurn = (turnNum == TURNS) ? 1 : 0;
}

void takeSingleTurn(Turn* pTurn)
{
	int lastAttempt = getRandomScore(0);
	pTurn->attempts[0] = lastAttempt;
	if (pTurn->isLastTurn)  // special frame - frame 10
		takeLastTurn(pTurn, lastAttempt);
	else  // normal shot - frames 1-9
		takeNormalTurn(pTurn, lastAttempt);
}
void takeLastTurn(Turn* pTurn, int lastAttempt)
{
	if (lastAttempt == MAX_SCORE)
	{  // if strike - get two more rolls
		pTurn->isStrike = 1;
		lastAttempt = getRandomScore(0);
		pTurn->attempts[1] = lastAttempt;
		if (lastAttempt == MAX_SCORE)  // second strike - third shot is fresh
			lastAttempt = getRandomScore(0);
		else
			lastAttempt = getRandomScore(lastAttempt);
		pTurn->attempts[2] = lastAttempt;
	}
	else
	{
		lastAttempt = getRandomScore(lastAttempt);
		pTurn->attempts[1] = lastAttempt;
		if (lastAttempt + pTurn->attempts[0] == MAX_SCORE)
		{  // if spare - get one more roll
			pTurn->isSpare = 1;
			lastAttempt = getRandomScore(0);
			pTurn->attempts[2] = lastAttempt;
		}
	}
}
void takeNormalTurn(Turn* pTurn, int lastAttempt)
{
	if (lastAttempt == MAX_SCORE)  // strike
		pTurn->isStrike = 1;
	lastAttempt = getRandomScore(lastAttempt);
	pTurn->attempts[1] = lastAttempt;
	if (lastAttempt + pTurn->attempts[0] == MAX_SCORE)  // spare
		pTurn->isSpare = 1;
}

int calculateFinalTurnScore(Turn* pTurn1, Turn* pTurn2, Turn* pTurn3)
{
	int sum = 0;
	if (pTurn1->isLastTurn)
		sum = pTurn1->attempts[0] + pTurn1->attempts[1] + pTurn1->attempts[2];
	else
	{
		if (pTurn1->isStrike)
		{  // score is 10 + two next attempts
			sum += 10;
			if (pTurn2->isLastTurn || !pTurn2->isStrike)
				sum += pTurn2->attempts[0] + pTurn2->attempts[1];
			else
				sum += 10 + pTurn3->attempts[0];
		}
		else if (pTurn1->isSpare)  // score is 10 + next turn's first attempt
			sum = 10 + pTurn2->attempts[0];
		else
			sum = pTurn1->attempts[0] + pTurn1->attempts[1];
	}
	pTurn1->finalTurnScore = sum;
	return sum;
}
int	getRandomScore(int lastAttempt)
{
	return getRandomInt(0, 10 - lastAttempt); // 0-10
}

void printTurnAttempts(const Turn* pTurn)
{
	if (pTurn->isLastTurn)
	{  // special frame - frame 10
		if (pTurn->isStrike)  // strike
		{
			printf("X ");
			if (pTurn->attempts[1] == MAX_SCORE)
			{  // second strike
				printf("X ");
				if (pTurn->attempts[2] == MAX_SCORE)  // third strike
					printf("X|");
				else if (!pTurn->attempts[2])
					printf("-|");
				else
					printf("%d|", pTurn->attempts[2]);
			}
			else
			{
				if (!pTurn->attempts[1])
					printf("- ");
				else
					printf("%d ", pTurn->attempts[2]);
				if (pTurn->attempts[2] == MAX_SCORE)  // spare
					printf("/|");
				else if (!pTurn->attempts[2])
					printf("-|");
				else
					printf("%d|", pTurn->attempts[2]);
			}
		}
		else if (pTurn->isSpare)
		{  // spare
			if (!pTurn->attempts[0])
				printf("- / ");
			else
				printf("%d / ", pTurn->attempts[0]);
			if (pTurn->attempts[2] == MAX_SCORE)  // third strike
				printf("X|");
			else if (!pTurn->attempts[2])
				printf("-|");
			else
				printf("%d|", pTurn->attempts[2]);
		}
		else  // just two normal attempts
		{
			if (!pTurn->attempts[0])
				printf("- ");
			else
				printf("%d ", pTurn->attempts[0]);
			if (!pTurn->attempts[1])
				printf("-  |");
			else
				printf("%d  |", pTurn->attempts[1]);
		}
	}
	else
	{  // frames 1-9
		if (pTurn->isStrike)
			printf("  X|");
		else if (pTurn->isSpare)
		{
			if (!pTurn->attempts[0])
				printf("- ");
			else
				printf("%d ", pTurn->attempts[0]);
			printf("/|");
		}
		else
		{
			if (!pTurn->attempts[0])
				printf("- ");
			else
				printf("%d ", pTurn->attempts[0]);
			if (!pTurn->attempts[1])
				printf("-|");
			else
				printf("%d|", pTurn->attempts[1]);
		}
	}
}
void freeTurn(Turn* pTurn)
{
	IF_NOT_NULL_FREE(pTurn)
}

