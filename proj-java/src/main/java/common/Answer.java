public class Answer extends Post{
	/** ID do post a que pertence a pergunta */
	private int parentId
	/** Número de comentários de uma respostas */
	private int comments
	/** Score de uma resposta */
	private int score

	/**
	* Construtor parametrizado
	*/
	public Answer(Sint parentId, int comments, int score, char postTypeId, int id, int ownerUserId, Date creationDate){
		super(postTypeId, id, ownerUserId, creationDate);
		this.parentId = parentId;
		this.comments = comments;
		this.score = score;
	}

	/**
	* Construtor por cópia
	*/
	public Answer(Answer a){
		super(a);
		this.parentId = a.getParentId();
		this.comments = a.getComments();
		this.score = a.getScore();
	}

	/**
	* Devolve o ID do post a que a resposta pertence
	*/
	public int getParentId(){
		return this.parentId;
	}

	/**
	* Devolve o número de comentários
	*/
	public int getComments(){
		return this.comments;
	}

	/**
	* Devolve a score de uma resposta
	*/
	public int getScore(){
		return this.score;
	}

	/**
	* Define o ID do post a que a resposta pertence
	*/
	public void setParentId(int parentId){
		this.parentId = parentId;
	}

	/**
	* Define o número de comentários de uma resposta
	*/
	public void setComments(int comments){
		this.comments = comments;
	}

	/**
	* Define a score de uma resposta
	*/
	public void setScore(int score){
		this.score = score;
	}

	/**
	* Método que imprime a representação textual do objeto
	*/
	public String toString(){
		return (super().toString() + 
				"Parent ID: " + this.parentId + "\n" +
				"Número de comentários: " + this.comments + "\n" +
				"Score: " + this.score + "\n");
	}

	/**
	* Verifica se duass respostas são iguais
	*/
	public boolean equals(Object o){
		if(o == this) return true;
		if(o == null || o.getClass() != this.getClass()) return false;
		Answer a = (Answer) o;
		return (super.equals(a) &&
				this.parentId == a.getParentId() &&
				this.comments == a.getComments() &&
				this.score == a.getScore());
	}

	/**
	* Devolve uma cópia da classe
	*/
	public Post clone(){
		return new Answer(this);
	}

}