package mccf;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.Locale;

import mccf.definition.Configuration;
import mccf.model.ConfigurationModel;
import mccf.views.swing.ConfigurationFrameView;


public class ConfigurationEditor {
	public static final int MAJOR_VERSION_NO = 1;
	public static final int MINOR_VERSION_NO = 2;
	public static final int BUGFIX_VERSION_NO = 0;

	static final File DEFAULT_CONFIGURATION_DEFINITION_FILE = new File("configuration.xml");



	static void startSwingEditor(File configurationDefinitionFile, final Locale language) throws IOException, ConfigurationDefException {
		URL configurationDefinitionURL;
		URL configurationDefDirectoryURL;
		Configuration configuration;
		ConfigurationModel model;
		
		if(configurationDefinitionFile == null)
			configurationDefinitionFile = DEFAULT_CONFIGURATION_DEFINITION_FILE;
		
		configurationDefinitionURL = configurationDefinitionFile.toURI().toURL();
		configurationDefDirectoryURL = configurationDefinitionFile.getAbsoluteFile().getParentFile().toURI().toURL();
		configuration = ConfigurationDefUtility.unmarshallConfigurationDef(configurationDefinitionURL);
		model = new ConfigurationModel(configuration);
		new ConfigurationFrameView(configurationDefDirectoryURL, model, configuration, language);
	}
}
