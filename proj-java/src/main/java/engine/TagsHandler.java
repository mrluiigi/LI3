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


public class TagsHandler extends DefaultHandler {

  public Tags tags;


  public TagsHandler() {
    this.tags = new Tags();
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
    if (qName.equals("row")){
      String tag_name = attributes.getValue("TagName");
      int tag_id = Integer.parseInt(attributes.getValue("Id"));
      this.tags.insert_tag(tag_name, id);
    }
  }
  
}