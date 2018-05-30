public class Question extends Post{
	/** Título */
	private String *title;
	/** Número de respostas */
	private int nanswers;
	/** Tags */
	private HashMap<Integer, String> tags;
	/** Última atividade do post */
	private LocalDate lastActivityDate;
	
	/**
	* Construtor parametrizado
	*/
	public Question(String title, int nanswers, HashMap tags, LocalDate lastActivityDate, char postTypeId, int id, int ownerUserId, Date creationDate){
		super(postTypeId, id, ownerUserId, creationDate);
		this.title = title;
		this.nanswers = nanswers;
		this.tags = tags;
		this.lastActivityDate = lastActivityDate;
	}

	/**
	* Construtor por cópia
	*/
	public Question(Question q){
		super(q);
		this.title = q.getTitle();
		this.nanswers = q.getNanswers();
		this.tags = q.getTags();
		this.lastActivityDate = q.getLastActivityDate();
	}

	/**
	* Devolve o titulo de uma pergunta
	*/
	public String getTitle(){
		return this.title;
	}

	/**
	* Devolve o número de respostas
	*/
	public int getNanswers(){
		return this.nanswers;
	}

	/**
	* Devolve as tags de uma pergunta
	*/
	public HashMap getTags(){
		return this.tags;
	}

	/**
	* Devolve a data da útlima atividade de uma pergunta
	*/
	public LocalDate getLastActivityDate(){
		return this.lastActivityDate;
	}

	/**
	* Define o titulo de uma pergunta
	*/
	public void setTitle(String title){
		this.title = title;
	}

	/**
	* Define o número de respostas de uma pergunta
	*/
	public void setNanswers(int nanswers){
		this.nanswers = nanswers;
	}

	/**
	* Define as tasgs de uma pergunta
	*/
	public void setTags(HashMap tags){
		this.tags = tags;
	}

	/**
	* Define a data da última atividade de uma pergunta
	*/
	public void setLastActivityDate(LocalDate lastActivityDate){
		this.lastActivityDate = lastActivityDate;
	}

	/**
	* Método que imprime a representação textual do objeto
	*/
	public String toString(){
		return (super().toString() + 
				"Título: " + this.title + "\n" +
				"Número de respostas: " + this.nanswers + "\n" +
				"Tags: " + this.tags + "\n" +
				"Data da última atividade: " + this.lastActivityDate + "\n");
	}

	/**
	* Verifica se dois posts são iguais
	*/
	public boolean equals(Object o){
		if(o == this) return true;
		if(o == null || o.getClass() != this.getClass()) return false;
		Question q = (Question) o;
		return (super.equals(q) &&
				this.title.equals(p.getTitle()) &&
				this.nanswers == p.getNanswers() &&
				this.tags.equals(p.getTags()) &&
				this.lastActivityDate.equals(p.getLastActivityDate()));
	}

	/**
	* Devolve uma cópia da classe
	*/
	public Post clone(){
		return new Question(this);
	}

	/**
	* Verifica se uma resposta contém uma dada tag
	*/
	int contains_tag(int tagId){
		this.containsValue(tagId);
	}
}