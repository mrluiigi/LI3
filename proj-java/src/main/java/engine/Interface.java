package engine;

import common.MyLog;
import common.Pair;

import li3.TADCommunity;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.IOException;
import java.time.LocalDate;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Comparator;



import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;


/**
 * Classe que faz load aos ficheiros .xml e implementa os métodos para cada query
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro "Bichão" Barbosa (A82068)
 * @version 01/06/2018
 */

public class Interface implements TADCommunity
{
   /** Posts */
   private Posts posts;
   /** Users */
   private Users users;
   /** Tags */
   private Tags tags;

   /**
    * Método que permite inicializar a classe Interface
    * (Construtor vazio)
    */
   public Interface(){
       this.posts = new Posts();
       this.users = new Users();
   }

   /**
    * Método para transformar uma String numa LocalDate
    * @param data Data em formato String
    * @return LocalDate 
    */
   public LocalDate createDate(String data){
       int ano = Integer.parseInt(data.substring(0, 4));
       int mes = Integer.parseInt(data.substring(5, 7));
       int dia = Integer.parseInt(data.substring(8, 10));
       LocalDate res = LocalDate.of(ano, mes, dia);
       return res;
   }

   /**
    * Método para fazer load ao ficheiro Users.xml
    * @param dumpPath Path até à pasta do dump
    */
   public void loadUsers(String dumpPath){
       File inputFile = new File(dumpPath + "Users.xml");
       try {

       SAXParserFactory factory = SAXParserFactory.newInstance();

       SAXParser parser = factory.newSAXParser();

       UsersHandler handler = new UsersHandler();

       parser.parse(inputFile, handler);  

       this.users = handler.users;

       } catch (SAXException e) {
           e.printStackTrace();
       } catch (IOException e) {
           e.printStackTrace();
       } catch (ParserConfigurationException e) {
           e.printStackTrace();
       }
   }

   /**
    * Método para fazer load ao ficheiro Posts.xml
    * @param dumpPath Path até à pasta do dump
    */
   public void loadPosts(String dumpPath){
       File inputFile = new File(dumpPath + "Posts.xml");
       try {

       SAXParserFactory factory = SAXParserFactory.newInstance();

       SAXParser parser = factory.newSAXParser();

       PostsHandler handler = new PostsHandler(this.tags);

       parser.parse(inputFile, handler);  

       this.posts = handler.posts;

       posts.finalize();
       } catch (SAXException e) {
           e.printStackTrace();
       } catch (IOException e) {
           e.printStackTrace();
       } catch (ParserConfigurationException e) {
           e.printStackTrace();
       }
   }

   /**
    * Método para fazer load ao ficheiro Tags.xml
    * @param dumpPath Path até à pasta do dump
    */
   public void loadTags(String dumpPath){
       File inputFile = new File(dumpPath + "Tags.xml");
       try {

       SAXParserFactory factory = SAXParserFactory.newInstance();

       SAXParser parser = factory.newSAXParser();

       TagsHandler handler = new TagsHandler();

       parser.parse(inputFile, handler);  

       this.tags = handler.tags;

       } catch (SAXException e) {
           e.printStackTrace();
       } catch (IOException e) {
           e.printStackTrace();
       } catch (ParserConfigurationException e) {
           e.printStackTrace();
       }
   }

   /**
    * Método para fazer load aos ficheiros necessários para as queries
    * @param dumpPath Path até à pasta do dump
    */
   public void load(String dumpPath){
       this.loadTags(dumpPath);
       this.loadUsers(dumpPath);
       this.loadPosts(dumpPath);
       this.set_users_nr_posts_and_last_post();
   }

   /** 
    * Método que define o número de posts e o lastPost dos users
    */
  public void set_users_nr_posts_and_last_post(){
 
    for(Post p : this.posts.getList()){
      //Se o Post possuir ownerUserId
      if(p.hasOwner()){
      	//Define o lastPost
        this.users.find_and_set_user_lastPost(p.getOwnerUserId(),p.getId());
        //Incrementa o número de posts
        this.users.find_and_increment_user_nr_posts(p.getOwnerUserId());
      }
    }
  }

