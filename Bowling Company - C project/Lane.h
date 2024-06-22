#pragma once

#include "Game.h"

typedef struct {
	char	laneCode[2]; // 1c + 1 for '\0'
	Game** games;
	int		numOfGames;
	int		bestScore;
} Lane;

int		initLane(Lane* pLane, Lane** pLaneArr, int numOfElements);
int		getLaneCode(Lane* pLane, Lane** pLaneArr, int numOfElements);
int		isLaneCodeUniqueAndValid(Lane* pLane, Lane** pLaneArr, int numOfElements);

int		addGame(Lane* pLane);
void	clearGameList(Lane* pLane);

int		compareLanesByCode(char c1, char c2);
int		compareLanesByBestScore(int score1, int score2);

void	printLane(const Lane* pLane);
void	printLaneV(void* val);

void	freeLane(Lane* pLane);
void	freeLaneV(void* ppLane);
