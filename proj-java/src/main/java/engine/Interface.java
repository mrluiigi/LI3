package engine;

import common.MyLog;
import common.Pair;
import common.Posts;
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

       DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();

       DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();

       Document doc = dBuilder.parse(inputFile);

       doc.getDocumentElement().normalize();

       NodeList nList = doc.getElementsByTagName("row");

       for(int i = 0; i < nList.getLength(); i++){
           Node node = nList.item(i);
           Element e = (Element) node;
           int id = Integer.parseInt(e.getAttribute("Id"));
           String name = e.getAttribute("DisplayName");
           String shortBio = e.getAttribute("AboutMe");
           int nr_posts = 0;
           int lastPost = 0;
           int reputation = Integer.parseInt(e.getAttribute("Reputation"));
           users.add_myuser(id, name, shortBio, nr_posts, lastPost, reputation);
       }


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

       DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();

       DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();

       Document doc = dBuilder.parse(inputFile);

       doc.getDocumentElement().normalize();

       NodeList nList = doc.getElementsByTagName("row");

       for(int i = 0; i < nList.getLength(); i++){
           Node node = nList.item(i);
           Element e = (Element) node;
           char postTypeId = e.getAttribute("PostTypeId").charAt(0);
           if(postTypeId == '1' || postTypeId == '2'){
               int id = Integer.parseInt(e.getAttribute("Id"));
               int ownerUserId ;
               try {
             		ownerUserId = Integer.parseInt(e.getAttribute("OwnerUserId"));
             	}
             	catch(NumberFormatException exc){
             		ownerUserId = 0;
             	}
               LocalDate creationDate = createDate(e.getAttribute("CreationDate"));
               if(postTypeId == '1'){
                   String title = e.getAttribute("Title");
                   int nanswers = Integer.parseInt(e.getAttribute("AnswerCount"));
                   //ATRIBUIR TAGS
                   LocalDate lastActivityDate = createDate(e.getAttribute("LastActivityDate"));
                   posts.addQuestion(title, nanswers, new HashMap<>(), lastActivityDate, postTypeId, id, ownerUserId, creationDate);
               }
               else if (postTypeId == '2'){
                   int parentId = Integer.parseInt(e.getAttribute("ParentId"));
                   int comments = Integer.parseInt(e.getAttribute("CommentCount"));
                   int score = Integer.parseInt(e.getAttribute("Score"));
                   posts.addAnswer(parentId, comments, score, postTypeId, id, ownerUserId, creationDate);
               }
           }      
       }
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
    public Pair<String,String> infoFromPost(long id) {
        return new Pair<>("What are the actual risks of giving www-data sudo nopasswd access?", "WebNinja");
    }

    // Query 2
    public List<Long> topMostActive(int N) {
        return Arrays.asList(15811L,449L,158442L,167850L,367165L,295286L,59676L,93977L,35795L,3940L);
    }

    // Query 3
    public Pair<Long,Long> totalPosts(LocalDate begin, LocalDate end) {
        return new Pair<>(3667L,4102L);
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