   /**
	* QUERY 1
 	* Método que dado um ID de um post retorna o título do post e o nome do autor
 	* @param id ID do Post
 	* @return Um par onde o primeiro elemento é o título do post e o segundo o nome do autor 
 	*/
    public Pair<String,String> infoFromPost(long id) {
    	//Caso ao ID fornecido não corresponder nenhum Post
    	if(this.posts.containsPost(id) == false) return new Pair<>("","");
    	//Procura o Post relativo ao ID fornecido
      	Post p = this.posts.findPost(id);
    	Question q;
    	//Se o Post for uma Answer
    	if (p instanceof Answer) {
    		Answer a = (Answer) p;
    		//Vai buscar a Pergunta da Resposta para poder retornar o título desta
    		q = (Question) this.posts.findPost(a.getParentId());
    	}
    	//Se o Post for uma Question
    	else {
    		q = (Question) p;
    	}
    	return new Pair<>(q.getTitle(), this.users.find_user(q.getOwnerUserId()).getName());
    }

    /**
 	 * QUERY 2
 	 * Método que retorna os N utilizadores com maior número de posts de sempre
 	 * @param N Número dos utilizadores que deseja obter
 	 * @return Lista com os utilizadores
 	 */
    public List<Long> topMostActive(int N) {
        if (N == 0) return new ArrayList<>();
        return this.users.get_N_users_with_most_nr_posts(N);
    }

    /**
     * QUERY 3
 	 * Método que retorna o número total de posts num certo intervalo de tempo
 	 * @param begin Data inicial
 	 * @param end Data final
 	 * @return Par com o número de perguntas e respostas respetivamente
 	 */
    public Pair<Long,Long> totalPosts(LocalDate begin, LocalDate end) {
    	long perguntas = 0;
    	long respostas = 0;
    	//Vai buscar o Post mais recente depois do end
    	Post p = this.posts.find_post_by_date(end);
    	//Se for uma Question incrementa-se a variável perguntas
    	if(p instanceof Question) perguntas++;
    	//Se for Answer incrementa-se a variável respostas
    	else respostas++;

    	//Percorre a lista dos Posts da data final até à data inicial
    	while(this.posts.has_next(p) && (p = this.posts.get_next(p)).getCreationDate().isBefore(begin) == false) {
    		if(p instanceof Question) perguntas++;
    		else respostas++;
    	}

    	if(p.getCreationDate().isBefore(begin) == false) {
    		if(p instanceof Question) perguntas++;
    		else respostas++;
    	}
        return new Pair<>(perguntas,respostas);
    }

    /**
     * QUERY 4
     * Método que retorna todas as perguntas num certo intervalo de tempo contendo uma determinada tag  
     * @param tag Tag
     * @param begin Data inicial
     * @param end Data final
     * @return Lista dos IDs das Question
     */
    public List<Long> questionsWithTag(String tag, LocalDate begin, LocalDate end) {
        List<Long> res = new ArrayList<>();
        int tagid;
        //Vai buscar o ID da tag
        try{
          tagid = this.tags.convert_tag_name_to_id(tag);
        }
        //Se não existir a tag fornecida retorna-se uma List vazia
        catch(Exception TagInexistenteException){
          return res;
        }
    	//Vai buscar o Post mais recente depois do end
        Post p = this.posts.find_post_by_date(end);
    	//Percorre a lista dos Posts da data final até à data inicial
        while(this.posts.has_next(p) && p.getCreationDate().isBefore(begin) == false){
        	//Se o Post for uma Question
        	if(p instanceof Question){
        		//Verifica se a Question contém a tag fornecida
        		if(((Question)p).contains_tag(tagid)){
            		res.add(p.getId());
            	}
          	}
          	//Passa para o próximo Post
          	p = this.posts.get_next(p);
        }
        return res;
    }

