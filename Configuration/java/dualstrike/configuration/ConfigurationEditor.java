package dualstrike.configuration;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Event;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.KeyEvent;
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
import javax.swing.KeyStroke;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;

import dualstrike.configuration.action_listeners.AboutMessageActionListener;
import dualstrike.configuration.action_listeners.ActionListenerHandler;
import dualstrike.configuration.action_listeners.DefaultsActionListener;
import dualstrike.configuration.action_listeners.ExitActionListener;
import dualstrike.configuration.action_listeners.LoadActionListener;
import dualstrike.configuration.action_listeners.SaveActionListener;
import dualstrike.configuration.definition.BooleanSetting;
import dualstrike.configuration.definition.ChoiceSetting;
import dualstrike.configuration.definition.Configuration;
import dualstrike.configuration.definition.DescriptionImage;
import dualstrike.configuration.definition.Info;
import dualstrike.configuration.definition.Option;
import dualstrike.configuration.definition.Page;
import dualstrike.configuration.file.FileHandler;
import dualstrike.configuration.icons.IconHandler;
import dualstrike.configuration.model.ConfigurationModel;

public class ConfigurationEditor {
	private static final URL DEFAULT_CONFIGURATION_DEFINITION_FILE_URL = createFileURL("configuration.xml");
	private static final int FONT_SIZE = 14;
	private static final Font TITLE_FONT = new Font(Font.SANS_SERIF, Font.BOLD, FONT_SIZE); 
	private static final Font DESCRIPTION_FONT = new Font(Font.SANS_SERIF, Font.PLAIN, FONT_SIZE); 
	private static final Border BOTTOM_SPACER_BORDER = new EmptyBorder(0, 0, 10, 0);
	private static final Color WHITE = new Color(255, 255, 255, 150);
	
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

