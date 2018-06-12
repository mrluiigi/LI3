package engine;

import java.time.LocalDate;

/**
 * Classe que guarda informação sobre um Post
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro Barbosa (A82068) // Grupo 26
 * @version 31/05/2018
 */
public class Post implements Comparable<Post>{
	/** Identifica o tipo de post (Pergunta ou Resposta) */
	private char postTypeId;
	/** ID do post */
	private long id;
	/** ID do utilizador que fez o post */
	private long ownerUserId;
	/** Data da criação do post */
	private LocalDate creationDate;


	/**
	*  Método que permite inicializar a classe do Post
	* (Construtor parametrizado)
	* @param postTypeId  Tipo do Post
    * @param id  ID do Post
    * @param ownerUserId Id do user que fez a pergunta
    * @param creationDate Data da criação do Post
	*/
	public Post(char postTypeId, long id, long ownerUserId, LocalDate creationDate){
		this.postTypeId = postTypeId;
		this.id = id;
		this.ownerUserId = ownerUserId;
		this.creationDate = creationDate;
	}

	/**
	* Método que permite inicializar a classe Post recebendo como 
    * parâmetro uma classe Post 
	* (Construtor por cópia)
	* @param p Post
	*/
	public Post(Post p){
		this.postTypeId = p.getPostTypeId();
		this.id = p.getId();
		this.ownerUserId = p.getOwnerUserId();
		this.creationDate = p.getCreationDate();
	}

	/**
	* Método que devolve o tipo de Post
	* @return char Tipo do Post
	*/
	public char getPostTypeId(){
		return this.postTypeId;
	}

	/**
	* Método que devolve o id do Post
	* @return int ID do Post
	*/
	public long getId(){
		return this.id;
	}

	/**
	* Método que devolve o id do Owner do Post
	* @return long Id do user que fez a pergunta
	*/
	public long getOwnerUserId(){
		return this.ownerUserId;
	}

	/**
	* Método que devolve a data de publicação do Post
	* @return LocalDate Data da criação do Post
	*/
	public LocalDate getCreationDate(){
		return this.creationDate;
	}

	/**
	* Método que define o tipo do Post
	* @param char Tipo do Post
	*/
	public void setPostTypeId(char postTypeId){
		this.postTypeId = postTypeId;
	}

	/**
	* Método que define o id do Post
	* @param int ID do Post
	*/
	public void setId(long id){
		this.id = id;
	}

	/**
	* Método que define o id do Owner do Post
	* @param long Id do user que fez a pergunta
	*/
	public void setOwnerUserId(long ownerUserId){
		this.ownerUserId = ownerUserId;
	}

	/**
	* Método que define a data de publicação do Post
	* @param LocalDate Data da criação do Post
	*/
	public void setCreationDate(LocalDate creationDate){
		this.creationDate = creationDate;
	}

	/**
	* Método que imprime a representação textual do objeto
	* @return String
	*/
	public String toString(){
		return "Tipo do post: " + this.postTypeId + "\n" +
				"ID: " + this.id + "\n" +
				"ID do utilizador principal: " + this.ownerUserId + "\n" +
				"Data de criação: " + this.creationDate + "\n";
	}

	/**
	* Método que verifica se dois posts são iguais
	* @return true se forem iguais, false caso contrário
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
	* Método que devolve uma cópia da classe
	* @return Post
	*/
	public Post clone(){
		return new Post(this);
	}

	/**
	* Método que verifica se o post tem um owner definido
	* @return true se o post tiver um Owner, false caso contrário
	*/
	public boolean hasOwner(){
		return (this.ownerUserId != -2);
	}

	/**
	* Método que verifica se o id dado corresponde ao owner do post
	* @param id Id do user que fez a pergunta
	* @return true se id passado for igual ao do OwnerUserId do post, false caso contrário
	*/
	public boolean isOwner(long id){
		return (this.ownerUserId == id);
	}

	/**
	 * Método que implementa a ordem natural de Posts
	 * @param p Post
	 */
	public int compareTo(Post p) {
		if (this.creationDate.isAfter(p.creationDate)) return -1;
		if (this.creationDate.isBefore(p.creationDate)) return 1;
		if (this.id > p.id) return -1;
		if (this.id < p.id) return 1;
		return 0;
	}
}
