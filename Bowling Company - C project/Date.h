#ifndef __MYDATE__
#define __MYDATE__

#define MIN_YEAR 2023
#define SPECIAL_TAV '#'

typedef struct
{
	int		day;
	int		month;
	int		year;
}Date;

void	getCorrectDate(Date* pDate);
int		checkDate(char* date, Date* pDate);

int		compareDate(const void* d1, const void* d2);

void	printDate(const Date* pDate);

#endif
