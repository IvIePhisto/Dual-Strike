package dualstrike.configuration;

import java.util.Locale;

public class ConfigurationDefinitionException extends Exception {
	private static final long serialVersionUID = 1L;
	private final String url;
	
	public ConfigurationDefinitionException(final String url, final Exception cause, final Locale language) {
		super(MessageHelper.get(ConfigurationDefinitionException.class, "invalidConfigurationDefinition", language, url, cause.getLocalizedMessage()), cause);
		this.url = url;
	}

	public String getUrl() {
		return url;
	}
}
