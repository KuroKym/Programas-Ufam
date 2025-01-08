package br.edu.ufam.icomp.lab_excecoes;

public class CoordenadaNegativaException extends RoverCoordenadaException {
	private static final long serialVersionUID = 1L;
	public CoordenadaNegativaException() {
		super();
	}
	public CoordenadaNegativaException(String s) {
		super(s);
	}
	public String getMessage() {
		return "Coordenada com valor negativo";
	}
}
