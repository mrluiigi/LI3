#include "mydate.h"

/**
* Retorna 1 se a primeira data for a mais antiga, -1 se for a mais recente e 0 se forem iguais
*/
int compare_date (Date d1, Date d2){
	int ano1 = get_year(d1);
	int ano2 = get_year(d2);
	int mes1= get_month(d1);
	int mes2 = get_month(d2);
	int dia1 = get_day(d1);
	int dia2 = get_day(d2);

	if(ano1 > ano2) return -1;
	if(ano1 < ano2) return 1;

	if(mes1 > mes2) return -1;
	if(mes1 < mes2) return 1;

	if(dia1 > dia2) return -1;
	if(dia1 < dia2) return 1;

	return 0;
}

/**
* Função que converte a CreationDate do XML na struct date
*/
Date xmlCreationDate_to_Date(char* xmlDate) {
	char s_year[2];
	s_year[0] = xmlDate[2];
	s_year[1] = xmlDate[3];
	int year = 2000 + atoi(s_year);

	char s_month[2];
	s_month[0] = xmlDate[5];
	s_month[1] = xmlDate[6];
	int month = atoi(s_month);

	char s_day[2];
	s_day[0] = xmlDate[8];
	s_day[1] = xmlDate[9];
	int day = atoi(s_day);

	Date d = createDate(day, month, year);
	return d;
}
