#include <glib.h>
#include "date.h"
#include <stdlib.h>


#ifndef HEADER_FILE
#define HEADER_FILE

/**
* Retorna 1 se a primeira data for a mais antiga, -1 se for a mais recente e 0 se forem iguais
*/
int compare_date (Date d1, Date d2);

/**
* Função que converte a CreationDate do XML na struct date
*/
Date xmlCreationDate_to_Date(char* xmlDate);
#endif