    /**
     * QUERY 5
     * Método que devolve a informação do perfil e os IDs dos últimos 10 posts de um certo utilizador
     * @param id ID do utilizador
     * @return Par com a shortBio do user e o a Lista dos últimos 10 posts
     */
    public Pair<String, List<Long>> getUserInfo(long id){
    	//Procura para o user relativo ao ID dado
    	User u = this.users.find_user(id);
    	List<Long> post_history = new ArrayList<>();
    	//Vai buscar a shortBio do user
    	String bio = u.getShortBio();
    	int i = 0;

    	//Caso o utilizador não tenho Posts
    	if(u.getNr_posts() == 0) return (new Pair<>(bio, post_history));

    	//Vai buscar o último Post do utilizador
    	Post p = this.posts.findPost(u.getLastPost());

    	//Percorre a lista dos posts e sai do while caso se tenha passado por todos os post do user ou caso se tenha já passado por 10 posts 
    	while(i < u.getNr_posts() && i < 10 && this.posts.has_next(p)){
    		//Se o Post foi feito pelo user
        	if(p.getOwnerUserId() == id){
        	  	post_history.add(p.getId());
          		i++;
        	}
        	//Passa para o post seguinte
       		p = this.posts.get_next(p);
      	}

      	if(i < 10 && i < u.getNr_posts()){
        	if(p.getOwnerUserId() == id){
          		post_history.add(p.getId());
        	}
      	}

      	Pair<String, List<Long>> res = new Pair<>(bio, post_history);
      	return res;
    }

    /**
     * QUERY 6
     * Método que devolve os IDs das N respostas com mais votos num dado intervalo de tempo
     * @param N Número de respostas desejadas
     * @param begin Data inicial
     * @param end Data final
     * @return Lista dos IDs das respostas
     */
    public List<Long> mostVotedAnswers(int N, LocalDate begin, LocalDate end) {
    	//Declara o Set passado um comparador que ordena por ordem decrescente de Score
    	Set<Answer> ans = new TreeSet<>((Answer a1, Answer a2) -> ((a2.getScore() - a1.getScore() != 0) ?
        (a2.getScore() - a1.getScore()) : (a1.equals(a2) == true ? 0 : 1)));
        
    	//Vai buscar o Post mais recente depois do end
        Post p = this.posts.find_post_by_date(end);

    	//Percorre a lista dos Posts da data final até à data inicial
        while(this.posts.has_next(p) && p.getCreationDate().isBefore(begin) == false){
        	//Se o Post for uma Answer adiciona ao ans
        	if(p instanceof Answer){
            	ans.add((Answer) p);
          	}
          	//Passa para o próximo Post
          	p = this.posts.get_next(p);
        }
      	int i = 0;
      	List<Long> res = new ArrayList<>();
      	Iterator<Answer> iterador = ans.iterator();
      	
      	//Adiciona à lista a retornar N IDs das respostas
      	while(i < N && iterador.hasNext()){
        	res.add(iterador.next().getId());
        	i++;
      	}
      	return res;
    }

    /**
     * QUERY 7
	 * Método que devolve os IDs das N perguntas com mais respostas
	 * @param N Número de perguntas desejadas
     * @param begin Data inicial
     * @param end Data final
     * @return Lista das N perguntas
	 */
    public List<Long> mostAnsweredQuestions(int N, LocalDate begin, LocalDate end) {
    	//Declara o Set passado um comparador que ordena por ordem decrescente de número de respostas
     	 Set<Question> quest = new TreeSet<>((Question q1, Question q2) -> ((q2.getNanswers() - q1.getNanswers() != 0) ?
        	(q2.getNanswers() - q1.getNanswers()) : (q1.equals(q2) == true ? 0 : 1)));
      	
    	//Vai buscar o Post mais recente depois do end
      	Post p = this.posts.find_post_by_date(end);

    	//Percorre a lista dos Posts da data final até à data inicial
      	while(this.posts.has_next(p) && p.getCreationDate().isBefore(begin) == false){
        	  //Se o Post for uma Question adiciona ao quest
        	  if(p instanceof Question){
            	quest.add((Question) p);
          	}
          	//Passa para o próximo Post
          	p = this.posts.get_next(p);
        }
      	int i = 0;
     	List<Long> res = new ArrayList<>();
      	Iterator<Question> iterador = quest.iterator();

      	//Adiciona à lista a retornar N IDs das perguntas
      	while(i < N && iterador.hasNext()){
        	res.add(iterador.next().getId());
        	i++;
      	}
      	return res;
    }

