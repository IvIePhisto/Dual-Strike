package dualstrike.configuration;

import java.awt.BorderLayout;
import java.awt.Component;
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
import javax.swing.JComponent;
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
		frame.setVisible(true);
		frame.setLocationRelativeTo(null);
	}
	
	private Component createTabs() {
		JTabbedPane tabs;

		tabs = new JTabbedPane(JTabbedPane.TOP, JTabbedPane.SCROLL_TAB_LAYOUT);

		for(Page page: configuration.getPage()) {
			String tabTitle;
			
			tabTitle = getLocalizedInfo(page.getTitle(), true);
			tabs.addTab(tabTitle, createTabContent(page));
		}

		return tabs;
	}
	
	private Component createTabContent(Page page) {
		JLabel tabHelpLabel;
		JScrollPane tabContent;
		JPanel tabPanel;

		tabPanel = new ResizingJPanel();
		tabPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		tabPanel.setLayout(new BoxLayout(tabPanel, BoxLayout.Y_AXIS));
		tabPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		tabPanel.setAlignmentY(Component.TOP_ALIGNMENT);
		
		tabHelpLabel = createLabel(page.getHelp(), DESCRIPTION_FONT);
		tabHelpLabel.setBorder(BOTTOM_SPACER_BORDER);
		tabPanel.add(tabHelpLabel);
		
		for(Object setting: page.getChoiceOrBoolean()) {
			JComponent settingComponent;
			
			settingComponent = createControl(setting);
			tabPanel.add(settingComponent);
		}
		
		tabContent = new JScrollPane(tabPanel, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		
		return tabContent;
	}

	private JComponent createControl(Object settingObject) {
		JPanel settingPanel;
		JComponent selectorComponent;
		Component title;
		Component help;

		settingPanel = new JPanel();
		settingPanel.setBorder(BOTTOM_SPACER_BORDER);
		settingPanel.setLayout(new BorderLayout(5, 5));
		settingPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		settingPanel.setAlignmentY(Component.TOP_ALIGNMENT);
		
		if(settingObject instanceof BooleanSetting) {
			BooleanSetting setting;

			setting = (BooleanSetting)settingObject;
			title = createLabel(setting.getTitle(), TITLE_FONT);
			help = createLabel(setting.getHelp(), DESCRIPTION_FONT);
			selectorComponent = createSelectorComponent(setting);
		}
		else if(settingObject instanceof ChoiceSetting) {
			ChoiceSetting setting;
			
			setting = (ChoiceSetting)settingObject;
			title = createLabel(setting.getTitle(), TITLE_FONT);
			help = createLabel(setting.getHelp(), DESCRIPTION_FONT);
			selectorComponent = createSettingComponent(setting);
		}
		else {
			throw new Error(String.format("Unexpected class %s of setting object.", settingObject.getClass().getCanonicalName()));
		}
		
		settingPanel.add(title, BorderLayout.PAGE_START);
		settingPanel.add(help, BorderLayout.CENTER);
		settingPanel.add(selectorComponent, BorderLayout.LINE_END);
		
		return settingPanel;
	}

	private JComponent createSelectorComponent(BooleanSetting b) {
		JPanel selectorPanel;
		ButtonGroup buttons;
		boolean isEnabled;
	
		buttons = new ButtonGroup();
		selectorPanel = new JPanel();
		selectorPanel.setLayout(new BoxLayout(selectorPanel, BoxLayout.Y_AXIS));
		selectorPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		isEnabled = b != null && b.isDefault();
		addRadioButton(MessageHelper.get(this, "trueButtonText", language), buttons, selectorPanel, isEnabled);
		addRadioButton(MessageHelper.get(this, "falseButtonText", language), buttons, selectorPanel, !isEnabled);
	
		return selectorPanel;
	}

	private JComponent createSettingComponent(final ChoiceSetting c) {
		Option defaultOption;
		List<Option> options;
		final int listOptionCount;
		
		defaultOption = (Option)c.getDefault();
		options = c.getOption();
		listOptionCount = 3;
		
		if(options.size() <= listOptionCount)
			return createOptionRadioButtons(options, defaultOption.getId());
		else
			return createOptionList(options, defaultOption.getId(), listOptionCount);
	}
	
	private JComponent createOptionRadioButtons(final List<Option> options, final String defaultOptionID) {
		ButtonGroup buttons;
		JPanel selectorPanel;

		buttons = new ButtonGroup();
		selectorPanel = new JPanel();
		selectorPanel.setLayout(new BoxLayout(selectorPanel, BoxLayout.Y_AXIS));
		selectorPanel.setAlignmentX(Component.LEFT_ALIGNMENT);

		for(Option o: options) {
			boolean isSelected;
			String title;
			String help;
			
			isSelected = o.getId().equals(defaultOptionID);
			title = getLocalizedInfo(o.getTitle(), true);
			help = getLocalizedInfo(o.getHelp(), true);
			addRadioButton(title, buttons, selectorPanel, isSelected).setToolTipText(help);
		}
		
		return selectorPanel;
	}
	
	private JComponent createOptionList(List<Option> options, String defaultOptionID, final int optionListCount) {
		List<String> titles;
		List<String> helps;
		final String[] helpsArray;
		JList list;
		int selectedIndex;
		JScrollPane scrollPane;
		JPanel panel;
		
		titles = new LinkedList<String>();
		helps = new LinkedList<String>();
		selectedIndex = -1;
		
		for(Option o: options) {
			if(o.getId().equals(defaultOptionID))
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
				return helps[locationToIndex(event.getPoint())];
			}
		};

		list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		list.setLayoutOrientation(JList.VERTICAL);
		list.setVisibleRowCount(optionListCount);
		list.setSelectedIndex(selectedIndex);
		list.setBorder(BorderFactory.createEmptyBorder(2, 2, 2, 2));
		scrollPane = new JScrollPane(list, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		panel = new JPanel();
		panel.add(scrollPane);
		
		return panel;
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

	private JLabel createLabel(List<Info> info, Font font) {
		JLabel label;

		label = new JLabel(getLocalizedInfo(info, true));
		label.setFont(font);
		
		return label;
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
