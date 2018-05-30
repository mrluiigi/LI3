public class MyDate{
  /** Data inicial */
  LocalDate inicio;
  /** Data final */
  LocalDate fim;

  /**
   *  Cria duas datas com base nos parâmetros recebidos
   */
  public MyData(int ano_inicio, int mes_inicio, int dia_inicio, int ano_fim, int mes_fim, int dia_fim){
    this.inicio = LocalDate.of(ano_inicio, mes_inicio, dia_inicio);
    this.fim = LocalDate.of(ano_fim, mes_fim, dia_fim);
  }

  /**
   * Devolve a data inicial
   */
  public LocalDate getInicio(){
    return this.inicio;
  }

  /**
   * Devolve a data final
   */
  public LocalDate getFim(){
    return this.fim;
  }

  /**
  * Define a data inicial
  */
  public void setInicio(LocalDate data){
    this.inicio = data;
  }

  /**
  * Define a data final
  */
  public void setFim(LocalDate data){
    this.fim = data;
  }

  /**
  * Método que devolve a creation date de um post sobre o formato de LocalDate
  */
  public LocalDate xmlCreationDate_to_LocalDate(char * xmlDate){
     int i = 0;

     s_year = new ArrayList<>();
     s_year.add(xmlDate[2]);
     s_year.add(xmlDate[3]);
     while(i < s_year.size()){
       int year = 2000 + s_year.charAt(i);
       i++;
     }
     
     s_month = new ArrayList<>();
     s_month.add(xmlDate[5]);
     s_month.add(xmlDate[6]);
     i = 0;
     while(i < s_month.size()){
       int month = 2000 + s_month.charAt(i);
       i++;
     }

     i = 0;
     s_day = new ArrayList<>();
     s_day.add(xmlDate[8]);
     s_day.add(xmlDate[9]);
     i = 0:
     while(i < s_day.size()){
       int day = 2000 + s_day.charAt(i);
       i++;
     }

     LocalDate res = LocalDate.of(year, month, day);
     return res
  }

  /**
  * Método que imprime a representação textual do objeto
  */
  public String toString(){
    return "Data inicial: " + this.inicio + "\n" +
           "Data final" + this.fim;
  }

  /**
  * Verifica se dois conjuntos de datas são iguais
  */
  public boolean equals(Object o){
    if(o == this) return true;
    if(o == null || o.getClass() != this.getClass()) return false;
    MyDate d = (MyDate) o;
    return (this.inicio.equals(d.getInicio()) &&
            this.fim.equals(d.getFim()));
  }

  /**
  * Devolve uma cópia da classe
  */
  public MyDate clone(){
    return new MyDate(this);
  }

}