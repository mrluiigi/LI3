public class Question extends Post{
	/** Título */
	private String *title;
	/** Número de respostas */
	private int nanswers;
	/** Tags */
	private HashMap tags;
	/** Última atividade do post */
	private LocalDate lastActivityDate;

	public Question(String title, int nanswers, HashMap tags, LocalDate lastActivityDate, char postTypeId, int id, int ownerUserId, Date creationDate){
		this.title = title;
		this.nanswers = nanswers;
		this.tags = tags;
		this.lastActivityDate = lastActivityDate;
	}

	POST create_question(char * title, int nanswers, GSList * tags, Date lastActivityDate, char postTypeId, int id, char * ownerUserId, Date creationDate){
	POST p = malloc(sizeof(struct post));

	p->q->title = mystrdup(title);
	p->q->nanswers = nanswers;
	p->q->tags = g_slist_copy(tags);
	p->q->lastActivityDate = createDate(get_day(lastActivityDate),get_month(lastActivityDate),get_year(lastActivityDate));
	p->postTypeId = postTypeId;
	p->id = id;
	p->ownerUserId = mystrdup(ownerUserId);
	p->creationDate = createDate(get_day(creationDate),get_month(creationDate),get_year(creationDate));
	return p;
}
}