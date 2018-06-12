package common;

import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.TreeSet;
import java.util.List;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Iterator;
import java.util.stream.Collectors;

/**
 * Classe que guarda informação sobre os Users
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro Barbosa (A82068) // Grupo 26
 * @version 31/05/2018
 */
public class Users{
  /** Map que guarda os Users. A chave é o ID do user e o valor é um MyUser com a informação relativa a esse user */
  private Map<Long, MyUser> users;
  /** Set que guarda os users por ordem decrescente de número de posts */
  private Set<MyUser> users_by_nr_posts;
  /** Set que guarda os users por ordem decrescente de reputação */
  private Set<MyUser> users_by_reputation;

  /**
   * Método que permite inicializar a classe dos Users
   * (Construtor por omissão)
   */
  public Users(){
    this.users = new HashMap<>();
    this.users_by_nr_posts = new TreeSet<>();
    this.users_by_reputation = new TreeSet<>();
  }

  /**
   * Método que permite inicializar a classe dos Users recebendo como 
   * parâmetro uma classe Users
   * (Construtor por cópia)
   * @param u Users
   */
  public Users(Users u){
    this.setUsers(u.getUsers());
  }

  /**
   * Método que define os users dado um Map
   * @param us Map<Long, MyUser>
   */
  public void setUsers(Map<Long, MyUser> us){
    this.users = new HashMap<>();
    us.forEach((i,u) -> this.users.put(i, u.clone()));
  }

  /**
   *  Método que devolve um Map com todos os users
   * @return Map<Long, MyUser>
   */
  public Map<Long, MyUser> getUsers(){
    Map<Long, MyUser> res = new HashMap<>();
    this.users.forEach((i,u) -> res.put(i, u.clone()));
    return res;
  }

  /**
   * Método que devolve uma string representativa da classe
   * @return String 
   */
  public String toString(){
    return this.users.values().stream().map(u -> u.toString()).collect(Collectors.joining("\n"));
  }

  /**
   * Método que devolve uma cópia da classe
   * @return Users
   */
  public Users clone(){
    return new Users(this);
  }

  /**
   * Método que ordena os users dado um comparador
   * @param c Comparator<MyUser>
   * @return Set<MyUser> Utilizadores ordenados de acordo com o comparador passado
   */
  public Set<MyUser> sort_users(Comparator<MyUser> c) {
    Set<MyUser> res = new TreeSet(c);
    //Para todas as chaves do Map
    for(long i : this.users.keySet()){
      //Adiciona o valor dessa chave ao Set
      res.add(this.users.get(i));
    }
    return res;
  }

  /**
   * Método que encontra um determinado user dado o seu ID
   * @param id ID do user
   * @return MyUser User respetivo ao ID dado como parâmetro
   */
  public MyUser find_user(long id){
    return this.users.get(id);
  }

  /**
   * Método que insere um user no Map dos Users
   * @param id ID do user
   * @param name Nome do user
   * @param shortBio String da shortBio do user
   * @param nr_posts Número de Posts do user
   * @param lastPost LastPost do user
   * @param reputation Reputação do user
   */
  public void add_myuser(long id, String name, String shortBio, int nr_posts, long lastPost, int reputation){
    //Cria um MyUser
    MyUser u = new MyUser(id, name, shortBio, nr_posts, lastPost, reputation);
    this.users.put(id, u);
  }

  /**
   * Método que remove um user dado o ID
   * @param id ID do user
   */
  public void remove_myuser(long id){
    this.users.remove(id);
  }

  /**
   * Método que devolve uma lista com os N users que tem maior reputação
   * @param N Número de utilizadores que deseja
   * @return List<Long> Lista com os N users
   */
  public List<Long> get_N_users_with_most_reputation(int N) {
    //Se o Set estiver vazio
    if(this.users_by_reputation.isEmpty()) {
      //Coloca no Set os MyUser ordenados decrescentemente por reputação
      this.users_by_reputation = this.sort_users((MyUser u1, MyUser u2) -> ((u2.getReputation() - u1.getReputation() != 0) ?
        (u2.getReputation() - u1.getReputation()) : (u1.equals(u2) == true ? 0 : 1)));
    }
    List<Long> res = new ArrayList<>();
    int i = 0;
    Iterator<MyUser> iterador = this.users_by_reputation.iterator();

    //Enquanto não se atinge os N e ainda há MyUser 
    while(i < N && iterador.hasNext()){
      res.add(iterador.next().getId());
      i++;
    }
    return res;
  }

  /**
   * Método que devolve uma lista com os users que tem o maior número de posts
   * @param N Número de utilizadores que deseja
   * @return List<Long> Lista com os N users
   */
  public List<Long> get_N_users_with_most_nr_posts(int N){
    //Se o Set estiver vazio
    if(this.users_by_nr_posts.isEmpty()) {
      //Coloca no Set os MyUser ordenados decrescentemente por número de posts
      this.users_by_nr_posts = this.sort_users((MyUser u1, MyUser u2) -> ((u2.getNr_posts() - u1.getNr_posts() != 0) ?
        (u2.getNr_posts() - u1.getNr_posts()) : (u1.equals(u2) == true ? 0 : 1)));
    }
    List<Long> res = new ArrayList<>();
    int i = 0;
    Iterator<MyUser> iterador = this.users_by_nr_posts.iterator();

    //Enquanto não se atinge os N e ainda há MyUser 
    while(i < N && iterador.hasNext()){
      res.add(iterador.next().getId());
      i++;
    }
    return res;
  }

  /**
   * Método que incrementa o número de posts de um determinado User
   * @param id ID do user
   */
  public void find_and_increment_user_nr_posts(long id) {
        //Procura o utilizador
        MyUser user = this.users.get(id);
        //Incrementa o número de posts desse MyUser
        user.increment_nr_posts();
  }

  /**
   * Método que define o lastPost de uma dado user
   * @param id ID do user
   * @param lastPostId Último post do user
   */
  public void find_and_set_user_lastPost(long id, long lastPostId) {
      //Procura o utilizador
      MyUser u = this.users.get(id);
      //Define o lastPost
      u.setLastPost(lastPostId);
  }
}
