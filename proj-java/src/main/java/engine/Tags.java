package common;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

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
public int convert_tag_name_to_id(String name) throws TagInexistente{
	if(this.tags.contains(name))
		return this.tags.get(name);
	else
		throw new TagInexistente(name);
}

/**
 * Recebe a string das tags e coloca-as numa lista ligada
 */
public List<Integer> getTags(String tags){
	List<Integer> l = new ArrayList<>();

	int i = 0;
	StringBuilder res = new StringBuilder();
	int valor;

	while(tags.charAt(i) != '\0'){
		i++;
		while(tags.charAt(i) != '>'){
			res.append(tags.charAt(i).toString());
			i++;
		}
		res.append("\0");
		valor = this.tags(res.toString());
		l.add(valor);
		res = new StringBuilder();
		i++;
	}
	return l;
}