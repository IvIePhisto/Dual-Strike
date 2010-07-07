package dualstrike.configuration;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
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
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JToolBar;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import javax.swing.border.EtchedBorder;

import dualstrike.configuration.action_listeners.ActionListenerHandler;
import dualstrike.configuration.action_listeners.DefaultsActionListener;
import dualstrike.configuration.action_listeners.LoadActionListener;
import dualstrike.configuration.action_listeners.SaveActionListener;
import dualstrike.configuration.definition.BooleanSetting;
import dualstrike.configuration.definition.ChoiceSetting;
import dualstrike.configuration.definition.Configuration;
import dualstrike.configuration.definition.Info;
import dualstrike.configuration.definition.Option;
import dualstrike.configuration.definition.Page;
import dualstrike.configuration.icons.IconHandler;
import dualstrike.configuration.model.ConfigurationModel;

public class ConfigurationEditor {
	private static final URL DEFAULT_CONFIGURATION_DEFINITION_FILE_URL = createFileURL("configuration.xml");
	private static final int FONT_SIZE = 14;
	private static final Font TITLE_FONT = new Font(Font.SANS_SERIF, Font.BOLD, FONT_SIZE); 
	private static final Font DESCRIPTION_FONT = new Font(Font.SANS_SERIF, Font.PLAIN, FONT_SIZE); 
	private static final Border BOTTOM_SPACER_BORDER = new EmptyBorder(0, 0, 10, 0);
	private static final Color WHITE = new Color(255, 255, 255, 150);
	
	private final Configuration configuration;
	private final Locale language;
	private final Locale defaultLanguage;
	private final ConfigurationModel model;
	private final ActionListenerHandler actionListenerHandler;
	private JFrame view;
	private JPanel glassPanel;
	private JLabel statusLabel;
	private StatusClearer statusCleareance;

	private static URL createFileURL(String path) throws Error {
		try {
			return new File(path).toURI().toURL();
		}
		catch(MalformedURLException e) {
			throw new Error(e);
		}
	}
	
	private ConfigurationEditor(final Configuration configuration, final Locale language) {
		String title;
		ImageIcon icon;

		this.configuration = configuration;
		
		if(language == null)
			this.language = new Locale(configuration.getLang());
		else
			this.language = language;
		
		defaultLanguage = new Locale(configuration.getLang());
		model = new ConfigurationModel(configuration);

		title = getLocalizedInfo(configuration.getTitle(), false);
		view = new JFrame(title);
		icon = IconHandler.getIcon("application", null, 64, null);
		view.setIconImage(icon.getImage());
		actionListenerHandler = new ActionListenerHandler();
		createGlassPanel();
		registerActionHandlers();
		populateView();
		statusCleareance = new StatusClearer(this);
	}
	
