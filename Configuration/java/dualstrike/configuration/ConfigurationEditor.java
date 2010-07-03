package dualstrike.configuration;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.DefaultListModel;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;

import dualstrike.configuration.model.BooleanSetting;
import dualstrike.configuration.model.ChoiceSetting;
import dualstrike.configuration.model.Configuration;
import dualstrike.configuration.model.Info;
import dualstrike.configuration.model.Option;
import dualstrike.configuration.model.Page;

public class ConfigurationEditor {
	private static final URL DEFAULT_CONFIGURATION_DEFINITION_FILE_URL = createFileURL("configuration.xml");
	private static final int FONT_SIZE = 12;
	private static final Font TITLE_FONT = new Font(Font.SANS_SERIF, Font.BOLD, FONT_SIZE); 
	private static final Font DESCRIPTION_FONT = new Font(Font.SANS_SERIF, Font.PLAIN, FONT_SIZE); 
	private static final Border BOTTOM_SPACER_BORDER = new EmptyBorder(0, 0, 10, 0);

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
	
	private String getLocalizedInfo(List<Info> infos, boolean useHTML) {
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
		
		if(useHTML)
			value = "<html>" + value.replace("\n", "<br/>") + "</html>";
		
		return value;
	}
	
	public void init() {
		String title;
		JFrame frame;
		
		title = getLocalizedInfo(configuration.getTitle(), false);
		frame = new JFrame(title);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.add(createTabs());
		frame.pack();
		frame.setBounds(0, 0, 800, 600);
		frame.setVisible(true);
	}
	
	private Component createTabs() {
		JScrollPane tabsPanel;
		JTabbedPane tabs;

		tabs = new JTabbedPane();

		for(Page page: configuration.getPage()) {
			String tabTitle;
			
			tabTitle = getLocalizedInfo(page.getTitle(), true);
			tabs.addTab(tabTitle, createTabContent(page));
		}

		//tabsPanel = new JScrollPane(tabs, JScrollPane.VERTICAL_SCROLLBAR_NEVER, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

		return tabs;
	}
	
