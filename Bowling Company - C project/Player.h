#pragma once
#include "Turn.h"

#define TURNS 10

typedef struct {
	char* playerName;
	Turn	turns[TURNS];
	int		finalScore;
} Player;

void	initPlayer(Player* pPlayer);

int		comparePlayers(void* pPlayer1, void* pPlayer2);
int		comparePlayersByScore(int score1, int score2);
int		calculateScore(Player* pPlayer);

void	printPlayer(const Player* pPlayer);
void	printPlayerScoreboard(const Player* pPlayer);

void	freePlayer(Player* pPlayer);
void	freePlayerV(void* pPlayer);