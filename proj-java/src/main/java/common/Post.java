package common;

import java.time.LocalDate;

public class Post{
	/** Identifica o tipo de post (Pergunta ou Resposta) */
	private char postTypeId;
	/** ID do post */
	private long id;
	/** ID do utilizador que fez o post */
	private long ownerUserId;
	/** Data da criação do post */
	private LocalDate creationDate;


	/**
	* Construtor parametrizado
	*/
	public Post(char postTypeId, long id, long ownerUserId, LocalDate creationDate){
		this.postTypeId = postTypeId;
		this.id = id;
		this.ownerUserId = ownerUserId;
		this.creationDate = creationDate;
	}

	/**
	* Construtor por cópia
	*/
	public Post(Post p){
		this.postTypeId = p.getPostTypeId();
		this.id = p.getId();
		this.ownerUserId = p.getOwnerUserId();
		this.creationDate = p.getCreationDate();
	}

	/**
	* Devolve o tipo de Post
	*/
	public char getPostTypeId(){
		return this.postTypeId;
	}

	/**
	* Devolve o id do Post
	*/
	public long getId(){
		return this.id;
	}

	/**
	* Devolve o id do Owner do Post
	*/
	public long getOwnerUserId(){
		return this.ownerUserId;
	}

	/**
	* Devolve a data de publicação do Post
	*/
	public LocalDate getCreationDate(){
		return this.creationDate;
	}

	/**
	* Define o tipo do Post
	*/
	public void setPostTypeId(char postTypeId){
		this.postTypeId = postTypeId;
	}

	/**
	* Define o id do Post
	*/
	public void setId(long id){
		this.id = id;
	}

	/**
	* Define o id do Owner do Post
	*/
	public void setOwnerUserId(long ownerUserId){
		this.ownerUserId = ownerUserId;
	}

	/**
	* Define a data de publicação do Post
	*/
	public void setCreationDate(LocalDate creationDate){
		this.creationDate = creationDate;
	}

	/**
	* Método que imprime a representação textual do objeto
	*/
	public String toString(){
		return "Tipo do post: " + this.postTypeId + "\n" +
				"ID: " + this.id + "\n" +
				"ID do utilizador principal: " + this.ownerUserId + "\n" +
				"Data de criação: " + this.creationDate + "\n";
	}

	/**
	* Verifica se dois posts são iguais
	*/
	public boolean equals(Object o){
		if(o == this) return true;
		if(o == null || o.getClass() != this.getClass()) return false;
		Post p = (Post) o;
		return(this.postTypeId == p.getPostTypeId() &&
				this.id == p.getId() &&
				this.ownerUserId == p.getOwnerUserId() &&
				this.creationDate.equals(p.getCreationDate()));
	}

	/**
	* Devolve uma cópia da classe
	*/
	public Post clone(){
		return new Post(this);
	}

	/**
	* Verifica se o post tem um owner definido
	*/
	public boolean hasOwner(){
		return (this.ownerUserId != -2);
	}

	/**
	* Verifica se o id dado corresponde ao owner do post
	*/
	public boolean isOwner(long id){
		return (this.ownerUserId == id);
	}
}
