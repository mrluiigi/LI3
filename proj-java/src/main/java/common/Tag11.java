package common;

public class Tag11  implements Comparable<Tag11> {
	private int id;
	private int  ocorrencias;

	public Tag11(int id, int ocorrencias) {
		this.id = id;
		this.ocorrencias = ocorrencias;
	}

	public boolean equals(Object o) {
		if (this == o) return true;
		if(o == null || o.getClass() != this.getClass()) return false;
		Tag11 t = (Tag11) o;
		return(this.id == t.id && this.ocorrencias == t.ocorrencias);
	}

	public int compareTo(Tag11 t) {
		if (this.equals(t)) return 0;
		if (this.ocorrencias > t.ocorrencias) return -1;
		if(this.ocorrencias < t.ocorrencias) return 1;
		if(this.id < t.id) return -1;
		return 1;
	}

	public int getId() {
		return this.id;
	}
	public int getOcorrencias() {
		return this.ocorrencias;
	}

}

