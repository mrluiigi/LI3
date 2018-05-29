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
   *  Cria um user com base nos parâmetros recebidos
   */
  public MyUser(int id, char * name, char * shortBio, int nr_posts, int lastPost, int reputation){
    this.id = id;
    this.name = name;
    this.shortBio = shortBio;
    this.nr_posts = nr_posts;
    this.lastPost = lastPost;
    this.reputation = reputation;
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
}
