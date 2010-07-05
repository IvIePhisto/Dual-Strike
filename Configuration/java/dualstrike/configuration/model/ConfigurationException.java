package dualstrike.configuration.model;

public class ConfigurationException extends Exception {
	private static final long serialVersionUID = 1L;
	private final Type type;
	
	public static enum Type {
		DEVICE,
		VERSION,
		BYTE_COUNT,
		UNITITIALIZED_DATA
	}
	
	public ConfigurationException(Type type) {
		this.type = type;
	}

	public Type getType() {
		return type;
	}
}