	private void registerActionHandlers() {
		actionListenerHandler.registerActionListener("save", new SaveActionListener(this));
		actionListenerHandler.registerActionListener("load", new LoadActionListener(this));
		actionListenerHandler.registerActionListener("defaults", new DefaultsActionListener(this));
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
	
	public static String convertTextToHTML(String value) {
		return "<html>" + value.replace("\n", "<br/>") + "</html>";
	}
	
	private void populateView() {
		createMenuBar();
		createContentPane();
		view.pack();
		view.setLocationRelativeTo(null);
		view.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		view.setVisible(true);
	}
	
	private void createGlassPanel() {
		final JPanel glass;
		
		glass = (JPanel)view.getGlassPane();
		glass.setLayout(new BorderLayout());
		glassPanel = new JPanel() {
			private static final long serialVersionUID = 1L;
			@Override
		    public Dimension getPreferredSize() {
		        return view.getSize();
		    }
			@Override
		    public Dimension getMinimumSize() {
		        return view.getSize();
		    }
			@Override
			protected void paintComponent(Graphics g) {
			    super.paintComponent(g);
			}
		};		
		glassPanel.setLayout(new BorderLayout());
		glassPanel.setBackground(WHITE);
		glass.add(glassPanel, BorderLayout.CENTER);
	}
	
	public void makeViewInactive() {
		view.setEnabled(false);
		glassPanel.setVisible(true);
		//view.repaint();
		view.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
		view.getGlassPane().setVisible(true);
	}
	
	public void makeViewActive() {
		view.setCursor(null);
		glassPanel.setVisible(false);
		//view.repaint();
		view.setEnabled(true);
		view.requestFocus();
	}

	private void createMenuBar() {
		JMenuBar menuBar;
		JMenu menu;
		JMenuItem menuItem;

		menuBar = new JMenuBar();
		menu = new JMenu(MessageHelper.get(this, "deviceMenuName"));
		menuItem = new JMenuItem(MessageHelper.get(this, "loadMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "loadHelp"));
		menuItem.setIcon(IconHandler.getIcon("load", null, 16, null));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "load");
		menuItem = new JMenuItem(MessageHelper.get(this, "saveMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "saveHelp"));
		menuItem.setIcon(IconHandler.getIcon("save", null, 16, null));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "save");
		//menu = new JMenu(MessageHelper.get(this, "defaultsMenuItemName"));
		menuItem = new JMenuItem(MessageHelper.get(this, "defaultsMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "defaultsHelp"));
		menuItem.setIcon(IconHandler.getIcon("defaults", null, 16, null));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "defaults");
		menuBar.add(menu);
		
		view.setJMenuBar(menuBar);
	}
	
	private void createContentPane() {
		JPanel contentPanel;
		JComponent buttons;
		JComponent tabs;
		JComponent statusPanel;
		
		buttons = createToolBar();
		tabs = createTabs();
		statusPanel = createStatusPanel();

		contentPanel = (JPanel)view.getContentPane();
		contentPanel.add(buttons, BorderLayout.PAGE_START);
		contentPanel.add(tabs, BorderLayout.CENTER);
		contentPanel.add(statusPanel, BorderLayout.PAGE_END);
	}
	
	private JComponent createToolBar() {
		JToolBar toolBar;
		JButton button;
		
		toolBar = new JToolBar();
		//toolBar.setLayout(new BoxLayout(toolBar, BoxLayout.X_AXIS));
		
		button = new JButton();
		button.setToolTipText(MessageHelper.get(this, "loadHelp"));
		button.setIcon(IconHandler.getIcon("load", MessageHelper.get(this, "loadButtonTitle"), 22, null));
		toolBar.add(button);
		actionListenerHandler.registerAction(button, "load");
		
		button = new JButton();
		button.setToolTipText(MessageHelper.get(this, "saveHelp"));
		button.setIcon(IconHandler.getIcon("save", MessageHelper.get(this, "saveButtonTitle"), 22, null));
		toolBar.add(button);
		actionListenerHandler.registerAction(button, "save");
		
		button = new JButton();
		button.setToolTipText(MessageHelper.get(this, "defaultsHelp"));
		button.setIcon(IconHandler.getIcon("defaults", MessageHelper.get(this, "defaultsButtonTitle"), 22, null));
		toolBar.add(button);
		actionListenerHandler.registerAction(button, "defaults");
				
		return toolBar;
	}

	private JComponent createStatusPanel() {
		JPanel statusPanel;
		
		statusPanel = new JPanel();
		statusPanel.setLayout(new BorderLayout());
		statusPanel.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
		statusLabel = new JLabel(" ", JLabel.LEFT);
		statusLabel.setBorder(BorderFactory.createEmptyBorder(1, 4, 1, 4));
		statusLabel.setFont(DESCRIPTION_FONT);
		statusPanel.add(statusLabel);
		
		return statusLabel;
	}
	
	private JComponent createTabs() {
		JTabbedPane tabs;
		
		tabs = new JTabbedPane(JTabbedPane.TOP, JTabbedPane.SCROLL_TAB_LAYOUT);
		//helpLabel.setBorder(BorderFactory.createEmptyBorder(5, 5, 0, 5));
		tabs.addTab(MessageHelper.get(this, "overviewTabTitle"), createOverviewTabContent());
		
		for(Page page: configuration.getPage()) {
			String tabTitle;
			
			tabTitle = getLocalizedInfo(page.getTitle(), true);
			tabs.addTab(tabTitle, createTabContent(page));
		}
		
		return tabs;
	}
	
	private Component createOverviewTabContent() {
		JLabel helpLabel;
		JPanel tabPanel;
		JScrollPane tabContent;

		helpLabel = createLabel(configuration.getHelp(), DESCRIPTION_FONT);
		tabPanel = new ResizingJPanel();
		tabPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		tabPanel.setLayout(new BoxLayout(tabPanel, BoxLayout.Y_AXIS));
		tabPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		tabPanel.setAlignmentY(Component.TOP_ALIGNMENT);
		tabPanel.add(helpLabel);
		tabContent = new JScrollPane(tabPanel, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		
		return tabContent;
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
		JRadioButton disableButton;
		boolean isEnabled;
	
		buttons = new ButtonGroup();
		selectorPanel = new JPanel();
		selectorPanel.setLayout(new BoxLayout(selectorPanel, BoxLayout.Y_AXIS));
		selectorPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		isEnabled = b != null && b.isDefault();
		enableButton = addRadioButton(MessageHelper.get(this, "trueButtonText", language), buttons, selectorPanel, isEnabled);
		disableButton = addRadioButton(MessageHelper.get(this, "falseButtonText", language), buttons, selectorPanel, !isEnabled);
		model.addBoolean(b, enableButton, disableButton);
		
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

			ConfigurationEditor.newInstance(configurationURL, language);
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

	public ConfigurationModel getModel() {
		return model;
	}

	public JFrame getView() {
		return view;
	}

	JLabel getStatusLabel() {
		return statusLabel;
	}
	
	public synchronized void setStatusLabelText(final String text) {
		statusLabel.setText(text);
		statusCleareance.setActive();
	}
}
