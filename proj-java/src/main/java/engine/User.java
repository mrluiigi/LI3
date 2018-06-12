package engine;

/**
 * Classe que guarda informação sobre um utilizador
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro Barbosa (A82068) // Grupo 26
 * @version 30/05/2018
 */

public class User{
  /** ID do user */
  private long id;
  /** Nome do utilizador */
  private String name;
  /** ShortBio do utilizador */
  private String shortBio;
  /** Número de posts do utilizador */
  private int nr_posts;
  /** Último post do utilizador */
  private long lastPost;
  /** Reputação do utilizador */
  private long reputation;


  /**
   * Método que permite inicializar a classe do User
   * (Construtor parametrizado)
   * @param id ID do utilizador
   * @param name Nome do utilizador
   * @param shortBio String da shortBio do utilizador
   * @param nr_posts Número de Posts do utilizador
   * @param lastPost LastPost do utilizador
   * @param reputation Reputação do utilizador
   */
  public User(long id, String name, String shortBio, int nr_posts, long lastPost, long reputation){
    this.id = id;
    this.name = name;
    this.shortBio = shortBio;
    this.nr_posts = nr_posts;
    this.lastPost = lastPost;
    this.reputation = reputation;
  }
  /**
    * Método que permite inicializar a classe User recebendo como 
    * parâmetro uma classe User 
    * (Construtor por cópia)
    * @param u User
    */
  public User(User u){
    this.id = u.getId();
    this.name = u.getName();
    this.shortBio = u.getShortBio();
    this.nr_posts = u.getNr_posts();
    this.lastPost = u.getLastPost();
    this.reputation = u.getReputation();
  }

  /**
   * Método que devolve o ID do utilizador
   * @return long ID do utilizador
   */
  public long getId() {
  	return this.id;
  }

  /**
   * Método que devolve o nome do utilizador
   * @return String Nome do utilizador
   */
  public String getName() {
  	return this.name;
  }

  /**
   * Método que devolve a shortBio do utilizador
   * @return String ShortBio do utilizador
   */
  public String getShortBio() {
  	return this.shortBio;
  }

  /**
   * Método que define o lastPost de um utilizador
   * @param postID Último PostID
   */
  public void setLastPost(long postId){
    if(this.lastPost == 0) this.lastPost = postId;
  }

  /**
   * Método que devolve o número de posts de um utilizador
   *@return int Número de posts do utilizador
   */
  public int getNr_posts() {
  	return this.nr_posts;
  }

  /**
   * Método que devolve o lastPost de um utilizador
   * @return long Último post do utilizador
   */
  public long getLastPost() {
  	return this.lastPost;
  }

  /**
   * Método que devolve a reputação de um utilizador
   * @return int Reputação
   */
  public long getReputation() {
  	return this.reputation;
  }

  /**
   * Método que incrementa o número de posts de um utilizador
   */
  public void increment_nr_posts() {
  	this.nr_posts += 1;
  }

  /**
    * Método que verifica se dois utilizadores são iguais
    * @param o Object
    * @return true se forem iguais, false caso contrário
    */
  public boolean equals(Object o){
    if(o == this) return true;
    if(o == null || o.getClass() != this.getClass()) return false;

    User u = (User) o;
    return this.id == u.getId() &&
           this.name.equals(u.getName()) &&
           this.shortBio.equals(u.getShortBio()) &&
           this.nr_posts == u.getNr_posts() &&
           this.lastPost == u.getLastPost() &&
           this.reputation == u.getReputation();
  }

  /**
   * Método que devolve uma string representativa da classe
   * @return String
   */
  public String toString(){
    return "Id: " + this.id + "\n" +
           "Nome: " + this.name + "\n" +
           "ShortBio" + this.shortBio + "\n" +
           "Número de posts" + this.nr_posts + "\n" +
           "Último post do utilizador" + this.lastPost + "\n" +
           "Reputação" + this.reputation;
  }

  /**
    * Método que devolve uma cópia da classe
    * @return User
    */
  public User clone(){
    return new User(this);
  }
}
