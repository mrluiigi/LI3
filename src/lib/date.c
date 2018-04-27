#include <stdlib.h>
#include "date.h"

/**
 * Estrutura que guarda informação sobre uma data
 */
struct date {
  /** Dia da data */
  int day;
  /** Mês da data */
  int month;
  /** Ano da data */
  int year;
};

/**
* Função que inicializa a estrutura de dados das datas
*/
Date createDate(int day, int month, int year) {
    Date d = malloc(sizeof(struct date));
    d->day = day;
    d->month = month;
    d->year = year;
    return d;
}

/** 
 * Devolve o dia da data
 */
int get_day(Date d) {
    return d->day; 
}

/** 
 * Devolve o mês da data
 */
int get_month(Date d) {
    return d->month; //change to enum? (Acho que sim. Desta forma já garantimos a limitação necessária)
}

/** 
 * Devolve o ano da data
 */
int get_year(Date d) {
    return d->year;
}

/** 
 * Liberta a memória alocada para a struct date
 */
void free_date(Date d) {
    free(d);
}

