package common;

import java.util.List;
import java.util.LinkedList;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.stream.Collectors;
import java.time.LocalDate;


public class Posts
{
    private List<Post> list;
    //<Id, Index na list>
    private Map<Integer, Integer> hash;
    //<Id, Index na list>
    private Map<Integer, Integer > monthsHash;


    public Posts(){
        this.list = new ArrayList();
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

    public Map<Integer, Integer> getHash(){
        Map<Integer, Integer> res = new HashMap<>();
        this.hash.entrySet().forEach(entry -> res.put(entry.getKey(), entry.getValue()));
        return res;
    }

    public Map<Integer, Integer> getMonthsHash(){
        Map<Integer, Integer> res = new HashMap<>();
        this.monthsHash.entrySet().forEach(entry -> res.put(entry.getKey(), entry.getValue()));
        return res;
    }

    public void setList(List<Post> list){
        this.list = new LinkedList<>();
        list.forEach(p -> this.list.add(p.clone()));
    }

    public void setHash(Map<Integer, Integer> hash){
        this.hash = new HashMap<>();
        hash.entrySet().forEach(entry -> this.hash.put(entry.getKey(), entry.getValue()));
    }

    public void setMonthsHash(Map<Integer, Integer> months){
        this.monthsHash = new HashMap<>();
        months.entrySet().forEach(entry -> this.monthsHash.put(entry.getKey(), entry.getValue()));
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
        return this.list.get(this.hash.get(id));
    }

    /**
      * Função que converte a CreationDate do XML numa key para ser usada na monthshash
      */
    public int date_to_key(LocalDate date){
        int year = date.getYear();
        int month = date.getMonthValue();
        return (year * 100) + month;
    }

    //VERIFICAR COMPARADOR e valores adicionados aos map
    public void finalize(){
    	int i = 0;
        this.list.sort( (Post p1, Post p2) -> p2.getCreationDate().compareTo(p1.getCreationDate()));

        for(Post p : this.list){
            this.hash.put(p.getId(), i);
            LocalDate data = p.getCreationDate();
            this.monthsHash.putIfAbsent(this.date_to_key(data) , i);
            i++;
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

    public List<Post> getPostsTimeInterval(LocalDate begin, LocalDate end) {
        List<Post> res = new ArrayList<>();
        int i = find_by_date(end);
        for (; this.list.get(i).getCreationDate().isAfter(end); i++);
        for (; this.list.get(i).getCreationDate().isBefore(begin) == false; i++) {
            res.add(this.list.get(i).clone());
        }
        return res;
    } 


    public int find_by_date(LocalDate data) {
        LocalDate most_recent = this.list.get(0).getCreationDate();

        if(data.isAfter(most_recent)) {
            return 0;
        }
        else {
            int i = this.monthsHash.get(date_to_key(data));
            for (; this.list.get(i).getCreationDate().isAfter(data); i++);
            return i;
        }   
    }

}
