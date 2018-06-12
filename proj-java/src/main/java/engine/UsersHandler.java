package engine;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import java.time.LocalDate;
import java.util.HashMap;


public class UsersHandler extends DefaultHandler {

   public Users users;




   public UsersHandler() {
      this.users = new Users();
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
        long id = Long.parseLong(attributes.getValue("Id"));
        String name = attributes.getValue("DisplayName");
        String shortBio = attributes.getValue("AboutMe");
        int nr_posts = 0;
        long lastPost = 0;
        int reputation = Integer.parseInt(attributes.getValue("Reputation"));
        users.add_myuser(id, name, shortBio, nr_posts, lastPost, reputation);
      }
   }
}