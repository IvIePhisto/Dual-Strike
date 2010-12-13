package mccf;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Desktop;
import java.awt.Dimension;
import java.awt.Event;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.GraphicsEnvironment;
import java.awt.Image;
import java.awt.Insets;
import java.awt.Rectangle;
import java.awt.event.KeyEvent;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JLabel;
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
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;

import mccf.action_listeners.AboutMessageActionListener;
import mccf.action_listeners.ActionListenerHandler;
import mccf.action_listeners.DefaultsActionListener;
import mccf.action_listeners.ExitActionListener;
import mccf.action_listeners.HelpActionListener;
import mccf.action_listeners.LoadActionListener;
import mccf.action_listeners.SaveActionListener;
import mccf.action_listeners.TabDefaultsActionListener;
import mccf.definition.BooleanSetting;
import mccf.definition.ChoiceSetting;
import mccf.definition.Configuration;
import mccf.definition.DescriptionImage;
import mccf.definition.Info;
import mccf.definition.Option;
import mccf.definition.Page;
import mccf.definition.PathInfo;
import mccf.definition.ValueDomain;
import mccf.definition.ValueSetting;
import mccf.file.FileHandler;
import mccf.icons.IconHandler;
import mccf.model.ConfigurationModel;
import mccf.model.PageModel;
import mccf.model.ValueDomainModel;


public class ConfigurationEditor implements HyperlinkListener {
	public static final int MAJOR_VERSION_NO = 1;
	public static final int MINOR_VERSION_NO = 2;
	public static final int BUGFIX_VERSION_NO = 0;

	static final File DEFAULT_CONFIGURATION_DEFINITION_FILE = new File("configuration.xml");

	private static final Desktop DESKTOP = Desktop.isDesktopSupported()?Desktop.getDesktop():null;
	private static final int FONT_SIZE = 14;
	private static final Font TITLE_FONT = new Font(Font.SANS_SERIF, Font.BOLD, FONT_SIZE); 
	private static final Border BOTTOM_SPACER_BORDER = new EmptyBorder(0, 0, 10, 0);
	private static final Color MILK_GLASS = new Color(255, 255, 255, 150);
	/*
	private static final String MACOSX_USE_SCREEN_MENU_BAR_PROPERTY_NAME = "apple.laf.useScreenMenuBar";
	private static final String MACOSX_BRUSH_METAL_LOOK_PROPERTY_NAME = "apple.awt.brushMetalLook";
	*/
	
	public static final Font DESCRIPTION_FONT = new Font(Font.SANS_SERIF, Font.PLAIN, FONT_SIZE); 
	

	static ConfigurationEditor newInstance(File configurationDefinitionFile, final Locale language) throws IOException, ConfigurationDefException {
		ConfigurationEditor ce;
		
		/*
		System.setProperty(MACOSX_USE_SCREEN_MENU_BAR_PROPERTY_NAME, "true");
		System.setProperty(MACOSX_BRUSH_METAL_LOOK_PROPERTY_NAME, "true");
		*/

		if(configurationDefinitionFile == null)
			configurationDefinitionFile = DEFAULT_CONFIGURATION_DEFINITION_FILE;
		
		ce = new ConfigurationEditor(configurationDefinitionFile, language);

		return ce;
	}

