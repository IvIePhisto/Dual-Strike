package dualstrike.configuration;

import java.awt.Component;
import java.awt.Font;
import java.awt.GridLayout;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.List;
import java.util.Locale;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;

import dualstrike.configuration.model.BooleanSetting;
import dualstrike.configuration.model.ChoiceSetting;
import dualstrike.configuration.model.Configuration;
import dualstrike.configuration.model.Info;
import dualstrike.configuration.model.Option;
import dualstrike.configuration.model.Page;

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
	
	private String getLocalizedInfo(List<Info> infos) {
		String value;
		String defaultValue;
		
		value = null;
		defaultValue = null;
		
		for(Info currentTitle: infos) {
			String currentLang;
			String currentValue;
			
			currentLang = currentTitle.getLang();
			currentValue = currentTitle.getValue();
			
			if(currentLang == null || currentLang.equals("")) {
				if(defaultLanguage.equals(language.getLanguage()))
					value = currentValue;
				else
					defaultValue = currentValue;
			}
			else if(currentLang.equals(language.getLanguage())) {
				value = currentValue;
				break;
			}
		}
		
		if(value == null)
			value = defaultValue;

		return value;
	}
	
	public void init() {
		String title;
		JFrame frame;
		
		title = getLocalizedInfo(configuration.getTitle());
		frame = new JFrame(title);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.add(createTabs());
		frame.pack();
		frame.setVisible(true);
	}
	
	private JTabbedPane createTabs() {
		JTabbedPane tabs;

		tabs = new JTabbedPane();
		
		for(Page page: configuration.getPage()) {
			String tabTitle;
			
			tabTitle = getLocalizedInfo(page.getTitle());
			tabs.addTab(tabTitle, createTabContent(page));
		}

		return tabs;
	}
	
	private Component createTabContent(Page page) {
		String tabHelp;
		JLabel tabHelpLabel;
		JScrollPane tabContent;
		JPanel panel;;

		panel = new JPanel();
		panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		panel.setLayout(new GridLayout());
		tabHelp =  getLocalizedInfo(page.getHelp());
		tabHelpLabel = new JLabel(tabHelp);
		panel.add(tabHelpLabel);
		
		for(Object setting: page.getChoiceOrBoolean())
			addControl(panel, setting);
		
		tabContent = new JScrollPane(panel);
		
		return tabContent;
	}

	private void addControl(JPanel parent, Object settingObject) {
		JPanel settingPanel;
		
		settingPanel = new JPanel();
		settingPanel.setLayout(new GridLayout(1, 2));
		
		if(settingObject instanceof BooleanSetting) {
			BooleanSetting setting;

			setting = (BooleanSetting)settingObject;
			addInfos(settingPanel, setting.getTitle(), setting.getHelp());
			addSelector(parent, setting);
		}
		else if(settingObject instanceof ChoiceSetting) {
			ChoiceSetting setting;
			
			setting = (ChoiceSetting)settingObject;
			addInfos(parent, setting.getTitle(), setting.getHelp());
			addSelector(parent, setting);
		}
		else {
			throw new Error(String.format("Unexpected class %s of setting object.", settingObject.getClass().getCanonicalName()));
		}
	}
	
	private void addInfos(JPanel parent, List<Info> title, List<Info> help) {
		JPanel infosPanel;
		JLabel titleLabel;
		JLabel helpLabel;
		
		infosPanel = new JPanel();
		infosPanel.setLayout(new GridLayout());
		titleLabel = new JLabel(getLocalizedInfo(title));
		titleLabel.setFont(new Font(titleLabel.getFont().getName(), Font.BOLD, titleLabel.getFont().getSize()));
		helpLabel = new JLabel(getLocalizedInfo(help));
		infosPanel.add(titleLabel);
		infosPanel.add(helpLabel);
		parent.add(infosPanel);
	}
	
	private void addSelector(JPanel parent, BooleanSetting b) {
		JPanel selectorPanel;
		JRadioButton trueButton;
		JRadioButton falseButton;
		ButtonGroup buttons;

		trueButton = new JRadioButton(MessageHelper.get(this, "trueButtonText", language));
		falseButton = new JRadioButton(MessageHelper.get(this, "falseButtonText", language));
		
		if(b.isDefault())
			trueButton.setEnabled(true);
		else
			falseButton.setEnabled(true);
		
		buttons = new ButtonGroup();
		buttons.add(trueButton);
		buttons.add(falseButton);
		selectorPanel = new JPanel();
		selectorPanel.add(trueButton);
		selectorPanel.add(falseButton);
		parent.add(selectorPanel);
	}

	private void addSelector(JPanel parent, ChoiceSetting c) {
		List<Option> options;
		
		options = c.getOption();
		
		if(options.size() < 5) {
			ButtonGroup buttons;

			
		}
		else {
			
		}
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
