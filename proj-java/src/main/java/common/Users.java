import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.TreeSet;
import java.util.List;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Iterator;
import java.util.stream.Collectors;


public class Users{
  
  private Map<Integer, MyUser> users;

  /**
   * Construtor por omissão
   */
  public Users(){
    this.users = new HashMap<>();
  }

  /**
   * Construtor por cópia
   */
  public Users(Users u){
    this.setUsers(u.getUsers());
  }

  /**
   * Define os users dado um Map
   */
  public void setUsers(Map<Integer, MyUser> us){
    this.users = new HashMap<>();
    us.forEach((i,u) -> this.users.put(i, u.clone()));
  }

  /**
   *  Devolve um Map com todos os users
   */
  public Map<Integer, MyUser> getUsers(){
    Map<Integer, MyUser> res = new HashMap<>();
    this.users.forEach((i,u) -> res.put(i, u.clone()));
    return res;
  }

  /**
   * Método que devolve uma string representativa da classe
   */
  public String toString(){
    return this.users.values().stream().map(u -> u.toString()).collect(Collectors.joining("\n"));
  }

  /**
   * Método que devolve uma cópia da classe
   */
  public Users clone(){
    return new Users(this);
  }

  /**
   * Método que ordena os users dado um comparador
   */
  public Set<MyUser> sort_users(Comparator<MyUser> c) {
    Set<MyUser> res = new TreeSet(c);
    for(int i : this.users.keySet()){
      res.add(this.users.get(i).clone());
    }
    return res;
  }

  /**
   * Encontra um determinado user dado o seu ID
   */
  public MyUser find_user(int id){
    return this.users.get(id);
  }

  /**
   * Insere um user
   */
  public void add_myuser(int id, String name, String shortBio, int nr_posts, int lastPost, int reputation){
    MyUser u = new MyUser(id, name, shortBio, nr_posts, lastPost, reputation);
    this.users.put(id, u);
  }

  /**
   * Remove um user
   */
  public void remove_myuser(int id){
    this.users.remove(id);
  }

  /**
   * Devolve uma lista com os N users que tem maior reputação
   */
  public List<MyUser> get_N_users_with_most_reputation(int N) {
    Comparator<MyUser> c = (MyUser u1, MyUser u2) -> u2.getReputation() - u1.getReputation();
    Set<MyUser> set = this.sort_users(c);
    List<MyUser> res = new ArrayList<>();
    int i = 0;
    Iterator<MyUser> iterador = set.iterator();

    while(i < N && iterador.hasNext()){
      res.add(iterador.next());
      i++;
    }
    return res;
  }

  /**
   * Devolve uma lista com os users que tem o maior número de posts
   */
  public List<MyUser> get_N_users_with_most_nr_posts(int N){
      Comparator<MyUser> c = (MyUser u1, MyUser u2) -> u2.getNr_posts() - u1.getNr_posts();
      List<MyUser> res = new ArrayList<>();
      Set<MyUser> set = this.sort_users(c);
      int i = 0;
      Iterator<MyUser> iterador = set.iterator();

      while(i < N && iterador.hasNext()){
        res.add(iterador.next());
        i++;
      }
      return res;
  }

  /**
   * Incrementa o número de posts de um determinado user
   */
  public void find_and_increment_user_nr_posts(int id) {
        MyUser user = this.users.get(id);
        user.increment_nr_posts();
  }

  /**
   * Define o lastPost de uma dado user
   */
  public void find_and_set_user_lastPost(int id, int lastPostId) {
      MyUser u = this.users.get(id);
      u.setLastPost(lastPostId);
  }

}

