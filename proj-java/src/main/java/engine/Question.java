package engine;

import java.util.Map;
import java.util.HashMap;
import java.time.LocalDate;
import java.util.List;

/**
 * Classe que guarda informação sobre uma Question
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro Barbosa (A82068) // Grupo 26
 * @version 30/05/2018
 */
public class Question extends Post{
	/** Título */
	private String title;
	/** Número de respostas */
	private int nanswers;
	/** Tags */
	private List<Integer> tags;
	/** Última atividade do post */
	private LocalDate lastActivityDate;

	/**
	 * Método que permite inicializar a classe dos Posts
	 * (Construtor parametrizado)
	 * @param title Título da Pergunta
     * @param nanswers Número de respostas
     * @param tags Lista das tags
     * @param lastActivityDate Data da última atividade
     * @param postTypeId  Tipo do Post
   	 * @param id  ID do Post
   	 * @param ownerUserId Id do user que fez a pergunta
   	 * @param creationDate Data da criação do Post
	 */
	public Question(String title, int nanswers, List<Integer> tags, LocalDate lastActivityDate,
									char postTypeId, long id, long ownerUserId, LocalDate creationDate){
		super(postTypeId, id, ownerUserId, creationDate);
		this.title = title;
		this.nanswers = nanswers;
		this.tags = tags;
		this.lastActivityDate = lastActivityDate;
	}

	/**
	 * Método que permite inicializar a classe Question recebendo como 
     * parâmetro uma classe Question 
	 * (Construtor por cópia)
	 * @param q Question
	 */
	public Question(Question q){
		super(q);
		this.title = q.getTitle();
		this.nanswers = q.getNanswers();
		this.tags = q.getTags();
		this.lastActivityDate = q.getLastActivityDate();
	}

	/**
	* Método que devolve o titulo de uma pergunta
	* @return String Título da Pergunta
	*/
	public String getTitle(){
		return this.title;
	}

	/**
	* Método que devolve o número de respostas
	* @return  nanswers Número de respostas
	*/
	public int getNanswers(){
		return this.nanswers;
	}

	/**
	* Método que evolve as tags de uma pergunta
	* @return tags Lista das tags
	*/
	public List<Integer> getTags(){
		return this.tags;
	}

	/**
	* Método que devolve a data da útlima atividade de uma pergunta.
	* @return lastActivityDate Data da última atividade
	*/
	public LocalDate getLastActivityDate(){
		return this.lastActivityDate;
	}

	/**
	* Método que define o titulo de uma pergunta
	* @param String Título da Pergunta
	*/
	public void setTitle(String title){
		this.title = title;
	}

	/**
	* Método que define o número de respostas de uma pergunta
	* @param nanswers Número de respostas
	*/
	public void setNanswers(int nanswers){
		this.nanswers = nanswers;
	}

	/**
	* Método que define as tasgs de uma pergunta
	* @param tags Lista das tags
	*/
	public void setTags(List<Integer> tags){
		this.tags = tags;
	}

	/**
	* Método que define a data da última atividade de uma pergunta
	* @param lastActivityDate Data da última atividade
	*/
	public void setLastActivityDate(LocalDate lastActivityDate){
		this.lastActivityDate = lastActivityDate;
	}

	/**
	* Método que imprime a representação textual do objeto
	* @return String
	*/
	public String toString(){
		return (super.toString() +
				"Título: " + this.title + "\n" +
				"Número de respostas: " + this.nanswers + "\n" +
				"Tags: " + this.tags + "\n" +
				"Data da última atividade: " + this.lastActivityDate + "\n");
	}

	/**
	* Método que verifica se dois posts são iguais
	* @return true se forem iguais, false caso contrário
	*/
	public boolean equals(Object o){
		if(o == this) return true;
		if(o == null || o.getClass() != this.getClass()) return false;
		Question q = (Question) o;
		return (super.equals(q) &&
				this.title.equals(q.getTitle()) &&
				this.nanswers == q.getNanswers() &&
				this.tags.equals(q.getTags()) &&
				this.lastActivityDate.equals(q.getLastActivityDate()));
	}

	/**
	* Método que devolve uma cópia da classe
	* @return Question
	*/
	public Question clone(){
		return new Question(this);
	}

	/**
	* Método que verifica se uma resposta contém uma dada tag
	* @param tagId ID da tag
	* @return true se existir essa tag, false caso contrário
	*/
	public boolean contains_tag(int tagId){
		return this.tags.contains(tagId);
	}
}
