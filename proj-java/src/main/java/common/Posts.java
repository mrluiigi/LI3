import java.util.List;
import java.util.LinkedList;
import java.util.Map;
import java.util.HashMap;
import java.util.stream.Collectors;
import java.time.LocalDate;


public class Posts
{
    private List<Post> list;
    private Map<Integer, Post> hash;
    private Map<Integer, Post> monthsHash;


    public Posts(){
        this.list = new LinkedList();
        this.hash = new HashMap<>();
        this.monthsHash = new HashMap<>();
    }

    public Posts(Posts p){
        this.setList(p.getList());
        this.setHash(p.getHash());
        this.setMonthsHash(p.getMonthsHash());
    }

    public List<Post> getList(){
        return this.list.stream().map(Post::clone).collect(Collectors.toList());
    }

    public Map<Integer, Post> getHash(){
        Map<Integer, Post> res = new HashMap<>();
        this.hash.values().forEach( p -> res.put(p.getId(), p.clone()));
        return res;
    }

    public Map<Integer, Post> getMonthsHash(){
        Map<Integer, Post> res = new HashMap<>();
        this.monthsHash.keySet().forEach( i -> res.put(i, this.monthsHash.get(i)));
        return res;
    }

    public void setList(List<Post> list){
        this.list = new LinkedList<>();
        list.forEach(p -> this.list.add(p.clone()));
    }

    public void setHash(Map<Integer, Post> hash){
        this.hash = new HashMap<>();
        hash.keySet().forEach( i -> this.hash.put(i, hash.get(i)));
    }

    public void setMonthsHash(Map<Integer, Post> months){
        this.monthsHash = new HashMap<>();
        months.keySet().forEach( i -> this.monthsHash.put(i, months.get(i)));
    }

    public Posts clone(){
        return new Posts(this);
    }

    public void addQuestion(String title, int nanswers, Map<Integer, String> tags, LocalDate lastActivityDate,
                            char postTypeId, int id, int ownerUserId, LocalDate creationDate){
        Question q = new Question(title, nanswers, tags, lastActivityDate, postTypeId, id, ownerUserId, creationDate);
        this.list.add(q);
    }

    public void addAnswer(int parentId, int comments, int score, char postTypeId,
                          int id, int ownerUserId, LocalDate creationDate){
        Answer a = new Answer(parentId, comments, score, postTypeId, id, ownerUserId, creationDate);
        this.list.add(a);
    }

    public Post findPost(int id){
        return this.list.get(id);
    }

    /**
      * Função que converte a CreationDate do XML numa key para ser usada na monthshash
      */
    public int date_to_key(int year, int month){
        return (year * 100) + month;
    }

    //VERIFICAR COMPARADOR e valores adicionados aos map
    public void finalize(){
        this.list.sort( (Post p1, Post p2) -> p2.getCreationDate().compareTo(p1.getCreationDate()));

        for(Post p : this.list){
            this.hash.put(p.getId(), p);
            LocalDate data = p.getCreationDate();
            this.monthsHash.put(this.date_to_key(data.getYear(), data.getMonthValue()) , p);
        }
    }

    //VERIFICAR
    public int getParentOwner(Post p){
        if(p instanceof Answer){
            Answer a = (Answer) p;
            Post parent = this.findPost(a.getParentId());
            return parent.getOwnerUserId();
        }
        else{
            return -1;
        }
    }
}
