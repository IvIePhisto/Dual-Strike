package mccf;

import java.util.Locale;

public class ConfigurationDefException extends Exception {
	private static final long serialVersionUID = 1L;
	private final String url;
	
	public ConfigurationDefException(final String url, final Exception cause, final Locale language) {
		super(MessageHelper.get(ConfigurationDefException.class, "invalidConfigurationDefinition", language, url, cause.getLocalizedMessage()), cause);
		this.url = url;
	}

	public String getUrl() {
		return url;
	}
}
