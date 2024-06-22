#pragma once

#include "Player.h"
#include "Date.h"
#include "GeneralList.h"

#define MAX_PLAYERS 6

typedef struct {
	LIST	playersList;
	int		numOfPlayers;
	Date	date;
	Player* winner;
} Game;

int		initGame(Game* pGame);
int		getPlayers(Game* pGame);
Player* getWinner(Game* pGame);

int		compareGamesByDate(Date date1, Date date2);

void	printGame(const Game* pGame);
void	printGameSummary(const Game* pGame);
void	printGameScoreboard(const Game* pGame);

void	freeGame(Game* pGame);
void	freeGameV(void* ppGame);
