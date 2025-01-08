package br.edu.ufam.icomp.lab_excecoes;

public class DigitoInvalidoException extends RoverCoordenadaException {
	private static final long serialVersionUID = 1L;
	public DigitoInvalidoException() {
		super();
	}
	public DigitoInvalidoException(String s) {
		super(s);
	}
	public String getMessage() {
		return "Digito da coordenada inválido";
	}
}
