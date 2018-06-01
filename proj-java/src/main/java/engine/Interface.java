import org.w3c.dom.*;
import org.xml.sax.SAXException;

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

public class Interface
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
               int ownerUserId = Integer.parseInt(e.getAttribute("OwnerUserId"));
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
           posts.finalize();
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
    * Método para fazer load aos ficheiros necessários para as queries
    */
   public void load(String dumpPath){
       this.loadUsers(dumpPath);
       this.loadPosts(dumpPath);
   }

}
