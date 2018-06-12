package engine;



/**
 * Classe que contém a excepção que ocorre quando se procura por um utilizador inexistente.
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro "Bichão" Barbosa (A82068)
 * @version 11/06/2018
 */
public class UserInexistenteException extends Exception
{   
	/**
     * Excepção que ocorre quando se procura por um utilizador inexistente
     * @param id Id que levou a criar a excepçao
     */
    public UserInexistenteException(long id){
        super("Não exite o post " + id);
    }
}
