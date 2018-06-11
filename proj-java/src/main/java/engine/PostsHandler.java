package engine;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import common.Post;
import common.Answer;
import common.Question;
import common.Posts;
import java.time.LocalDate;
import java.util.HashMap;
import java.util.List;


public class PostsHandler extends DefaultHandler {

   public Posts posts;
   private Tags tags;



   public PostsHandler(Tags tags) {
      this.posts = new Posts();
      this.tags = tags;
   }

   public LocalDate createDate(String data){
       int ano = Integer.parseInt(data.substring(0, 4));
       int mes = Integer.parseInt(data.substring(5, 7));
       int dia = Integer.parseInt(data.substring(8, 10));
       LocalDate res = LocalDate.of(ano, mes, dia);
       return res;
   }
   

   @Override
   public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
      if (qName.equals("row")) {
         char postTypeId = attributes.getValue("PostTypeId").charAt(0);
         if(postTypeId == '1' || postTypeId == '2'){
            int id = Integer.parseInt(attributes.getValue("Id"));
            int ownerUserId ;
            try {
               ownerUserId = Integer.parseInt(attributes.getValue("OwnerUserId"));
            }
            catch(NumberFormatException exc){
               ownerUserId = 0;
            }
            LocalDate creationDate = createDate(attributes.getValue("CreationDate"));
            if(postTypeId == '1'){
                String title = attributes.getValue("Title");
                int nanswers = Integer.parseInt(attributes.getValue("AnswerCount"));
                List<Integer> tags = this.tags.getTags(attributes.getValue("Tags"));
                LocalDate lastActivityDate = createDate(attributes.getValue("LastActivityDate"));
                posts.addQuestion(title, nanswers, tags, lastActivityDate, postTypeId, id, ownerUserId, creationDate);
            }
            else if (postTypeId == '2'){
                int parentId = Integer.parseInt(attributes.getValue("ParentId"));
                int comments = Integer.parseInt(attributes.getValue("CommentCount"));
                int score = Integer.parseInt(attributes.getValue("Score"));
                posts.addAnswer(parentId, comments, score, postTypeId, id, ownerUserId, creationDate);
            }
         }
      }
   }
}