import java.util.HashMap;
import java.util.TreeSet;
import java.util.Comparator;
import java.util.Iterator;

public class Users{

  private Map<Integer, MyUser> users;

  /**
   * Construtor por omissão
   */
  public Users(){
    this.users = new HashMap<>();
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
    Comparator c = (MyUser u1, MyUser u2) -> u2.getReputation() - u1.getReputation());
    Set<MyUser> set = sort_users(c);
    List<MyUser> res = new ArrayList<>();
    int i = 0;
    Iterator<MyUser> iterador = set.iterator();

    while(i < N && iterador.hasNext()){
      res.add(iterator.next());
      i++;
    }
    return res;
  }

  /**
   * Devolve uma lista com os users que tem o maior número de posts
   */
  public List<MyUser> get_N_users_with_most_nr_posts(int N){
      Comparator c = (MyUser u1, MyUser u2) -> u2.getNr_posts() - u1.getNr_posts());
      List<MyUser> res = new ArrayList<>();
      Set<MyUser> set = sort_users_by_nr_posts(c);
      int i = 0;
      Iterator<MyUser> iterador = set.iterator();

      while(i < N && iterator.hasNext()){
        res.add(iterator.next());
        i++;
      }
      return res;
  }

  /**
   * Incrementa o número de posts de um determinado user
   */
  public void find_and_increment_user_nr_posts(int id) {
    	MyUSER user = this.users.get(id);
    	user.increment_user_nr_posts();
  }

  /**
   * Define o lastPost de uma dado user
   */
  public void find_and_set_user_lastPost(int id, int lastPostId) {
  		MY_USER u = this.users.get(id);
  		if(u)
  			u.set_user_lastPost(lastPostId);
  }

}
