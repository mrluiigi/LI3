package engine;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import java.time.LocalDate;
import java.util.HashMap;
import java.util.List;

/**
 * Gera uma classe PostsHandler com toda a informação necessária  
 * de todos as Posts presentes no ficheiro Posts.xml
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro Barbosa (A82068) // Grupo 26
 * @version 11/06/2018
 */
public class PostsHandler extends DefaultHandler {

    /** Posts */
    public Posts posts;
    /** Tags */
    private Tags tags;


    /**
     * Método que permite inicializar a classe PostsHandler
     * (Construtor vazio)
     */
    public PostsHandler(Tags tags) {
        this.posts = new Posts();
        this.tags = tags;
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
     * Carrega para a estrutura dos posts todos os Posts presentes no ficheiro
     */
    @Override
    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
        //Se a linha começar por "row"
        if (qName.equals("row")) {
            char postTypeId = attributes.getValue("PostTypeId").charAt(0);
            //Se o Post for uma Question ou uma Answer
            if(postTypeId == '1' || postTypeId == '2'){
                int id = Integer.parseInt(attributes.getValue("Id"));
                int ownerUserId ;
                //Poderá não ter ownerUserId, caso não tenha este fica a 0
                try {
                    ownerUserId = Integer.parseInt(attributes.getValue("OwnerUserId"));
                }
                catch(NumberFormatException exc){
                    ownerUserId = 0;
                }
                LocalDate creationDate = createDate(attributes.getValue("CreationDate"));

                //Se for uma Question
                if(postTypeId == '1'){
                    String title = attributes.getValue("Title");
                    int nanswers = Integer.parseInt(attributes.getValue("AnswerCount"));
                    List<Integer> tags = this.tags.getTags(attributes.getValue("Tags"));
                    LocalDate lastActivityDate = createDate(attributes.getValue("LastActivityDate"));
                    //Adiciona a Question aos Posts 
                    posts.addQuestion(title, nanswers, tags, lastActivityDate, postTypeId, id, ownerUserId, creationDate);
                }

                //Se for uma Answer 
                else if (postTypeId == '2'){
                    int parentId = Integer.parseInt(attributes.getValue("ParentId"));
                    int comments = Integer.parseInt(attributes.getValue("CommentCount"));
                    int score = Integer.parseInt(attributes.getValue("Score"));
                    //Adiciona a Answer aos Posts
                    posts.addAnswer(parentId, comments, score, postTypeId, id, ownerUserId, creationDate);
                }
            }
        }
    }
}