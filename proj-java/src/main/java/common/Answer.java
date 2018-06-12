package common;

import java.time.LocalDate;

/**
 * Classe que guarda informação sobre uma Answer
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro Barbosa (A82068) // Grupo 26
 * @version 30/05/2018
 */
public class Answer extends Post{
	/** ID do post a que pertence a pergunta */
	private long parentId;
	/** Número de comentários de uma respostas */
	private int comments;
	/** Score de uma resposta */
	private int score;

	/**
	* Método que permite inicializar a classe dos Posts
	* (Construtor parametrizado)
	*/
	public Answer(long parentId, int comments, int score,
						char postTypeId, long id, long ownerUserId, LocalDate creationDate){
		super(postTypeId, id, ownerUserId, creationDate);
		this.parentId = parentId;
		this.comments = comments;
		this.score = score;
	}

	/**
	* Método que permite inicializar a classe Answer recebendo como 
    * parâmetro uma classe Answer 
	* (Construtor por cópia)
	* @param a Answer
	*/
	public Answer(Answer a){
		super(a);
		this.parentId = a.getParentId();
		this.comments = a.getComments();
		this.score = a.getScore();
	}

	/**
	* Método que devolve o ID do post a que a resposta pertence
	* @return long ID da Pergunta
	*/
	public long getParentId(){
		return this.parentId;
	}

	/**
	* Método que devolve o número de comentários
	* @return int Número de comentários
	*/
	public int getComments(){
		return this.comments;
	}

	/**
	* Método que devolve a score de uma resposta
	* @return int Score
	*/
	public int getScore(){
		return this.score;
	}

	/**
	* Método que define o ID do post a que a resposta pertence
	* @param long ID da pergunta
	*/
	public void setParentId(long parentId){
		this.parentId = parentId;
	}

	/**
	* Método que define o número de comentários de uma resposta
	* @param int Número de comentários
	*/
	public void setComments(int comments){
		this.comments = comments;
	}

	/**
	* Método que define a score de uma resposta
	* @param int Score
	*/
	public void setScore(int score){
		this.score = score;
	}

	/**
	* Método que imprime a representação textual do objeto
	* @return String
	*/
	public String toString(){
		return (super.toString() +
				"Parent ID: " + this.parentId + "\n" +
				"Número de comentários: " + this.comments + "\n" +
				"Score: " + this.score + "\n");
	}

	/**
	* Método que verifica se duas respostas são iguais
	* @return true se forem iguais, false caso contrário
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
	* Método que devolve uma cópia da classe
	* @return Answer
	*/
	public Answer clone(){
		return new Answer(this);
	}

}
