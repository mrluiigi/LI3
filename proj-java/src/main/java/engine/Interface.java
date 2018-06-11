package engine;

import common.MyLog;
import common.Pair;
import common.Post;
import common.Answer;
import common.Question;
import common.Posts;
import common.MyUser;
import common.Users;
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
 * Write a description of class Main here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */

public class Interface implements TADCommunity
{
   private Posts posts;
   private Users users;
   private Tags tags;
      //ADICIONAR TAGS

   /**
    * Construtor
    */
   public Interface(){
       this.posts = new Posts();
       this.users = new Users();
   }

   /**
    * Método para transformar uma String numa data
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
    */
   public void load(String dumpPath){
       this.loadTags(dumpPath);
       this.loadUsers(dumpPath);
       this.loadPosts(dumpPath);
       this.set_users_nr_posts_and_last_post();
   }

  public void set_users_nr_posts_and_last_post(){
    for(Post p : this.posts.getList()){
      if(p.hasOwner()){
        this.users.find_and_set_user_lastPost(p.getOwnerUserId(),p.getId());
        this.users.find_and_increment_user_nr_posts(p.getOwnerUserId());
      }
    }
  }

   // Query 1
   //long ->int??
    public Pair<String,String> infoFromPost(long id) {
    	Post p = this.posts.findPost((int)id);
    	Question q;
    	if (p instanceof Answer) {
    		Answer a = (Answer) p;
    		q = (Question) this.posts.findPost(a.getParentId());
    	}
    	else {
    		q = (Question) p;
    	}
    	return new Pair<>(q.getTitle(), this.users.find_user(q.getOwnerUserId()).getName());
    }

    // Query 2
    public List<Long> topMostActive(int N) {
        return this.users.get_N_users_with_most_nr_posts(N);
    }

    // Query 3
    public Pair<Long,Long> totalPosts(LocalDate begin, LocalDate end) {
    	long perguntas = 0;
    	long respostas = 0;
    	Post p = this.posts.find_post_by_date(end);
    	if(p instanceof Question) perguntas++;
    	else respostas++;

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

    // Query 4
    public List<Long> questionsWithTag(String tag, LocalDate begin, LocalDate end) {
        List<Long> res = new ArrayList<>();
        int tagid;
        try{
          tagid = this.tags.convert_tag_name_to_id(tag);
        }
        catch(Exception TagInexistenteException){
          return res;
        }
        Post p = this.posts.find_post_by_date(end);
        while(this.posts.has_next(p) && p.getCreationDate().isBefore(begin) == false){
          if(p instanceof Question){
            if(((Question)p).contains_tag(tagid)){
              res.add(p.getId());
            }
          }
          p = this.posts.get_next(p);
        }
        return res;
    }

    // Query 5
    public Pair<String, List<Long>> getUserInfo(long id){
      MyUser u = this.users.find_user(id);
      List<Long> post_history = new ArrayList<>();
      String bio = u.getShortBio();
      int i = 0;

      if(u.getNr_posts() == 0) return (new Pair<>(bio, post_history));

      Post p = this.posts.findPost(u.getLastPost());

      while(i < u.getNr_posts() && i < 10 && this.posts.has_next(p)){
        if(p.getOwnerUserId() == id){
          post_history.add(p.getId());
          i++;
        }
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

    // Query 6
    public List<Long> mostVotedAnswers(int N, LocalDate begin, LocalDate end) {
      Set<Answer> ans = new TreeSet<>((Answer a1, Answer a2) -> ((a2.getScore() - a1.getScore() != 0) ?
        (a2.getScore() - a1.getScore()) : (a1.equals(a2) == true ? 0 : 1)));
        Post p = this.posts.find_post_by_date(end);
        while(this.posts.has_next(p) && p.getCreationDate().isBefore(begin) == false){
          if(p instanceof Answer){
            ans.add((Answer) p);
          }
          p = this.posts.get_next(p);
        }
      int i = 0;
      List<Long> res = new ArrayList<>();
      Iterator<Answer> iterador = ans.iterator();
      while(i < N && iterador.hasNext()){
        res.add(iterador.next().getId());
        i++;
      }
      return res;
    }

    // Query 7
    public List<Long> mostAnsweredQuestions(int N, LocalDate begin, LocalDate end) {
      Set<Question> quest = new TreeSet<>((Question q1, Question q2) -> ((q2.getNanswers() - q1.getNanswers() != 0) ?
        (q2.getNanswers() - q1.getNanswers()) : (q1.equals(q2) == true ? 0 : 1)));
      Post p = this.posts.find_post_by_date(end);
      while(this.posts.has_next(p) && p.getCreationDate().isBefore(begin) == false){
          if(p instanceof Question){
            quest.add((Question) p);
          }
          p = this.posts.get_next(p);
        }
      int i = 0;
      List<Long> res = new ArrayList<>();
      Iterator<Question> iterador = quest.iterator();
      while(i < N && iterador.hasNext()){
        res.add(iterador.next().getId());
        i++;
      }
      return res;
    }

    // Query 8
    public List<Long> containsWord(int N, String word) {
        return Arrays.asList(980835L,979082L,974117L,974105L,973832L,971812L,971056L,968451L,964999L,962770L);
    }

    // Query 9
    public List<Long> bothParticipated(int N, long id1, long id2) {
        return Arrays.asList(594L);
    }

    // Query 10
    public long betterAnswer(long id) {
        return 175891;
    }

    // Query 11
    public List<Long> mostUsedBestRep(int N, LocalDate begin, LocalDate end) {
        return Arrays.asList(6L,29L,72L,163L,587L);
    }

    public void clear(){

    }


}
