package dualstrike.configuration;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
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
import javax.swing.JButton;
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
import javax.swing.SpringLayout;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;

import dualstrike.configuration.definition.BooleanSetting;
import dualstrike.configuration.definition.ChoiceSetting;
import dualstrike.configuration.definition.Configuration;
import dualstrike.configuration.definition.Info;
import dualstrike.configuration.definition.Option;
import dualstrike.configuration.definition.Page;
import dualstrike.configuration.model.ConfigurationModel;

public class ConfigurationEditor {
	private static final URL DEFAULT_CONFIGURATION_DEFINITION_FILE_URL = createFileURL("configuration.xml");
	private static final int FONT_SIZE = 12;
	private static final Font TITLE_FONT = new Font(Font.SANS_SERIF, Font.BOLD, FONT_SIZE); 
	private static final Font DESCRIPTION_FONT = new Font(Font.SANS_SERIF, Font.PLAIN, FONT_SIZE); 
	private static final Border BOTTOM_SPACER_BORDER = new EmptyBorder(0, 0, 10, 0);

	private final Configuration configuration;
	private final Locale language;
	private final Locale defaultLanguage;
	private final ConfigurationModel model;
	private JFrame view;
	private JPanel glassPanel;

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
		model = new ConfigurationModel(configuration);
	}
	
	private void createGlassPanel() {
		final JPanel glass;
		final Color color;
		
		glass = (JPanel)view.getGlassPane();
		glass.setLayout(new BorderLayout());
		color = new Color(255, 255, 255, 125);
		glassPanel = new JPanel() {
			private static final long serialVersionUID = 1L;
			@Override
		    public Dimension getPreferredSize() {
		        return view.getSize();
		    }
			
			protected void paintComponent(Graphics g) {
		        super.paintComponent(g);
		    }
		};
		
		glassPanel.setBackground(color);
		glass.add(glassPanel);
	}
	
	private static ConfigurationEditor newInstance(URL configurationDefinitionURL, final Locale language) throws IOException, ConfigurationDefinitionException {
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
			value = convertTextToHTML(value);
		
		return value;
	}
	
	static String convertTextToHTML(String value) {
		return "<html>" + value.replace("\n", "<br/>") + "</html>";
	}
	
	private void createView() {
		String title;
		JPanel contentPanel;
		JComponent buttons;
		JComponent tabs;
		SpringLayout layout;
		
		title = getLocalizedInfo(configuration.getTitle(), false);
		view = new JFrame(title);
		buttons = createButtons();
		tabs = createTabs();
		contentPanel = new JPanel();
		contentPanel.add(buttons);
		contentPanel.add(tabs);
		//contentPanel.setLayout(new BorderLayout());
		/*
		contentPanel.setLayout(new BoxLayout(contentPanel, BoxLayout.Y_AXIS));
		contentPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		contentPanel.setAlignmentY(Component.TOP_ALIGNMENT);
		*/
		/*
		layout = new GroupLayout(contentPanel);
		layout.setVerticalGroup(
				layout.createSequentialGroup()
				.addComponent(buttons, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
				.addComponent(tabs, 0, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE));
		layout.setHorizontalGroup(
				layout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addComponent(buttons, 0, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
				.addComponent(tabs));
		*/
		layout = new SpringLayout();
		contentPanel.setLayout(layout);
		layout.putConstraint(SpringLayout.NORTH, buttons, 0, SpringLayout.NORTH, contentPanel);
		layout.putConstraint(SpringLayout.EAST, contentPanel, 0, SpringLayout.EAST, buttons);
		layout.putConstraint(SpringLayout.EAST, contentPanel, 0, SpringLayout.EAST, tabs);
		layout.putConstraint(SpringLayout.WEST, contentPanel, 0, SpringLayout.WEST, buttons);
		layout.putConstraint(SpringLayout.WEST, contentPanel, 0, SpringLayout.WEST, tabs);
		layout.putConstraint(SpringLayout.SOUTH, contentPanel, 0, SpringLayout.SOUTH, tabs);
		//layout.putConstraint(SpringLayout.EAST, tabs, 0, SpringLayout.EAST, buttons);
		layout.putConstraint(SpringLayout.WEST, tabs, 0, SpringLayout.WEST, buttons);
		layout.putConstraint(SpringLayout.NORTH, tabs, 0, SpringLayout.SOUTH, buttons);
		//layout.putConstraint(SpringLayout.SOUTH, buttons, 0, SpringLayout.NORTH, tabs);
		
		view.getContentPane().add(contentPanel);
		view.pack();
		createGlassPanel();
		view.setLocationRelativeTo(null);
		view.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		view.setVisible(true);
	}
	
	private JComponent createButtons() {
		JPanel buttonsPanel;
		JButton loadButton;
		JButton saveButton;
		JButton defaultsButton;
		
		buttonsPanel = new JPanel();
		loadButton = new JButton(MessageHelper.get(this, "loadButtonTitle"));
		loadButton.setToolTipText(MessageHelper.get(this, "loadButtonHelp"));
		buttonsPanel.add(loadButton);
		new LoadExecHandler(view, model, loadButton);		
		saveButton = new JButton(MessageHelper.get(this, "saveButtonTitle"));
		saveButton.setToolTipText(MessageHelper.get(this, "saveButtonHelp"));
		buttonsPanel.add(saveButton);
		defaultsButton = new JButton(MessageHelper.get(this, "defaultsButtonTitle"));
		defaultsButton.setToolTipText(MessageHelper.get(this, "defaultsButtonHelp"));
		buttonsPanel.add(defaultsButton);
		new LoadDefaultsHandler(view, model, defaultsButton);
		
		return buttonsPanel;
	}
	
	private JComponent createTabs() {
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
			
			settingComponent = createSettingComponent(setting);
			tabPanel.add(settingComponent);
		}
		
		tabContent = new JScrollPane(tabPanel, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		
		return tabContent;
	}

	private JComponent createSettingComponent(Object settingObject) {
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
			selectorComponent = createSettingComponent(setting);
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

	private JComponent createSettingComponent(BooleanSetting b) {
		JPanel selectorPanel;
		ButtonGroup buttons;
		JRadioButton enableButton;
		boolean isEnabled;
	
		buttons = new ButtonGroup();
		selectorPanel = new JPanel();
		selectorPanel.setLayout(new BoxLayout(selectorPanel, BoxLayout.Y_AXIS));
		selectorPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		isEnabled = b != null && b.isDefault();
		enableButton = addRadioButton(MessageHelper.get(this, "trueButtonText", language), buttons, selectorPanel, isEnabled);
		addRadioButton(MessageHelper.get(this, "falseButtonText", language), buttons, selectorPanel, !isEnabled);
		model.addBoolean(b, enableButton);
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
			return createOptionRadioButtons(c, defaultOption.getId());
		else
			return createOptionList(c, defaultOption.getId(), listOptionCount);
	}
	
	private JComponent createOptionRadioButtons(final ChoiceSetting choiceSetting, final String defaultOptionID) {
		ButtonGroup buttons;
		JPanel selectorPanel;

		buttons = new ButtonGroup();
		selectorPanel = new JPanel();
		selectorPanel.setLayout(new BoxLayout(selectorPanel, BoxLayout.Y_AXIS));
		selectorPanel.setAlignmentX(Component.LEFT_ALIGNMENT);

		for(Option o: choiceSetting.getOption()) {
			boolean isSelected;
			String title;
			String help;
			
			isSelected = o.getId().equals(defaultOptionID);
			title = getLocalizedInfo(o.getTitle(), true);
			help = getLocalizedInfo(o.getHelp(), true);
			addRadioButton(title, buttons, selectorPanel, isSelected).setToolTipText(help);
		}
		
		model.addChoice(choiceSetting, buttons);
		
		return selectorPanel;
	}
	
	private JComponent createOptionList(final ChoiceSetting choiceSetting, String defaultOptionID, final int optionListCount) {
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
		
		for(Option o: choiceSetting.getOption()) {
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
		model.addChoice(choiceSetting, list);

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
			ConfigurationEditor ce;
			
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
			ce.createView();
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
