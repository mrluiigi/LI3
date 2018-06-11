package engine;

import java.lang.String;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

public class Tags{
	/** Map para guardar as tags */
	private Map<String, Integer> tags;
	/**
	 * Construtor vazio
	 */
	public Tags(){
		this.tags = new HashMap<>();
	}

	/**
	 * Insere uma tag no Map
	 */
	public void insert_tag(String name, int id){
		this.tags.put(name, id);
	}

	/**
	 * Devolve o ID de uma tag
	 */
	public int convert_tag_name_to_id(String name) throws TagInexistenteException{
		if(this.tags.containsKey(name))
			return this.tags.get(name);
		else
			throw new TagInexistenteException(name);
	}

	/**
	 * Recebe a string das tags e coloca-as numa lista ligada
	 */
	public List<Integer> getTags(String t){
		List<Integer> l = new ArrayList<>();

		int length = t.length();
		String tags = t.substring(1, length-1);


		String[] aux = tags.split("><", -2);
		for(String s : aux){
			l.add(this.tags.get(s));
		}
		return l;
	}
}