package engine;

/**
 * Classe auxiliar para Query 11
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro Barbosa (A82068) // Grupo 26
 * @version 12/06/2018
 */

public class Tag11  implements Comparable<Tag11> {
	/** ID da tag */
	private int id;
	/** Número de ocorrencias da tag*/
	private int  ocorrencias;

	/**
	* Método que permite inicializar a classe Tag11
	* (Construtor parametrizado)
	* @param id ID da tag
	* @param ocorrencias Número de ocorrencias da tag
	*/
	public Tag11(int id, int ocorrencias) {
		this.id = id;
		this.ocorrencias = ocorrencias;
	}

	/**
	* Método que verifica se duas Tag11 são iguais
	* @return true se forem iguais, false caso contrário
	*/
	public boolean equals(Object o) {
		if (this == o) return true;
		if(o == null || o.getClass() != this.getClass()) return false;
		Tag11 t = (Tag11) o;
		return(this.id == t.id && this.ocorrencias == t.ocorrencias);
	}

	/**
	 * Método que implementa a ordem natural
	 * @param t Tag11 
	 */
	public int compareTo(Tag11 t) {
		if (this.equals(t)) return 0;
		if (this.ocorrencias > t.ocorrencias) return -1;
		if(this.ocorrencias < t.ocorrencias) return 1;
		if(this.id < t.id) return -1;
		return 1;
	}

	/**
	 * Método que devolve o ID da Tag
	 * @return int ID da tag
	 */
	public int getId() {
		return this.id;
	}

	/**
	 * Método que devolve o número de ocorrencias
	 * @param int Número de ocorrencias da tag
	 */
	public int getOcorrencias() {
		return this.ocorrencias;
	}
}

