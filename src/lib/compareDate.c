int isAfterBegin (Date begin, Date current){
	anoBegin = get_year(begin);
	anoCurrent = get_year(current);
	mesBegin = get_month(begin);
	mesCurrent = get_month(current);
	diaBegin = get_day(begin);
	diaCurrent = get_day(current);

	if(anoCurrent >= anoBegin && mesCurrent >= mesBegin && diaCurrent >= diaBegin) return 1;
	else return -1;
}


int isBeforeEnd (Date end, Date current){
	anoEnd = get_year(end);
	anoCurrent = get_year(current);
	mesEnd = get_month(end);
	mesCurrent = get_month(current);
	diaEnd = get_day(end);
	diaCurrent = get_day(current);

	if(anoCurrent <= anoEnd && mesCurrent <= mesEnd && diaCurrent <= diaEnd) return 1;
	else return -1;
}