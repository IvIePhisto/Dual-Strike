package dualstrike.configuration;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Locale;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import dualstrike.configuration.model.Configuration;
import dualstrike.configuration.model.Info;

public class ConfigurationEditor {
	public static final URL DEFAULT_CONFIGURATION_DEFINITION_FILE_URL = createFileURL("configuration.xml");
	private final Configuration configuration;
	private final Locale language;
	private final Locale defaultLanguage;
	
	private static URL createFileURL(String path) throws Error {
		try {
			return new File(path).toURI().toURL();
		}
		catch(MalformedURLException e) {
			throw new Error(e);
		}
	}
	
	private ConfigurationEditor(final Configuration configuration, final Locale language) {
		this.configuration = configuration;
		
		if(language == null)
			this.language = new Locale(configuration.getLang());
		else
			this.language = language;
		
		defaultLanguage = new Locale(configuration.getLang());
	}
	
	public static ConfigurationEditor newInstance(URL configurationDefinitionURL, final Locale language) throws IOException, ConfigurationDefinitionException {
		Configuration configuration;
		ConfigurationEditor ce;
		
		if(configurationDefinitionURL == null)
			configurationDefinitionURL = DEFAULT_CONFIGURATION_DEFINITION_FILE_URL;
		
		configuration = ConfigurationUtility.unmarshallConfiguration(configurationDefinitionURL);
		ce = new ConfigurationEditor(configuration, language);
		
		return ce;
	}
	
	public void init() {
		String title;
		String defaultTitle;
		JFrame frame;
		
		title = null;
		defaultTitle = null;
		
		for(Info currentTitle: configuration.getTitle()) {
			String currentLang;
			String value;
			
			currentLang = currentTitle.getLang();
			value = currentTitle.getValue();
			
			if(currentLang == null || currentLang.equals("")) {
				if(defaultLanguage.equals(language))
					title = value;
				else
					defaultTitle = value;
			}
			else if(currentLang.equals(language)) {
				title = value;
				break;
			}
		}
		
		if(title == null)
			title = defaultTitle;
		
		frame = new JFrame(title);

		//2. Optional: What happens when the frame closes?
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		//4. Size the frame.
		frame.pack();

		//5. Show it.
		frame.setVisible(true);
	}
	
	private static void showError(String message, Locale language) {
		JFrame frame;
		String[] options;
		
		options = new String[]{"OK"};
		frame = new JFrame();
		JOptionPane.showOptionDialog(frame, message, MessageHelper.get(ConfigurationEditor.class, "errorMessageTitle", language), JOptionPane.OK_OPTION , JOptionPane.ERROR_MESSAGE, null, options, options[0]);
		System.exit(1);
	}
	
	public static void main(String[] args) {
		Locale language;
		URL configurationURL;
		ConfigurationEditor ce;
		
		if(args.length == 0)
			configurationURL = null;
		else
			configurationURL = createFileURL(args[0]);
			
		if(args.length > 1)
			language = new Locale(args[1]);
		else
			language = Locale.getDefault();
		
		Locale.setDefault(language);
		
		try {
			ce = ConfigurationEditor.newInstance(configurationURL, language);
			ce.init();
		}
		catch(ConfigurationDefinitionException e) {
			showError(e.getLocalizedMessage(), language);
		}
		catch(FileNotFoundException e) {
			showError(MessageHelper.get(ConfigurationEditor.class, "configurationDefinitionNotFound", language, configurationURL), language);
		}
		catch(IOException e) {
			showError(MessageHelper.get(ConfigurationEditor.class, "configurationDefinitionLoadingError", language, e.getLocalizedMessage()), language);
		}
	}
}
