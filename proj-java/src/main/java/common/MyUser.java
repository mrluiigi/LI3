

public class MyUser{
  /** ID do user */
  private int id;
  /** Nome do utilizador */
  private String name;
  /** ShortBio do utilizador */
  private String shortBio;
  /** Número de posts do utilizador */
  private int nr_posts;
  /** Último post do utilizador */
  private int lastPost;
  /** Reputação do utilizador */
  private int reputation;



  /**
   *  Construtor parametrizado
   */
  public MyUser(int id, String name, String shortBio, int nr_posts, int lastPost, int reputation){
    this.id = id;
    this.name = name;
    this.shortBio = shortBio;
    this.nr_posts = nr_posts;
    this.lastPost = lastPost;
    this.reputation = reputation;
  }
  /**
    * Construtor por cópia
    */
  public MyUser(MyUser u){
    this.id = u.getId();
    this.name = u.getName();
    this.shortBio = u.getShortBio();
    this.nr_posts = u.getNr_posts();
    this.lastPost = u.getLastPost();
    this.reputation = u.getReputation();
  }

  /**
   * Devolve o ID do utilizador
   */
  public int getId() {
  	return this.id;
  }

  /**
   * Devolve o nome do utilizador
   */
  public String getName() {
  	return this.name;
  }

  /**
   * Devolve a shortBio do utilizador
   */
  public String getShortBio() {
  	return this.shortBio;
  }

  //VERIFICAR FUNÇAO
  /**
   * Define o lastPost de um utilizador
   */
  public void setLastPost(int postId){
    if(this.lastPost == 0) this.lastPost = postId;
  }

  /**
   * Devolve o número de posts de um utilizador
   */
  public int getNr_posts() {
  	return this.nr_posts;
  }

  /**
   * Devolve o lastPost de um utilizador
   */
  public int getLastPost() {
  	return this.lastPost;
  }

  /**
   * Devolve a reputação de um utilizador
   */
  public int getReputation() {
  	return this.reputation;
  }

  /**
   * Incrementa o número de posts de um utilizador
   */
  public void increment_nr_posts() {
  	this.nr_posts += 1;
  }

  /**
    * Verifica se dois utilizadores são iguais
    */
  public boolean equals(Object o){
    if(o == this) return true;
    if(o == null || o.getClass() != this.getClass()) return false;

    MyUser u = (MyUser) o;
    return this.id == u.getId() &&
           this.name.equals(u.getName()) &&
           this.shortBio.equals(u.getShortBio()) &&
           this.nr_posts == u.getNr_posts() &&
           this.lastPost == u.getLastPost() &&
           this.reputation == u.getReputation();
  }

  /**
   * Devolve uma string representativa da classe
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
    * Devolve uma cópia da classe
    */
  public MyUser clone(){
    return new MyUser(this);
  }
}
