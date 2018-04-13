#include <glib.h>
#include "date.h"
#include <stdlib.h>


#ifndef HEADER_FILE
#define HEADER_FILE

int compare_date (Date d1, Date d2);

Date xmlCreationDate_to_Date(char* xmlDate);

int date_to_Key(int year, int month);



#endif