    /**
	 * QUERY 8
	 * Método que devolve os IDs de N perguntas cujos títulos contenham uma dada palavra
	 * @param N Número de perguntas desejadas
	 * @param word Palavra a verificar se o título contém
	 * @return Devolve a lista das N perguntas
	 */
    //FALTA ACABAR DE DOCUMENTAR
    public List<Long> containsWord(int N, String word) {
        List<Long> res = new ArrayList<>();
        int i = 0;


        for(Post p : this.posts.getList()){
          if(p instanceof Question){
            if(((Question) p).getTitle().contains(word) && i < N){
               res.add(p.getId());
               i++;
            }
          }
        }
        return res;
    }

	/**
 	 * QUERY 9
 	 * Método que devolve as últimas N perguntas em que dois utilizadores participaram
 	 * @param N Número de perguntas pretendidas
 	 * @param id1 ID do primeiro utilizador
 	 * @param id2 ID do segundo utilizador
 	 * @return Lista com os IDs das perguntas
	 */
    public List<Long> bothParticipated(int N, long id1, long id2) {
        List<Long> queuque1 = new LinkedList<>();
    	List<Long> queuque2 = new LinkedList<>();
    	List<Long> queuquef = new LinkedList<>();
    	List<Long> res = new ArrayList<>();
    	//Vai buscar o primeiro utilizador
    	User u1 = this.users.find_user(id1);
    	//Vai buscar o segundo utilizador
    	User u2 = this.users.find_user(id2);
    	int n1 = 0;
    	int n2 = 0;
    	//Vai buscar o último Post do primeiro utilizador
    	Post p1 = this.posts.findPost(u1.getLastPost());
    	//Vai buscar o último Post do segundo utilizador
    	Post p2 = this.posts.findPost(u2.getLastPost());
    	Post p;

    	if (p1.getCreationDate().isAfter(p2.getCreationDate())) {
    		p = p1;
    	}
    	else {
    		p = p2;
    	}

    	/*Percorre a lista de posts a partir do último post mais recente. O ciclo para caso te tenha percorrido a lista toda 
    	ou caso se tenha percorrido todos os posts de um dos utilizadores*/
    	while(this.posts.has_next(p) && n1 < u1.getNr_posts() && n2 < u2.getNr_posts()) {
    		//Se o Post for uma Answer
    		if (p instanceof Answer) {
    			Answer a = (Answer) p;
    			//Caso o OwnerUserID do post for o primeiro utilizador
    			if(a.getOwnerUserId() == id1) {
    				//Incrementa-se o contador do número de posts do primeiro user pelos quais já se passaram.
    				n1++;
    				//Caso o utilizador que fez a pergunta tenha sido o segundo utilizador adiciona à lista queuquef
    				if(this.posts.get_parent_owner(a) == id2) {
       					queuquef.add(a.getParentId());
    				}
    				//Se a resposta existir na queuque2 adiciona-se à queuquef e remove-se da queuque2
    				else if (queuque2.contains(a.getId())) {
    					queuquef.add(a.getParentId());
    					queuque2.remove(a.getParentId());
    				}
    				//Caso contrário adiciona-se à queuque1
    				else {
    					queuque1.add(a.getId());
    				}
    			}
    			
    			//Caso o OwnerUserID do post for o segundo utilizador
    			if(a.getOwnerUserId() == id2) {
    				//Incrementa-se o contador número de posts do segundo user pelos quais já se passaram.
    				n2++;
    				//Caso o utilizador que fez a pergunta tenha sido o primeiro utilizador adiciona à lista queuquef
    				if(this.posts.get_parent_owner(a)  == id1) {
    					queuquef.add(a.getParentId());
    				}
    				//Se a resposta existir na queuque1 adiciona-se à queuquef e remove-se da queuque1
    				else if (queuque1.contains(a.getId())) {
    					queuquef.add(a.getParentId());
    					queuque1.remove(a.getParentId());
    				}
    				//Caso contrário adiciona-se à queuque2
    				else {
    					queuque2.add(a.getParentId());
    				}
    			}
    		}
    		else if ((p.getOwnerUserId() == id1 || p.getOwnerUserId() == id2) && queuquef.contains(p.getId())) {
    			res.add(p.getId());
    		}
    		//Vai buscar o Post seguinte
    		p = this.posts.get_next(p);
    	}
    	return res;
    }

