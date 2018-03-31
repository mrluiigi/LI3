#include <date.h>

int isAfterBegin (Date begin, Date current){
	int anoBegin = get_year(begin);
	int anoCurrent = get_year(current);
	int mesBegin = get_month(begin);
	int mesCurrent = get_month(current);
	int diaBegin = get_day(begin);
	int diaCurrent = get_day(current);

	if(anoCurrent >= anoBegin && mesCurrent >= mesBegin && diaCurrent >= diaBegin) return 1;
	else return -1;
}


int isBeforeEnd (Date end, Date current){
	int anoEnd = get_year(end);
	int anoCurrent = get_year(current);
	int mesEnd = get_month(end);
	int mesCurrent = get_month(current);
	int diaEnd = get_day(end);
	int diaCurrent = get_day(current);

	if(anoCurrent <= anoEnd && mesCurrent <= mesEnd && diaCurrent <= diaEnd) return 1;
	else return -1;
}