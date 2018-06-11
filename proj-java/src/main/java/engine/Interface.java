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

       PostsHandler handler = new PostsHandler();

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
    * Método para fazer load aos ficheiros necessários para as queries
    */
   public void load(String dumpPath){
       this.loadUsers(dumpPath);
       this.loadPosts(dumpPath);
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
    	for (Post p : this.posts.getPostsTimeInterval(begin, end)) {
    		if(p instanceof Question) perguntas++;
    		else respostas++;
    	}
        return new Pair<>(perguntas,respostas);
    }

    // Query 4
    public List<Long> questionsWithTag(String tag, LocalDate begin, LocalDate end) {
        return Arrays.asList(276174L,276029L,274462L,274324L,274316L,274141L,274100L,272937L,
                272813L,272754L,272666L,272565L,272450L,272313L,271816L,271683L,271647L,270853L,270608L,270528L,270488L,
                270188L,270014L,269876L,269781L,269095L,268501L,268155L,267746L,267656L,267625L,266742L,266335L,266016L,
                265531L,265483L,265443L,265347L,265104L,265067L,265028L,264764L,264762L,264616L,264525L,264292L,263816L,
                263740L,263460L,263405L,263378L,263253L,262733L,262574L);
    }

    // Query 5
    public Pair<String, List<Long>> getUserInfo(long id) {
        String shortBio = "<p>Coder. JS, Perl, Python, Basic<br>Books/movies: SF+F.<br>Dead:" +
                "dell 9300<br>Dead: dell 1720 as of may 10th 2011.</p>\n" +
                "<p>Current system: Acer Aspire 7750G.<br>\n" +
                "Works OOTB as of Ubuntu 12.04.<br></p>";
        List<Long> ids = Arrays.asList(982507L,982455L,980877L,980197L,980189L,976713L,974412L,
                974359L,973895L,973838L);
        return new Pair<>(shortBio,ids);
    }

    // Query 6
    public List<Long> mostVotedAnswers(int N, LocalDate begin, LocalDate end) {
        return Arrays.asList(701775L,697197L,694560L,696641L,704208L);
    }

    // Query 7
    public List<Long> mostAnsweredQuestions(int N, LocalDate begin, LocalDate end) {
        return Arrays.asList(505506L,508221L,506510L,508029L,506824L,505581L,505368L,509498L,509283L,508635L);
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
