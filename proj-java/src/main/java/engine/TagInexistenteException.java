package engine;

/**
 * Classe que contém a excepção que ocorre quando se procura por uma tag inexistente.
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro "Bichão" Barbosa (A82068)
 * @version 11/06/2018
 */
public class TagInexistenteException extends Exception
{   
	/**
     * Excepção que ocorre quando se procura por uma Tag inexistente
     * @param c Chave que levou a criar a excepçao
     */
    public TagInexistenteException(String c){
        super(c);
    }
}
