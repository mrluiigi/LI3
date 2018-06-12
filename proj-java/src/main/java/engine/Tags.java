package engine;

import java.lang.String;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

/**
 * Classe que guarda informação sobre as tags
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro Barbosa (A82068) // Grupo 26
 * @version 11/06/2018
 */
public class Tags{
	/** Map para guardar as tags. Chave é o nome da Tag, valor é o ID da tag */
	private Map<String, Integer> tags;


	/**
	 * Método que permite inicializar a classe Tags
	 * (Construtor vazio)
	 */
	public Tags(){
		this.tags = new HashMap<>();
	}

	/**
	 * Método que insere uma tag no Map
	 * @param name Nome da Tag
	 * @param id ID da Tag
	 */
	public void insert_tag(String name, int id){
		this.tags.put(name, id);
	}

	/**
	 * Método que devolve o ID de uma tag
	 * @param name Nome da Tag
	 * @return int ID da tag caso ela exita
	 * @see TagInexistenteException
	 */
	public int convert_tag_name_to_id(String name) throws TagInexistenteException{
		//Se a tag existir
		if(this.tags.containsKey(name))
			return this.tags.get(name);
		else
			throw new TagInexistenteException(name);
	}

	/**
	 * Método que recebe a string das tags e coloca-as numa lista ligada
	 * @param t String contendo todas as tags
	 * @return Lista com os IDs de cada tag
	 */
	public List<Integer> getTags(String t){
		List<Integer> l = new ArrayList<>();

		int length = t.length();
		//Ignora o primeiro char (<) e o último char (>) 
		String tags = t.substring(1, length-1);

		//Separa as strings que estão entre ><
		String[] aux = tags.split("><", -2);
		//Adiciona na List os IDs das tags
		for(String s : aux){
			l.add(this.tags.get(s));
		}
		return l;
	}
}