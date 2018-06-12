package engine;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import java.time.LocalDate;
import java.util.HashMap;

/**
 * Gera uma classe TagsHandler com toda a informação necessária  
 * de todas as Tags presentes no ficheiro Tags.xml
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro Barbosa (A82068) // Grupo 26
 * @version 11/06/2018
 */
public class TagsHandler extends DefaultHandler {
    /** Tags */
    public Tags tags;


    /**
     * Método que permite inicializar a classe TagsHandler
     * (Construtor vazio)
     */
    public TagsHandler() {
        this.tags = new Tags();
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
     * Carrega para a estrutura das tags todos as tags presentes no ficheiro
     */
    @Override
    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {  
        //Se a linha começar por "row"
        if (qName.equals("row")){
            String tag_name = attributes.getValue("TagName");
            int tag_id = Integer.parseInt(attributes.getValue("Id"));
            //Adiciona a Tag às tags
            this.tags.insert_tag(tag_name, tag_id);
        }
    }
}