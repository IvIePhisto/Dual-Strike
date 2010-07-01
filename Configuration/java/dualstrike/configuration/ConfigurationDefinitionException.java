package dualstrike.configuration;

public class ConfigurationDefinitionException extends Exception {
	private static final long serialVersionUID = 1L;
	private final String url;
	
	public ConfigurationDefinitionException(final String url, final Exception cause) {
		super(String.format("Configuration \"%s\" invalid:\n%s", url, cause.getLocalizedMessage()), cause);
		this.url = url;
	}

	public String getUrl() {
		return url;
	}
}
