package engine;

import java.time.LocalDate;


/**
 * Classe que contém a excepção que ocorre quando se procura por um post inexistente.
 *
 * @author José Pinto (A81317); Luís Correia (A81141); Pedro "Bichão" Barbosa (A82068)
 * @version 11/06/2018
 */
public class PostInexistenteException extends Exception
{   
	/**
     * Excepção que ocorre quando se procura por um post inexistente
     * @param id Id que levou a criar a excepçao
     */
    public PostInexistenteException(long id){
        super("Não exite o post " + id);
    }

    /**
     * Excepção que ocorre quando se procura por um post inexistente
     * @param date Data antes da qual não há posts.
     */
    public PostInexistenteException(LocalDate date){
        super("Não existem posts antes de" + date.toString());
    }
}
