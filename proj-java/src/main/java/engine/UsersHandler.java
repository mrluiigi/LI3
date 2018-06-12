package engine;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import java.time.LocalDate;
import java.util.HashMap;

/**
 * Gera uma classe UsersHandler com toda a informação necessária  
 * de todos os Users presentes no ficheiro Users.xml
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro Barbosa (A82068) // Grupo 26
 * @version 11/06/2018
 */
public class UsersHandler extends DefaultHandler {
   /** Utilizadores */
   public Users users;



   /**
    * Método que permite inicializar a classe UsersHandler
    * (Construtor vazio)
    */
   public UsersHandler() {
      this.users = new Users();
   }

   /** 
    * Método para devolver um LocalDate dado uma data em forma de String
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
    * Carrega para a estrutura dos users todos os users presentes no ficheiro
    */
   @Override
   public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
      //Se a linha começar por "row"
      if (qName.equals("row")) {
        long id = Long.parseLong(attributes.getValue("Id"));
        String name = attributes.getValue("DisplayName");
        String shortBio = attributes.getValue("AboutMe");
        int nr_posts = 0;
        long lastPost = 0;
        long reputation = Long.parseLong(attributes.getValue("Reputation"));
        //Adiciona esse User à variável dos Users
        users.add_myuser(id, name, shortBio, nr_posts, lastPost, reputation);
      }
   }
}