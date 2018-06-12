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
    /** Lista dos posts do mais recente para o mais antigo */
    private List<Post> list;
    /** Número de posts */
    private long n;
    /** Map para guardar os posts. A chave é o ID e o valor é o índice na list */
    private Map<Long, Integer> hash;
    /** Map para guardar os post. A chave é o resultado de chamar a função date_to_key e o valor é o índice na list */
    private Map<Integer, Integer > monthsHash;

    /**
     * Método que permite inicializar a classe dos Posts
     * (Construtor por omissão)
     */
    public Posts(){
        this.list = new ArrayList();
        this.n = 0;
        this.hash = new HashMap<>();
        this.monthsHash = new HashMap<>();
    }

    /**
     * Método que permite inicializar a classe dos Posts recebendo como 
     * parâmetro uma classe Posts 
     * (Construtor parametrizado)
     * @param p Posts
     */
    public Posts(Posts p){
        this.setList(p.getList());
        this.setHash(p.getHash());
        this.setMonthsHash(p.getMonthsHash());
    }

    /**
     * Método que devolve uma List com todos os Posts
     * @return List Post
     */
    public List<Post> getList(){
        return this.list.stream().map(Post::clone).collect(Collectors.toList());
    }

    /**
     * Método que devolve o Map da hash.
     * @return Map Integer
     */
    public Map<Long, Integer> getHash(){
        Map<Long, Integer> res = new HashMap<>();
        this.hash.entrySet().forEach(entry -> res.put(entry.getKey(), entry.getValue()));
        return res;
    }

    /**
     * Método que devolve o Map da monthsHash.
     * @return Map Integer
     */
    public Map<Integer, Integer> getMonthsHash(){
        Map<Integer, Integer> res = new HashMap<>();
        this.monthsHash.entrySet().forEach(entry -> res.put(entry.getKey(), entry.getValue()));
        return res;
    }

    /**
     * Método que define a list passado uma List como argumento
     * @param list
     * @return Vazio
     */
    public void setList(List<Post> list){
        this.list = new LinkedList<>();
        list.forEach(p -> this.list.add(p.clone()));
    }

    /**
     * Método que define o hash passado um Map como argumento
     * @param hash
     * @return Vazio
     */
    public void setHash(Map<Long, Integer> hash){
        this.hash = new HashMap<>();
        hash.entrySet().forEach(entry -> this.hash.put(entry.getKey(), entry.getValue()));
    }

    /**
     * Método que define a monthsHash passado um Map como argumento
     * @param months
     * @return Vazio
     */
    public void setMonthsHash(Map<Integer, Integer> months){
        this.monthsHash = new HashMap<>();
        months.entrySet().forEach(entry -> this.monthsHash.put(entry.getKey(), entry.getValue()));
    }

    /**
     * Método que cria uma cópia da Classe
     * @return Posts
     */
    public Posts clone(){
        return new Posts(this);
    }

    /**
     * Método que adiciona uma pergunta à list dos posts
     * @param title
     * @param nanswers
     * @param tags
     * @param lastActivityDate
     * @param postTypeId
     * @param id
     * @param ownerUserId
     * @param creationDate
     */
    public void addQuestion(String title, int nanswers, List<Integer> tags, LocalDate lastActivityDate,
                            char postTypeId, long id, long ownerUserId, LocalDate creationDate){
        Question q = new Question(title, nanswers, tags, lastActivityDate, postTypeId, id, ownerUserId, creationDate);
        this.list.add(q);
    }

    /**
     * Método que adiciona uma resposta à list dos posts
     * @param parentId
     * @param comments
     * @param score
     * @param postTypeId
     * @param id
     * @param ownerUserId
     * @param creationDate
     */
    public void addAnswer(int parentId, int comments, int score, char postTypeId,
                          long id, long ownerUserId, LocalDate creationDate){
        Answer a = new Answer(parentId, comments, score, postTypeId, id, ownerUserId, creationDate);
        this.list.add(a);
    }

    /**
     * Método que devolve um Post dado o id desse Post.
     * @param id
     * @return Post
     */
    public Post findPost(long id){
        return this.list.get(this.hash.get(id));
    }

    public Boolean containsPost(long id){
        if(this.hash.containsKey(id)){
            return true;
        }
        return false;
    }

    /**
      * Método que converte a CreationDate do XML numa key para ser usada na monthshash.
      * @param date
      * @return int
      */
    public int date_to_key(LocalDate date){
        int year = date.getYear();
        int month = date.getMonthValue();
        return (year * 100) + month;
    }

    /**
     * Método a chamar depois de inseridos todos os posts para garantir o funcionamento correto do módulo.
     * @return Vazio
     */
    public void finalize(){
    	int i = 0;
        //Ordena a list do mais recente para o mais antigo
        this.list.sort( (Post p1, Post p2) -> p2.getCreationDate().compareTo(p1.getCreationDate()));

        for(Post p : this.list){
            //Coloca o Post p na hash
            this.hash.put(p.getId(), i);
            LocalDate data = p.getCreationDate();
            //Coloca o Post p na monthsHash se é o primeiro post desse mês.
            this.monthsHash.putIfAbsent(this.date_to_key(data) , i);
            i++;
        }
        //Define o número de Posts
        n = i;
    }

    /**
     * Método que devolve o ID do utilizador que publicou o Post ao qual a resposta atual respondeu
     * @param p Post
     * @return long 
     */
    public long getParentOwner(Post p){
        //Se o post for uma resposta
        if(p instanceof Answer){
            Answer a = (Answer) p;
            //Procura o Post da pergunta a essa resposta
            Post parent = this.findPost(a.getParentId());
            return parent.getOwnerUserId();
        }
        //Se não for uma resposta devolve -1
        else{
            return -1;
        }
    }

    /**
     * Método que devolve uma List dos Posts entre um intervalo de tempo
     * @param begin Data do ínicio
     * @param end Data do fim
     * @return List<Post> com os Posts dentro desse intervalo de tempo
     */
    public List<Post> getPostsTimeInterval(LocalDate begin, LocalDate end) {
        List<Post> res = new ArrayList<>();
        //Encontra o índice da list onde começa o intervalo de tempo
        int i = find_by_date(end);
        for (; this.list.get(i).getCreationDate().isAfter(end); i++);
        //Enquanto não se chegar à data inicial, copia-se os Posts para a lista a retornar
        for (; this.list.get(i).getCreationDate().isBefore(begin) == false; i++) {
            res.add(this.list.get(i).clone());
        }
        return res;
    } 

    /**
     * Método que devolve o Post pertencente à data passada como argumento
     * @param data
     * @return Post
     */
    public Post find_post_by_date(LocalDate data) {
        LocalDate most_recent = this.list.get(0).getCreationDate();

        //Se a data for anterior à mais recente, o Post procurado não existe
        if(data.isAfter(most_recent)) {
            return null;
        }
        else {
            int i = this.monthsHash.get(date_to_key(data));
            //Vai até à posição do Post procurado
            for (; this.list.get(i).getCreationDate().isAfter(data); i++);
            return this.list.get(i);
        }   
    }

    /**
     * Método que devolve a posição na list do primeiro post depois da data passada 
     * @param data
     * @return int Índice na List
     */
    public int find_by_date(LocalDate data) {
        LocalDate most_recent = this.list.get(0).getCreationDate();

        //Se a data passada for depois da primeira data, o índice é o 0
        if(data.isAfter(most_recent)) {
            return 0;
        }
        else {
            //Vai buscar o índice do primeiro Post do mês da data passada
            int i = this.monthsHash.get(date_to_key(data));
            for (; this.list.get(i).getCreationDate().isAfter(data); i++);
            return i;
        }   
    }

    /**
     * Método para verificar se existe um Post a seguir ao Post passado como argumento na lista
     * @param p Post
     * @return boolean true se houver Post a seguir ao dado como argumento, false se não houver mais Posts
     */
    public boolean has_next(Post p) {
        //Vai buscar a posição do Post passado como argumento
        int i = this.hash.get(p.getId());
        return (i + 1 < this.n);
    }

    /**
     * Método que devolve o próximo Post da list
     * @param p Post
     * @return Post O próximo Post
     */
    public Post get_next(Post p) {
        //Vai buscar a posição do Post passado como argumento
        int i = this.hash.get(p.getId());
        return this.list.get(i+1);
    }

    /**
     * Método que devolve o ID do utilizador que publicou o post ao qual a resposta atual respondeu
     * @param a Answer
     * @return long
     */
    public long get_parent_owner(Answer a) {
        if (this.hash.containsKey(a.getParentId())) {
            return this.list.get(this.hash.get(a.getParentId())).getOwnerUserId();
        }
        else return 0;
    }
}
