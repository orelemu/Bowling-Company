#pragma once

#define ATTEMPTS 3
#define MAX_SCORE 10

typedef struct {
	int	attempts[ATTEMPTS];
	int finalTurnScore;
	int	isStrike;
	int	isSpare;
	int	isLastTurn;
} Turn;

void	initTurn(Turn* pTurn, int turnNum);

void	takeSingleTurn(Turn* pTurn);
void	takeLastTurn(Turn* pTurn, int lastAttempt);
void	takeNormalTurn(Turn* pTurn, int lastAttempt);

int		calculateFinalTurnScore(Turn* pTurn1, Turn* pTurn2, Turn* pTurn3);
int		getRandomScore(int lastAttempt);

void	printTurnAttempts(const Turn* pTurn);
void	freeTurn(Turn* pTurn);