		try {
			ConfigurationEditor.newInstance(configurationURL, language);
		}
		catch(ConfigurationDefinitionException e) {
			showExitErrorDialog(e.getLocalizedMessage(), language);
		}
		catch(FileNotFoundException e) {
			showExitErrorDialog(MessageHelper.get(ConfigurationEditor.class, "configurationDefinitionNotFound", language, configurationURL), language);
		}
		catch(IOException e) {
			showExitErrorDialog(MessageHelper.get(ConfigurationEditor.class, "configurationDefinitionLoadingError", language, e.getLocalizedMessage()), language);
		}
	}

	private static void showExitErrorDialog(String message, Locale language) {
		JFrame frame;
		String[] options;
		
		options = new String[]{"OK"};
		frame = new JFrame();
		JOptionPane.showOptionDialog(frame, message, MessageHelper.get(ConfigurationEditor.class, "errorMessageTitle", language), JOptionPane.OK_OPTION , JOptionPane.ERROR_MESSAGE, null, options, options[0]);
		System.exit(1);
	}

	public static String convertTextToHTML(String value) {
		return "<html>" + value.replace("\n", "<br/>") + "</html>";
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

	private static URL createFileURL(String path) throws Error {
		try {
			return new File(path).toURI().toURL();
		}
		catch(MalformedURLException e) {
			throw new Error(e);
		}
	}
	
	private static ImageIcon createFileImageIcon(final String path) {
		try {
			ImageIcon imageIcon;
			File file;
			URL url;
			
			file = new File(path);
			
			if(!file.exists())
				throw new Error(String.format("Image \"%s\" not found.", path));
	
			url = file.toURI().toURL();
			imageIcon = new ImageIcon(url);
			
			return imageIcon;
		}
		catch(MalformedURLException e) {
			throw new Error(e);
		}
	}

	private static Image createApplicationImage(final String path) {
		Image image;
		
		if(path == null || path.equals(""))
			image = IconHandler.getIcon("application", null, 64, null).getImage();
		else {
			image = createFileImageIcon(path).getImage();
		}
		
		return image;
	}

	private final Configuration configuration;
	private final Locale language;
	private final Locale defaultLanguage;
	private final ConfigurationModel model;
	private final ActionListenerHandler actionListenerHandler;
	private final String windowTitle;
	private final FileHandler fileHandler;
	private final JFrame window;
	private JPanel glassPanel;
	private JLabel statusLabel;
	private final StatusClearer statusClearer;
	private ConnectionChecker connectionChecker;

	private ConfigurationEditor(final Configuration configuration, final Locale language) {
		this.configuration = configuration;
		
		if(language == null)
			this.language = new Locale(configuration.getLang());
		else
			this.language = language;
		
		defaultLanguage = new Locale(configuration.getLang());
		fileHandler = new FileHandler(this);
		model = new ConfigurationModel(fileHandler, configuration);
		windowTitle = getLocalizedInfo(configuration.getTitle(), false);
		window = new JFrame(windowTitle);
		window.setIconImage(createApplicationImage(configuration.getIconPath()));
		actionListenerHandler = new ActionListenerHandler();
		createGlassPanel();
		registerActionHandlers();
		populateWindow();
		statusClearer = new StatusClearer(this);
	}
	
	private void registerActionHandlers() {
		actionListenerHandler.registerActionListener("loadFile", fileHandler.createLoadFileActionListener());
		actionListenerHandler.registerActionListener("saveFile", fileHandler.createSaveFileActionListener());
		actionListenerHandler.registerActionListener("saveAsFile", fileHandler.createSaveAsFileActionListener());
		actionListenerHandler.registerActionListener("forgetFile", fileHandler.createForgetFileActionListener());
		actionListenerHandler.registerActionListener("exit", new ExitActionListener(this));
		actionListenerHandler.registerActionListener("save", new SaveActionListener(this));
		actionListenerHandler.registerActionListener("load", new LoadActionListener(this));
		actionListenerHandler.registerActionListener("defaults", new DefaultsActionListener(this));
		actionListenerHandler.registerActionListener("about", new AboutMessageActionListener(this));
	}

	private void populateWindow() {
		createMenuBar();
		createContentPane();
		window.pack();
		window.setLocationRelativeTo(null);
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		window.setVisible(true);
	}

	private void createGlassPanel() {
		final JPanel glass;
		
		glass = (JPanel)window.getGlassPane();
		glass.setLayout(new BorderLayout());
		glassPanel = new JPanel() {
			private static final long serialVersionUID = 1L;
			@Override
		    public Dimension getPreferredSize() {
		        return window.getSize();
		    }
			@Override
		    public Dimension getMinimumSize() {
		        return window.getSize();
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
	
	private void createMenuBar() {
		JMenuBar menuBar;
		JMenu menu;
		JMenuItem menuItem;

		menuBar = new JMenuBar();
		menu = new JMenu(MessageHelper.get(this, "fileMenuName"));
		
		menuItem = new JMenuItem(MessageHelper.get(this, "loadFileMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "loadFileHelp"));
		menuItem.setIcon(IconHandler.getIcon("loadFile", null, 16, null));
		menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O,  Event.CTRL_MASK));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "loadFile");

		menuItem = new JMenuItem(MessageHelper.get(this, "saveFileMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "saveFileHelp"));
		menuItem.setIcon(IconHandler.getIcon("saveFile", null, 16, null));
		menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S,  Event.CTRL_MASK));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "saveFile");

		menuItem = new JMenuItem(MessageHelper.get(this, "saveAsFileMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "saveAsFileHelp"));
		menuItem.setIcon(IconHandler.getIcon("saveAsFile", null, 16, null));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "saveAsFile");

		menuItem = new JMenuItem(MessageHelper.get(this, "forgetFileMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "forgetFileHelp"));
		menuItem.setIcon(IconHandler.getIcon("forgetFile", null, 16, null));
		menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F,  Event.CTRL_MASK));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "forgetFile");
		
		menu.addSeparator();
		menuItem = new JMenuItem(MessageHelper.get(this, "exitMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "exitHelp"));
		menuItem.setIcon(IconHandler.getIcon("exit", null, 16, null));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "exit");

		menuBar.add(menu);
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

		menu.addSeparator();
		menuItem = new JMenuItem(MessageHelper.get(this, "defaultsMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "defaultsHelp"));
		menuItem.setIcon(IconHandler.getIcon("defaults", null, 16, null));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "defaults");

		menuBar.add(menu);
		menu = new JMenu(MessageHelper.get(this, "helpMenuName"));
		
		menuItem = new JMenuItem(MessageHelper.get(this, "aboutMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "aboutHelp"));
		menuItem.setIcon(IconHandler.getIcon("about", null, 16, null));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "about");
		menuBar.add(menu);

		window.setJMenuBar(menuBar);
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

	private void createContentPane() {
		JPanel contentPanel;
		JComponent buttons;
		JComponent tabs;
		JComponent statusPanel;
		
		buttons = createToolBar();
		tabs = createTabs();
		statusPanel = createStatusPanel();

		contentPanel = (JPanel)window.getContentPane();
		contentPanel.add(buttons, BorderLayout.PAGE_START);
		contentPanel.add(tabs, BorderLayout.CENTER);
		contentPanel.add(statusPanel, BorderLayout.PAGE_END);
	}
		
	private JComponent createStatusPanel() {
		JPanel statusPanel;
		JLabel connectedLabel;
		JLabel disconnectedLabel;
		JPanel connectionStatusPanel;
		String connectedMessage;
		String disconnectedMessage;
		
		statusPanel = new JPanel();
		statusPanel.setLayout(new BorderLayout());
		connectedMessage = MessageHelper.get(this, "connected");
		connectedLabel = new JLabel(IconHandler.getIcon("connected", connectedMessage, 16, null));
		connectedLabel.setVisible(false);
		connectedLabel.setToolTipText(connectedMessage);
		disconnectedMessage = MessageHelper.get(this, "disconnected");
		disconnectedLabel = new JLabel(IconHandler.getIcon("disconnected", disconnectedMessage, 16, null));
		disconnectedLabel.setToolTipText(disconnectedMessage);
		connectionStatusPanel = new JPanel();
		connectionStatusPanel.add(connectedLabel);
		connectionStatusPanel.add(disconnectedLabel);
		connectionChecker = new ConnectionChecker(connectedLabel, disconnectedLabel);
		statusLabel = new JLabel(" ", JLabel.LEFT);
		statusLabel.setBorder(BorderFactory.createEmptyBorder(1, 4, 1, 4));
		statusLabel.setFont(DESCRIPTION_FONT);
		statusPanel.add(statusLabel, BorderLayout.CENTER);
		statusPanel.add(connectionStatusPanel, BorderLayout.LINE_END);
		
		return statusPanel;
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
		JLabel imageLabel;

		tabPanel = new ResizingJPanel();
		tabPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		tabPanel.setLayout(new BoxLayout(tabPanel, BoxLayout.X_AXIS));
		tabPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		tabPanel.setAlignmentY(Component.TOP_ALIGNMENT);
		
		helpLabel = createLabel(configuration.getHelp(), DESCRIPTION_FONT);
		tabPanel.add(helpLabel);	

		imageLabel = createImageLabel(configuration.getImage());
		
		if(imageLabel != null) {
			imageLabel.setAlignmentX(Component.RIGHT_ALIGNMENT);
			imageLabel.setAlignmentY(Component.TOP_ALIGNMENT);
			imageLabel.setBorder(BorderFactory.createEmptyBorder(0, 10, 0, 0));
			tabPanel.add(imageLabel);
		}
		
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
		JLabel title;
		JLabel help;
		JPanel helpPanel;
		JLabel imageLabel;

		settingPanel = new JPanel();
		settingPanel.setBorder(BOTTOM_SPACER_BORDER);
		settingPanel.setLayout(new BorderLayout(10, 5));
		settingPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		settingPanel.setAlignmentY(Component.TOP_ALIGNMENT);
		
		if(settingObject instanceof BooleanSetting) {
			BooleanSetting setting;

			setting = (BooleanSetting)settingObject;
			title = createLabel(setting.getTitle(), TITLE_FONT);
			help = createLabel(setting.getHelp(), DESCRIPTION_FONT);
			selectorComponent = createSettingComponent(setting);
			imageLabel = createImageLabel(setting.getImage());
		}
		else if(settingObject instanceof ChoiceSetting) {
			ChoiceSetting setting;
			
			setting = (ChoiceSetting)settingObject;
			title = createLabel(setting.getTitle(), TITLE_FONT);
			help = createLabel(setting.getHelp(), DESCRIPTION_FONT);
			selectorComponent = createSettingComponent(setting);
			imageLabel = createImageLabel(setting.getImage());
		}
		else {
			throw new Error(String.format("Unexpected class %s of setting object.", settingObject.getClass().getCanonicalName()));
		}
		
		settingPanel.add(title, BorderLayout.PAGE_START);
		helpPanel = new JPanel();
		helpPanel.setLayout(new BoxLayout(helpPanel, BoxLayout.Y_AXIS));
		helpPanel.add(help);
		settingPanel.add(helpPanel, BorderLayout.CENTER);
		settingPanel.add(selectorComponent, BorderLayout.LINE_END);
		
		if(imageLabel != null)
			settingPanel.add(imageLabel, BorderLayout.LINE_START);
		
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

	private JLabel createImageLabel(final DescriptionImage descriptionImage) {
		JLabel label;
		ImageIcon imageIcon;
		
		if(descriptionImage == null)
			return null;
		
		imageIcon = createFileImageIcon(descriptionImage.getPath());
		imageIcon.setDescription(getLocalizedInfo(descriptionImage.getTitle(), true));
		label = new JLabel(imageIcon);
		label.setToolTipText(getLocalizedInfo(descriptionImage.getHelp(), true));
		
		return label;
	}

	private JLabel createLabel(List<Info> info, Font font) {
		JLabel label;
	
		label = new JLabel(getLocalizedInfo(info, true), JLabel.LEFT);
		label.setFont(font);
		
		return label;
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

	public ConfigurationModel getModel() {
		return model;
	}

	public JFrame getWindow() {
		return window;
	}

	public FileHandler getFileHandler() {
		return fileHandler;
	}

	public void makeWindowInactive() {
		window.setEnabled(false);
		window.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
		window.getGlassPane().setVisible(true);
	}

	public void makeWindowActive() {
		window.getGlassPane().setVisible(false);
		window.setCursor(null);
		window.setEnabled(true);
		window.requestFocus();
	}

	public ConnectionChecker getConnectionChecker() {
		return connectionChecker;
	}

	JLabel getStatusLabel() {
		return statusLabel;
	}
	
	public synchronized void setStatusLabelText(final String text) {
		statusLabel.setText(text);
		statusClearer.setActive();
	}

	public synchronized void setWindowTitleAmendment(final String text) {
		if(text == null)
			window.setTitle(windowTitle);
		else
			window.setTitle(windowTitle + ": " + text);
	}

	public void showErrorDialog(String title, String message) {
		String[] options;
		
		options = new String[]{"OK"};
		JOptionPane.showOptionDialog(window, message, title, JOptionPane.OK_OPTION , JOptionPane.ERROR_MESSAGE, null, options, options[0]);
	}

	public synchronized void exit() {
		System.exit(0);
	}
}