	private Component createTabContent(Page page) {
		String tabHelp;
		JLabel tabHelpLabel;
		JScrollPane tabContent;
		JPanel panel;
		BoxLayout tabLayout;

		panel = new JPanel();
		panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		tabLayout = new BoxLayout(panel, BoxLayout.Y_AXIS);
		panel.setLayout(tabLayout);
		tabHelp =  getLocalizedInfo(page.getHelp(), true);
		tabHelpLabel = new JLabel(tabHelp);
		tabHelpLabel.setFont(DESCRIPTION_FONT);
		tabHelpLabel.setBorder(BOTTOM_SPACER_BORDER);
		panel.add(tabHelpLabel, BorderLayout.LINE_START);
		
		for(Object setting: page.getChoiceOrBoolean())
			addControl(panel, setting);
		
		tabContent = new JScrollPane(panel, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		
		return tabContent;
	}

	private void addControl(JPanel parent, Object settingObject) {
		JPanel settingPanel;
		JPanel infosPanel;
		Component selectorComponent;
		
		settingPanel = new JPanel();
		settingPanel.setBorder(BOTTOM_SPACER_BORDER);
		settingPanel.setAlignmentX(0);
		settingPanel.setLayout(new BoxLayout(settingPanel, BoxLayout.X_AXIS));
		
		if(settingObject instanceof BooleanSetting) {
			BooleanSetting setting;

			setting = (BooleanSetting)settingObject;
			infosPanel = createInfosPanel(setting.getTitle(), setting.getHelp());
			selectorComponent = createSelectorComponent(setting);
		}
		else if(settingObject instanceof ChoiceSetting) {
			ChoiceSetting setting;
			
			setting = (ChoiceSetting)settingObject;
			infosPanel = createInfosPanel(setting.getTitle(), setting.getHelp());
			selectorComponent = createSelectorComponent(setting);
		}
		else {
			throw new Error(String.format("Unexpected class %s of setting object.", settingObject.getClass().getCanonicalName()));
		}
		
		settingPanel.add(infosPanel);
		settingPanel.add(selectorComponent);
		parent.add(settingPanel, BorderLayout.LINE_START);
	}
	
	private JPanel createInfosPanel(List<Info> title, List<Info> help) {
		JPanel infosPanel;
		JLabel titleLabel;
		JLabel helpLabel;
		
		infosPanel = new JPanel();
		infosPanel.setLayout(new BoxLayout(infosPanel, BoxLayout.Y_AXIS));
		titleLabel = createLabel(title, TITLE_FONT);
		infosPanel.add(titleLabel);
		helpLabel = createLabel(help, DESCRIPTION_FONT);
		helpLabel.setLayout(new FlowLayout());
		infosPanel.add(helpLabel);
		
		return infosPanel;
	}
	
	private JLabel createLabel(List<Info> info, Font font) {
		JLabel label;

		label = new JLabel(getLocalizedInfo(info, true));
		label.setFont(font);
		
		return label;
	}
	
	private Component createSelectorComponent(BooleanSetting b) {
		JPanel selectorPanel;
		ButtonGroup buttons;
		boolean isEnabled;

		buttons = new ButtonGroup();
		selectorPanel = new JPanel();
		selectorPanel.setLayout(new BoxLayout(selectorPanel, BoxLayout.Y_AXIS));
		isEnabled = b != null && b.isDefault();
		addRadioButton(MessageHelper.get(this, "trueButtonText", language), buttons, selectorPanel, isEnabled);
		addRadioButton(MessageHelper.get(this, "falseButtonText", language), buttons, selectorPanel, !isEnabled);

		return selectorPanel;
	}
	
	private JRadioButton addRadioButton(String text, ButtonGroup buttons, JPanel panel, boolean selected) {
		JRadioButton button;
		
		button = new JRadioButton(text);
		button.setFont(DESCRIPTION_FONT);
		button.setAlignmentX(0);
		button.setSelected(selected);
		buttons.add(button);
		panel.add(button);
		
		return button;
	}

	private Component createSelectorComponent(ChoiceSetting c) {
		Option defaultOption;
		List<Option> options;
		
		defaultOption = (Option)c.getDefault();
		options = c.getOption();
		
		if(options.size() < 3) {
			ButtonGroup buttons;
			JPanel selectorPanel;

			buttons = new ButtonGroup();
			selectorPanel = new JPanel();
			selectorPanel.setLayout(new BoxLayout(selectorPanel, BoxLayout.Y_AXIS));

			for(Option o: options) {
				boolean isSelected;
				String title;
				String help;
				
				isSelected = o.getId().equals(defaultOption.getId());
				title = getLocalizedInfo(o.getTitle(), true);
				help = getLocalizedInfo(o.getHelp(), true);
				addRadioButton(title, buttons, selectorPanel, isSelected).setToolTipText(help);
			}
			
			return selectorPanel;
		}
		else {
			List<String> titles;
			List<String> helps;
			final String[] helpsArray;
			JList list;
			int selectedIndex;
			
			titles = new LinkedList<String>();
			helps = new LinkedList<String>();
			selectedIndex = -1;
			
			for(Option o: options) {
				if(o.getId().equals(defaultOption.getId()))
					selectedIndex = titles.size();
				
				titles.add(getLocalizedInfo(o.getTitle(), true));
				helps.add(getLocalizedInfo(o.getHelp(), true));
			}
			
			helpsArray = helps.toArray(new String[]{});
			
			list = new JList(titles.toArray()) {
				private static final long serialVersionUID = 1L;
				private String[] helps = helpsArray;

				@Override
				public String getToolTipText(MouseEvent event) {
					int index;
					
					index = locationToIndex(event.getPoint());
					
					return helps[index];
				}
			};

			list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
			list.setLayoutOrientation(JList.VERTICAL);
			list.setVisibleRowCount(3);
			list.setSelectedIndex(selectedIndex);
			
			return list;
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
			try {
				UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
			}
			catch(UnsupportedLookAndFeelException e) {
				throw new Error(e);
			}
			catch(IllegalAccessException e) {
				throw new Error(e);
			}
			catch(InstantiationException e) {
				throw new Error(e);
			}
			catch(ClassNotFoundException e) {
				throw new Error(e);
			}

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
