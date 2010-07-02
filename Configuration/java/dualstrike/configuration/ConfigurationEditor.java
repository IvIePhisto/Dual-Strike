package dualstrike.configuration;

import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;

import javax.swing.JFrame;

import dualstrike.configuration.model.Configuration;
import dualstrike.configuration.model.Info;

public class ConfigurationEditor {
	public static final URL DEFAULT_CONFIGURATION_DEFINITION_FILE_URL = createFileURL("configuration.xml");
	private final Configuration configuration;
	private final String language;
	private final String defaultLanguage;
	
	private static URL createFileURL(String path) throws Error {
		try {
			return new File(path).toURI().toURL();
		}
		catch(MalformedURLException e) {
			throw new Error(e);
		}
	}
	
	private ConfigurationEditor(final Configuration configuration, final String language) {
		this.configuration = configuration;
		
		if(language == null)
			this.language = configuration.getLang();
		else
			this.language = language;
		
		defaultLanguage = configuration.getLang();
	}
	
	public static ConfigurationEditor newInstance(URL configurationDefinitionURL, final String language) throws IOException, ConfigurationDefinitionException {
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
		
		try {
			wait(1000L);
		}
		catch(InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) {
			String language;
			URL configurationURL;
			ConfigurationEditor ce;
			
			if(args.length == 0)
				configurationURL = null;
			else
				configurationURL = createFileURL(args[0]);
				
			if(args.length > 1)
				language = args[1];
			else
				language = null;
			
			try {
				ce = ConfigurationEditor.newInstance(configurationURL, language);
			}
			catch(ConfigurationDefinitionException e) {
				System.err.print(e.getLocalizedMessage());
				System.exit(1);
				return;
			}
			catch(IOException e) {
				System.err.printf("Could not load configuration:\n%s", e.getLocalizedMessage());
				System.exit(1);
				return;
			}
			
			ce.init();
			System.exit(0);
	}
}