	static URL createFileURL(String path) throws Error {
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

	private static List<Image> createApplicationImages(final String path) {
		List<Image> images;
		Image currentImage;
		
		images = new LinkedList<Image>();
		
		if(path == null || path.equals("")) {
			currentImage = IconHandler.getIcon("application", null, 64, null).getImage();
			images.add(currentImage);
			currentImage = IconHandler.getIcon("application", null, 22, null).getImage();
			images.add(currentImage);
			currentImage = IconHandler.getIcon("application", null, 16, null).getImage();
			images.add(currentImage);
		}
		else {
			int mainImageFileLastDot;
			int nextImageNumber;
			File currentImageFile;
			File imageFilesDir;
			String mainImageFileName;
			String imageFilesPrefix;
			String imageFilesPostfix;
			
			currentImageFile = new File(path);
			imageFilesDir = currentImageFile.getParentFile();
			mainImageFileName = currentImageFile.getName();
			mainImageFileLastDot = mainImageFileName.lastIndexOf('.');
			imageFilesPrefix = mainImageFileName.substring(0, mainImageFileLastDot);
			imageFilesPostfix = mainImageFileName.substring(mainImageFileLastDot);
			nextImageNumber = 1;
			
			do {
				currentImage = createFileImageIcon(currentImageFile.getAbsolutePath()).getImage();
				images.add(currentImage);
				currentImageFile = new File(imageFilesDir, imageFilesPrefix + "_" + nextImageNumber++ + imageFilesPostfix);
			} while(currentImageFile.exists());
		}
		
		return images;
	}

	private static void makeDefault(final JComponent defaultSetting) {
		String tooltip;
		String postfix;
		
		defaultSetting.setFont(defaultSetting.getFont().deriveFont(Font.ITALIC));
		tooltip = defaultSetting.getToolTipText();
		postfix = MessageHelper.get(ConfigurationEditor.class, "default");
		
		if(tooltip == null || tooltip.equals(""))
			tooltip = postfix;
		else {
			Matcher matcher;
			
			matcher = Pattern.compile("(.*<html>.*)(</html>.*)").matcher(tooltip);
			
			if(matcher.matches()) {
				tooltip = matcher.group(1) + " " + postfix + matcher.group(2);
			}
			else
				tooltip += " " + postfix;
		}
		
		defaultSetting.setToolTipText(tooltip);
	}

	private final URL configurationDefinitionURL;
	private final URL configurationDefDirectoryURL;
	private final Configuration configuration;
	private final Locale language;
	private final Locale defaultLanguage;
	private final ConfigurationModel model;
	private final ActionListenerHandler actionListenerHandler;
	private final String mainTitle;
	private final FileHandler fileHandler;
	private final JFrame window;
	private final StatusClearer statusClearer;
	private final List<JComponent> connectionDependingComponents = new LinkedList<JComponent>();

	private JPanel glassPanel;
	private JLabel statusLabel;
	private ConnectionChecker connectionChecker;

	private ConfigurationEditor(final File configurationDefinitionFile, final Locale language) throws ConfigurationDefException, IOException {
		configurationDefinitionURL = configurationDefinitionFile.toURI().toURL();
		configurationDefDirectoryURL = configurationDefinitionFile.getAbsoluteFile().getParentFile().toURI().toURL();
		configuration = ConfigurationDefUtility.unmarshallConfigurationDef(configurationDefinitionURL);
		
		if(language == null)
			this.language = new Locale(configuration.getLang());
		else
			this.language = language;
		
		defaultLanguage = new Locale(configuration.getLang());
		fileHandler = new FileHandler(this);
		model = new ConfigurationModel(fileHandler, configuration);
		
		for(ValueDomain valueDomain: configuration.getValueDomain())
			new ValueDomainModel(model, valueDomain);
		
		mainTitle = getLocalizedInfo(configuration.getTitle(), false);
		JFrame.setDefaultLookAndFeelDecorated(true);
		window = new JFrame(mainTitle);
		window.setIconImages(createApplicationImages(configuration.getIconPath()));
		actionListenerHandler = new ActionListenerHandler();
		createGlassPanel();
		registerActionHandlers();
		populateWindow();
		model.initConstraints();
		statusClearer = new StatusClearer(this);
	}
	
	private void registerActionHandlers() {
		actionListenerHandler.registerActionListener("loadFile", fileHandler.createLoadFileActionListener());
		actionListenerHandler.registerActionListener("saveFile", fileHandler.createSaveFileActionListener());
		actionListenerHandler.registerActionListener("saveAsFile", fileHandler.createSaveAsFileActionListener());
		actionListenerHandler.registerActionListener("closeFile", fileHandler.createCloseFileActionListener());
		actionListenerHandler.registerActionListener("exit", new ExitActionListener(this));
		actionListenerHandler.registerActionListener("save", new SaveActionListener(this));
		actionListenerHandler.registerActionListener("load", new LoadActionListener(this));
		actionListenerHandler.registerActionListener("defaults", new DefaultsActionListener(this));
		actionListenerHandler.registerActionListener("tabDefaults", new TabDefaultsActionListener(this));
		actionListenerHandler.registerActionListener("help", new HelpActionListener(this, getLocalizedInfo(configuration.getHelp(), true)));
		actionListenerHandler.registerActionListener("about", new AboutMessageActionListener(this));
	}

	private void populateWindow() {
		GraphicsEnvironment graphicsEnvironment;
		Rectangle maximumWindowBounds;
		int maximumWidth;
		int maximumHeight;
		Dimension windowSize;
		
		createMenuBar();
		createContentPane();
		graphicsEnvironment = GraphicsEnvironment.getLocalGraphicsEnvironment();
		maximumWindowBounds = graphicsEnvironment.getMaximumWindowBounds();
		maximumWidth = maximumWindowBounds.width;
		maximumHeight = maximumWindowBounds.height;
		window.pack();
		windowSize = window.getSize();
		
		if(windowSize.width > maximumWidth)
			windowSize.width = maximumWidth;

		if(windowSize.height > maximumHeight)
			windowSize.height = maximumHeight;
		
		window.setSize(windowSize);
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
		glassPanel.setBackground(MILK_GLASS);
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

		menu.addSeparator();

		menuItem = new JMenuItem(MessageHelper.get(this, "closeFileMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "closeFileHelp"));
		menuItem.setIcon(IconHandler.getIcon("closeFile", null, 16, null));
		menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_W,  Event.CTRL_MASK));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "closeFile");

		menu.addSeparator();
		
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
		connectionDependingComponents.add(menuItem);

		menuItem = new JMenuItem(MessageHelper.get(this, "saveMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "saveHelp"));
		menuItem.setIcon(IconHandler.getIcon("save", null, 16, null));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "save");
		connectionDependingComponents.add(menuItem);

		menu.addSeparator();
		menuItem = new JMenuItem(MessageHelper.get(this, "defaultsMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "defaultsHelp"));
		menuItem.setIcon(IconHandler.getIcon("defaults", null, 16, null));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "defaults");

		menuItem = new JMenuItem(MessageHelper.get(this, "tabDefaultsMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "tabDefaultsHelp"));
		menuItem.setIcon(IconHandler.getIcon("tabDefaults", null, 16, null));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "tabDefaults");

		menuBar.add(menu);
		menu = new JMenu(MessageHelper.get(this, "helpMenuName"));
		
		menuItem = new JMenuItem(MessageHelper.get(this, "helpMenuItemName"));
		menuItem.setToolTipText(MessageHelper.get(this, "helpHelp"));
		menuItem.setIcon(IconHandler.getIcon("help", null, 16, null));
		menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F1,  0));
		menu.add(menuItem);
		actionListenerHandler.registerAction(menuItem, "help");
		menuBar.add(menu);

		menu.addSeparator();
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
		
		button = new JButton();
		button.setToolTipText(MessageHelper.get(this, "loadHelp"));
		button.setIcon(IconHandler.getIcon("load", MessageHelper.get(this, "loadButtonTitle"), 22, null));
		toolBar.add(button);
		actionListenerHandler.registerAction(button, "load");
		connectionDependingComponents.add(button);
		
		button = new JButton();
		button.setToolTipText(MessageHelper.get(this, "saveHelp"));
		button.setIcon(IconHandler.getIcon("save", MessageHelper.get(this, "saveButtonTitle"), 22, null));
		toolBar.add(button);
		actionListenerHandler.registerAction(button, "save");
		connectionDependingComponents.add(button);
		
		toolBar.addSeparator();
		button = new JButton();
		button.setToolTipText(MessageHelper.get(this, "defaultsHelp"));
		button.setIcon(IconHandler.getIcon("defaults", MessageHelper.get(this, "defaultsButtonTitle"), 22, null));
		toolBar.add(button);
		actionListenerHandler.registerAction(button, "defaults");

		button = new JButton();
		button.setToolTipText(MessageHelper.get(this, "tabDefaultsHelp"));
		button.setIcon(IconHandler.getIcon("tabDefaults", MessageHelper.get(this, "tabDefaultsButtonTitle"), 22, null));
		toolBar.add(button);
		actionListenerHandler.registerAction(button, "tabDefaults");

		toolBar.addSeparator();
		button = new JButton();
		button.setToolTipText(MessageHelper.get(this, "helpHelp"));
		button.setIcon(IconHandler.getIcon("help", MessageHelper.get(this, "helpButtonTitle"), 22, null));
		toolBar.add(button);
		actionListenerHandler.registerAction(button, "help");
		
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
		JLabel connectionHelp;
		JPanel disconnectedPanel;
		JLabel disconnectedLabel;
		JPanel connectionStatusPanel;
		String connectedMessage;
		String disconnectedMessage;
		
		connectedMessage = MessageHelper.get(this, "connected");
		connectedLabel = new JLabel(IconHandler.getIcon("connected", connectedMessage, 16, null));
		connectedLabel.setVisible(false);
		connectedLabel.setToolTipText(connectedMessage);

		disconnectedMessage = MessageHelper.get(this, "disconnected");
		disconnectedLabel = new JLabel(IconHandler.getIcon("disconnected", disconnectedMessage, 16, null));
		disconnectedLabel.setToolTipText(disconnectedMessage);
		connectionHelp = new JLabel(getLocalizedInfo(configuration.getConnectionHelp(), true));
		disconnectedPanel = new JPanel();
		disconnectedPanel.add(connectionHelp);
		disconnectedPanel.add(disconnectedLabel);
		
		connectionStatusPanel = new JPanel();
		connectionStatusPanel.add(connectedLabel);
		connectionStatusPanel.add(disconnectedPanel);
		
		connectionChecker = new ConnectionChecker(connectedLabel, disconnectedPanel, connectionDependingComponents);
		statusLabel = new JLabel(" ", JLabel.LEFT);
		statusLabel.setBorder(BorderFactory.createEmptyBorder(1, 4, 1, 4));
		statusLabel.setFont(DESCRIPTION_FONT);
		
		statusPanel = new JPanel();
		statusPanel.setLayout(new BorderLayout());
		statusPanel.add(statusLabel, BorderLayout.CENTER);
		statusPanel.add(connectionStatusPanel, BorderLayout.LINE_END);
		
		return statusPanel;
	}
	
	private JComponent createTabs() {
		JTabbedPane tabs;
		TabDefaultsActionListener tabDefaults;
		
		tabs = new JTabbedPane(JTabbedPane.TOP, JTabbedPane.SCROLL_TAB_LAYOUT);
		tabDefaults = (TabDefaultsActionListener)actionListenerHandler.getActionListener("tabDefaults");
		tabDefaults.setTabs(tabs);
		
		for(Page page: configuration.getPage())
			tabs.addTab(getLocalizedInfo(page.getTitle(), true), createTabContent(page, tabDefaults));
		
		return tabs;
	}
	
	private Component createTabContent(final Page page, final TabDefaultsActionListener tabDefaults) {
		JLabel tabHelpLabel;
		JScrollPane scrollPane;
		JPanel panel;
		PageModel pageModel;
		
		pageModel = model.addPage();
		tabDefaults.addPageModel(pageModel);
		panel = new ResizingJPanel();
		panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
		panel.setAlignmentX(Component.LEFT_ALIGNMENT);
		panel.setAlignmentY(Component.TOP_ALIGNMENT);
		
		tabHelpLabel = createLabel(page.getHelp(), DESCRIPTION_FONT);
		tabHelpLabel.setBorder(BOTTOM_SPACER_BORDER);
		tabHelpLabel.setAlignmentX(Component.LEFT_ALIGNMENT);
		tabHelpLabel.setAlignmentY(Component.TOP_ALIGNMENT);
		panel.add(tabHelpLabel);

		for(Object setting: page.getChoiceOrBooleanOrValue()) {
			JComponent settingComponent;
			
			settingComponent = createSettingComponent(setting, pageModel);
			settingComponent.setAlignmentX(Component.LEFT_ALIGNMENT);
			settingComponent.setAlignmentY(Component.TOP_ALIGNMENT);
			panel.add(settingComponent);
		}

		scrollPane = new JScrollPane(panel, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);

		return scrollPane;
	}

	private JComponent createSettingComponent(final Object settingObject, final PageModel pageModel) {
		JPanel settingPanel;
		JPanel settingContentPanel;
		JPanel textPanel;
		JComponent selectorComponent;
		JLabel title;
		String helpString;
		JLabel imageLabel;

		settingPanel = new JPanel(new BorderLayout(10, 5));
		//settingPanel.setBorder(BOTTOM_SPACER_BORDER);
		settingPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		settingPanel.setAlignmentY(Component.TOP_ALIGNMENT);
		settingPanel.setBorder(BorderFactory.createBevelBorder(1));

		if(settingObject instanceof BooleanSetting) {
			BooleanSetting setting;

			setting = (BooleanSetting)settingObject;
			title = createLabel(setting.getTitle(), TITLE_FONT);
			helpString = getLocalizedInfo(setting.getHelp(), true);
			selectorComponent = createSettingComponent(setting, pageModel);
			imageLabel = createImageLabel(setting.getImage());
		}
		else if(settingObject instanceof ChoiceSetting) {
			ChoiceSetting setting;
			
			setting = (ChoiceSetting)settingObject;
			title = createLabel(setting.getTitle(), TITLE_FONT);
			helpString = getLocalizedInfo(setting.getHelp(), true);
			selectorComponent = createSettingComponent(setting, pageModel);
			imageLabel = createImageLabel(setting.getImage());
		}
		else if(settingObject instanceof ValueSetting) {
			ValueSetting setting;
			
			setting = (ValueSetting)settingObject;
			title = createLabel(setting.getTitle(), TITLE_FONT);
			helpString = getLocalizedInfo(setting.getHelp(), true);
			selectorComponent = createSettingComponent(setting, pageModel);
			imageLabel = createImageLabel(setting.getImage());
		}
		else {
			throw new Error(String.format("Unexpected class %s of setting object.", settingObject.getClass().getCanonicalName()));
		}
		
		title.setAlignmentY(Component.TOP_ALIGNMENT);
		title.setAlignmentX(Component.LEFT_ALIGNMENT);
		selectorComponent.setAlignmentY(Component.TOP_ALIGNMENT);
		selectorComponent.setAlignmentX(Component.LEFT_ALIGNMENT);

		settingContentPanel = new JPanel(new BorderLayout(10, 5));
		settingContentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		settingContentPanel.add(selectorComponent, BorderLayout.LINE_END);
		textPanel = new JPanel(new BorderLayout(10, 5));
			

		if(helpString == null) {
			textPanel.add(title, BorderLayout.CENTER);
		}		
		else {
			JEditorPane help;
			
			textPanel.add(title, BorderLayout.PAGE_START);
			help = createHTMLPane(helpString);
			help.setFont(DESCRIPTION_FONT);
			help.setMargin(new Insets(0, 0, 0, 0));
			help.setBackground(settingPanel.getBackground());
			help.setAlignmentY(Component.TOP_ALIGNMENT);
			help.setAlignmentX(Component.LEFT_ALIGNMENT);
			textPanel.add(help, BorderLayout.CENTER);
			settingContentPanel.add(selectorComponent, BorderLayout.LINE_END);
		}
		
		if(imageLabel != null) {
			imageLabel.setAlignmentX(Component.LEFT_ALIGNMENT);
			imageLabel.setAlignmentY(Component.TOP_ALIGNMENT);
			textPanel.add(imageLabel, BorderLayout.LINE_START);
		}
		
		settingContentPanel.add(textPanel, BorderLayout.CENTER);
		settingPanel.add(settingContentPanel);
		
		return settingPanel;
	}

	private JComponent createSettingComponent(final BooleanSetting b, final PageModel pageModel) {
		JPanel selectorPanel;
		ButtonGroup buttons;
		JRadioButton enableButton;
		JRadioButton disableButton;
		boolean isEnabled;
	
		buttons = new ButtonGroup();
		selectorPanel = new JPanel();
		selectorPanel.setLayout(new BoxLayout(selectorPanel, BoxLayout.Y_AXIS));
		selectorPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		isEnabled = b.isDefault();
		enableButton = addRadioButton(MessageHelper.get(this, "trueButtonText", language), null, buttons, selectorPanel, isEnabled);
		disableButton = addRadioButton(MessageHelper.get(this, "falseButtonText", language), null, buttons, selectorPanel, !isEnabled);
		pageModel.addBoolean(b, enableButton, disableButton);
		
		return selectorPanel;
	}
	
	private JComponent createSettingComponent(final ChoiceSetting c, final PageModel pageModel) {
		Option defaultOption;
		List<Option> options;
		final int listOptionCount;
		
		defaultOption = (Option)c.getDefault();
		options = c.getOption();
		listOptionCount = 3;
		
		if(options.size() <= listOptionCount)
			return createOptionRadioButtons(c, defaultOption.getId(), pageModel);
		else
			return createOptionComboBox(c, defaultOption.getId(), pageModel);
	}
	
	private JComponent createSettingComponent(final ValueSetting v, final PageModel pageModel) {
		ValueDomain valueDomain;
		List<Option> options;
		final int listOptionCount;
		
		valueDomain = (ValueDomain)v.getDomain();
		options = c.getOption();
		listOptionCount = 3;
		
		List<String> titles;
		List<String> helps;
		final String[] helpsArray;
		JComboBox comboBox;
		int selectedIndex;
		ItemDisablerListCellRenderer listCellRenderer;
		JPanel panel;
		
		titles = new LinkedList<String>();
		helps = new LinkedList<String>();
		selectedIndex = -1;
		
		for(Option o: choiceSetting.getOption()) {
			String help;
			
			help = getLocalizedInfo(o.getHelp(), false);

			if(o.getId().equals(defaultOptionID)) {
				String defaultPostfix;
				
				selectedIndex = titles.size();
				defaultPostfix = MessageHelper.get(this, "default");
				
				if(help == null || help.equals(""))
					help = defaultPostfix;
				else
					help += " " + defaultPostfix;
			}

			titles.add(getLocalizedInfo(o.getTitle(), true));
			
			if(help == null)
				helps.add(null);
			else
				helps.add(convertTextToHTML(help, null));
		}
		
		helpsArray = helps.toArray(new String[]{});
		listCellRenderer = new ItemDisablerListCellRenderer(helpsArray);
		comboBox = new JComboBox(titles.toArray());
		comboBox.setEditable(true);
		comboBox.setRenderer(listCellRenderer);
		comboBox.setSelectedIndex(selectedIndex);
		comboBox.setBorder(BorderFactory.createEmptyBorder(2, 2, 2, 2));
		pageModel.addChoice(choiceSetting, comboBox, listCellRenderer);
		panel = new JPanel();
		panel.add(comboBox);

		return panel;
	}
	
	private JComponent createOptionComboBox(final ChoiceSetting choiceSetting, final String defaultOptionID, final PageModel pageModel) {
		List<String> titles;
		List<String> helps;
		final String[] helpsArray;
		JComboBox comboBox;
		int selectedIndex;
		ItemDisablerListCellRenderer listCellRenderer;
		JPanel panel;
		
		titles = new LinkedList<String>();
		helps = new LinkedList<String>();
		selectedIndex = -1;
		
		for(Option o: choiceSetting.getOption()) {
			String help;
			
			help = getLocalizedInfo(o.getHelp(), false);

			if(o.getId().equals(defaultOptionID)) {
				String defaultPostfix;
				
				selectedIndex = titles.size();
				defaultPostfix = MessageHelper.get(this, "default");
				
				if(help == null || help.equals(""))
					help = defaultPostfix;
				else
					help += " " + defaultPostfix;
			}

			titles.add(getLocalizedInfo(o.getTitle(), true));
			
			if(help == null)
				helps.add(null);
			else
				helps.add(convertTextToHTML(help, null));
		}
		
		helpsArray = helps.toArray(new String[]{});
		listCellRenderer = new ItemDisablerListCellRenderer(helpsArray);
		comboBox = new JComboBox(titles.toArray());
		comboBox.setEditable(true);
		comboBox.setRenderer(listCellRenderer);
		comboBox.setSelectedIndex(selectedIndex);
		comboBox.setBorder(BorderFactory.createEmptyBorder(2, 2, 2, 2));
		pageModel.addChoice(choiceSetting, comboBox, listCellRenderer);
		panel = new JPanel();
		panel.add(comboBox);

		return panel;
	}

	private JComponent createOptionRadioButtons(final ChoiceSetting choiceSetting, final String defaultOptionID, final PageModel pageModel) {
		ButtonGroup buttons;
		JPanel selectorPanel;
	
		buttons = new ButtonGroup();
		selectorPanel = new JPanel();
		selectorPanel.setLayout(new BoxLayout(selectorPanel, BoxLayout.Y_AXIS));
		selectorPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
	
		for(Option o: choiceSetting.getOption()) {
			boolean selected;
			String title;
			String help;
			
			selected = o.getId().equals(defaultOptionID);
			title = getLocalizedInfo(o.getTitle(), true);
			help = getLocalizedInfo(o.getHelp(), true);
			addRadioButton(title, help, buttons, selectorPanel, selected);
		}
		
		pageModel.addChoice(choiceSetting, buttons);
		
		return selectorPanel;
	}

	private JRadioButton addRadioButton(final String text, final String toolTip, final ButtonGroup buttons, final JPanel panel, final boolean selected) {
		JRadioButton button;
		
		button = new JRadioButton(text);
		button.setFont(DESCRIPTION_FONT);
		button.setSelected(selected);
		
		if(toolTip != null)
			button.setToolTipText(toolTip);
		
		if(selected)
			makeDefault(button);
		
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
	
	public JEditorPane createHTMLPane(final String message) {
		JEditorPane editorPane;
		
		editorPane = new JEditorPane("text/html", message);
		editorPane.setEditable(false);
		editorPane.putClientProperty(JEditorPane.HONOR_DISPLAY_PROPERTIES, true);
		editorPane.setCaretPosition(0);
		
		if(DESKTOP != null)
			editorPane.addHyperlinkListener(this);

		return editorPane;
	}

	private JLabel createLabel(List<? extends Info> info, Font font) {
		JLabel label;
	
		label = new JLabel(getLocalizedInfo(info, true), JLabel.LEFT);
		label.setFont(font);
		
		return label;
	}

	private <T extends Info> String getLocalizedInfo(final List<T> infos, final boolean convertToHTML) {
		String value;
		String defaultValue;
		
		value = null;
		defaultValue = null;
		
		for(T currentInfo: infos) {
			String currentLang;
			String currentValue;
			
			currentLang = currentInfo.getLang();
			currentValue = getText(currentInfo, convertToHTML);
			
			if(currentLang == null || currentLang.equals("")) {
				if(defaultLanguage.equals(language.getLanguage())) {
					value = currentValue;
					break;
				}
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
	
	private <T extends Info> String getText(final T info, final boolean convertToHTML) {
		String value;

		value = null;
		
		if(info instanceof PathInfo) {
			PathInfo pathInfo;
			String path;
			
			pathInfo = (PathInfo)info;
			path = pathInfo.getPath();
			
			if(path != null && !path.equals("")) {
				InputStream inputStream;
				
				try {
					File file;
					
					file = new File(path);
					inputStream = new FileInputStream(file);
					value = readUTF8String(inputStream);
					
					if(convertToHTML)
						value = convertTextToHTML(value, file.getAbsoluteFile().getParentFile().toURI().toURL().toExternalForm());
				}
				catch(FileNotFoundException e) {
					throw new Error(e);
				}
				catch(MalformedURLException e) {
					throw new Error(e);
				}
			}
		}

		if(value == null) {
			value = info.getValue();
			
			if(convertToHTML)
				value = convertTextToHTML(value, configurationDefDirectoryURL.toExternalForm());
		}
		
		return value;
	}

	public static String convertTextToHTML(String value, final String baseURL) {
		
		if(!value.matches(".*<html>.*</html>.*"))
			value = "<html>" + value + "</html>";
		
		if(baseURL != null && !value.matches("<html>.*<head>.*<base .*/>.*</head>.*</html>.*"))
			value = value.replace("<html>", "<html><head><base href=\"" + baseURL + "\"/></head>");
	
		return value;
	}

	public String getMainTitle() {
		return mainTitle;
	}

	public URL getConfigurationDefinitionURL() {
		return configurationDefinitionURL;
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
		window.getGlassPane().setVisible(true);
		window.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
	}

	public void makeWindowActive() {
		window.requestFocus();
		window.setCursor(null);
		window.getGlassPane().setVisible(false);
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
			window.setTitle(mainTitle);
		else
			window.setTitle(mainTitle + ": " + text);
	}

	public void showErrorDialog(String title, String message) {
		String[] options;
		
		options = new String[]{"OK"};
		JOptionPane.showOptionDialog(window, message, title, JOptionPane.OK_OPTION , JOptionPane.ERROR_MESSAGE, null, options, options[0]);
	}

	public synchronized void exit() {
		System.exit(0);
	}

	@Override
	public void hyperlinkUpdate(final HyperlinkEvent hyperlinkEvent) {
		if (DESKTOP != null && hyperlinkEvent.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
			try {
				DESKTOP.browse(hyperlinkEvent.getURL().toURI());
			}
			catch (URISyntaxException e) {
				throw new RuntimeException(e);
			}
			catch (IOException e) {
				throw new RuntimeException(e);
			}
		}
	}

	public static String readUTF8String(final InputStream inputStream) {
		StringBuilder sb;
		Reader reader;
		
		sb = new StringBuilder();
		
		try {
			int readByte;
			
			reader = new InputStreamReader(new BufferedInputStream(inputStream), "UTF-8");
			
			try {
				while((readByte = reader.read()) != -1) {
					sb.append((char)readByte);
				}
			}
			finally {
				reader.close();
			}
		}
		catch(UnsupportedEncodingException e) {
			throw new Error(e);
		}
		catch(IOException e) {
			throw new Error(e);
		}
		
		return sb.toString();
	}
}