    /**
     * Método para calcular o Score de uma resposta
     * @param ans Answer
     * @param user Utilizador que fez a resposta
     * @return Score da resposta
     */
    private double calculatesScore(Post ans, User user){
      return ((Answer)ans).getScore()*0.45 + user.getReputation()*0.25 + ((Answer)ans).getScore()*0.2 + ((Answer) ans).getComments()*0.1;
    }

    /**
     * QUERY 10
     * Método que obtém a melhor resposta dado o ID de uma pergunta
     * @param id ID da pergunta
     * @return Devolve o ID da melhor resposta
     */ 
    public long betterAnswer(long id) {
      double scoretemp = 0;
      long idtemp = -1;

      //Caso não exista um post relativo ao ID dado
      if(this.posts.containsPost(id) == false){
        return idtemp;
      }

      //Vai buscar o post relativo ao ID dado
      Post pergunta = this.posts.findPost(id);

      //Vai buscar o último post relativo à pergunta
      Post p = this.posts.find_post_by_date(((Question) pergunta).getLastActivityDate());

      //Percorre a lista dos posts até ao post pergunta
      while(p.getId() != id){
        if(p instanceof Answer){

          //Caso a post seja resposta à pergunta
          if(((Answer) p).getParentId() == id){
          	
          	//Calcula o score e verifica se é maior que um melhor score eventualmente calculado anteriormente 
            if(calculatesScore(p, this.users.find_user(p.getOwnerUserId())) > scoretemp){
              scoretemp = calculatesScore(p, this.users.find_user(p.getOwnerUserId()));
              idtemp = p.getId();
            }
          }
        }
        //Vai buscar o próximo post
        p = this.posts.get_next(p);
      }
      return idtemp;
    }

	/**
 	 * QUERY 11
 	 * Método que devolve os IDs das N tags mais utilizadas pelos N utilizadores com mais reputação
 	 * @param N Número de tags desejadas
 	 * @param begin Data inicial
 	 * @param end Data final
 	 * @return Lista dos IDs das tags
 	 */
    public List<Long> mostUsedBestRep(int N, LocalDate begin, LocalDate end) {
    	//Vai buscar a lista dos N users ordenada pela reputação
      	List bestUsers = this.users.get_N_users_with_most_reputation(N);
      	Map<Integer,Integer> map = new HashMap<>();
      	List<Long> res = new ArrayList<>();

      	//Vai buscar o post mais recente depois do end
	    Post p = this.posts.find_post_by_date(end);

	    //Percorre a lista dos post entre a data final e a inicial
    	while(this.posts.has_next(p) && (p.getCreationDate().isBefore(begin) == false)) {

    		//Se o Post for uma pergunta e for de algum dos utilizadores com maior reputação
       		if (p instanceof Question && bestUsers.contains(p.getOwnerUserId())) {
          		Question q = (Question) p;

          		//Para todas as tags dessa pergunta, adiciona-a se não estiver ainda no Map e incrementa caso já esteja
          		for(Integer tid : q.getTags()) {
            		int n = (map.containsKey(tid) ? map.get(tid) : 0);
            		map.put(tid, n+ 1);
          		}	   
        	}
        	//Vai buscar o Post seguint
        	p = this.posts.get_next(p);
      	}

      	//Condição para o último post
      	if(p.getCreationDate().isBefore(begin) == false) {
        	if (p instanceof Question && bestUsers.contains(p.getOwnerUserId())) {
       			Question q = (Question) p;
          		for(Integer tid : q.getTags()) {
            		int n = (map.containsKey(tid) ? map.get(tid) : 0);
            		map.put(tid, n+1);
          		}   
        	}
      	}


      	int i = 0;
     	Set<Tag11> set = new TreeSet<>();

     	//Adiciona ao Set o ID da tag mais o número de ocorrencias que esta teve 
      	map.keySet().stream().forEach(id -> set.add(new Tag11 ( id, map.get(id))));
      	Iterator<Tag11> iterador = set.iterator();

      	//Adiciona à lista a retornar os N IDs das tags mais utilizadas
      	while(i < N && iterador.hasNext()){
        	Tag11 t = iterador.next();
        	res.add((long)t.getId());
        	i++;
      	}
        return res;
    }

    public void clear(){

    }


}
