package mccf.views.swing;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Event;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.GraphicsEnvironment;
import java.awt.Insets;
import java.awt.Rectangle;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JToolBar;
import javax.swing.KeyStroke;
import javax.swing.border.EmptyBorder;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;

import mccf.MessageHelper;
import mccf.definition.BooleanSetting;
import mccf.definition.ChoiceSetting;
import mccf.definition.Configuration;
import mccf.definition.DescriptionImage;
import mccf.definition.Info;
import mccf.definition.IntegerSetting;
import mccf.definition.Page;
import mccf.definition.PathInfo;
import mccf.file.FileHandler;
import mccf.icons.IconHandler;
import mccf.model.ConfigurationModel;
import mccf.model.PageModel;
import mccf.views.swing.action_listeners.AboutMessageActionListener;
import mccf.views.swing.action_listeners.ActionListenerHandler;
import mccf.views.swing.action_listeners.DefaultsActionListener;
import mccf.views.swing.action_listeners.ExitActionListener;
import mccf.views.swing.action_listeners.HelpActionListener;
import mccf.views.swing.action_listeners.LoadActionListener;
import mccf.views.swing.action_listeners.SaveActionListener;
import mccf.views.swing.action_listeners.TabDefaultsActionListener;

public class ConfigurationFrameView implements HyperlinkListener {	
	private final Locale language;
	private final Locale defaultLanguage;
	private final String mainTitle;
	private final JFrame window;
	private final StatusClearer statusClearer;
	private final List<JComponent> connectionDependingComponents = new LinkedList<JComponent>();
	private final ActionListenerHandler actionListenerHandler;
	private final URL configurationDefDirectoryURL;
	private final FileHandler fileHandler;
	private final ConfigurationModel model;
	
	/*
	static {
		System.setProperty("apple.laf.useScreenMenuBar", "true");
		System.setProperty("apple.awt.brushMetalLook", "true");
	}
	*/

	private JPanel glassPanel;
	private JLabel statusLabel;
	private ConnectionChecker connectionChecker;
	
	public ConfigurationFrameView(final URL configurationDefDirectoryURL, final ConfigurationModel model, final Configuration configuration, final Locale language) {
		this.configurationDefDirectoryURL = configurationDefDirectoryURL;
		this.model = model;
		
		if(language == null)
			this.language = new Locale(configuration.getLang());
		else
			this.language = language;
		
		defaultLanguage = new Locale(configuration.getLang());
		mainTitle = extractLocalizedInfo(configuration.getTitle(), false);
		fileHandler = new FileHandler(this);
		statusClearer = new StatusClearer(this);
		actionListenerHandler = new ActionListenerHandler();
		registerActionHandlers(configuration);
		JFrame.setDefaultLookAndFeelDecorated(true);
		window = new JFrame(mainTitle);
		window.setIconImages(SwingViewUtility.createApplicationImages(configuration.getIconPath()));
		createGlassPanel();
		populateWindow(model, configuration);
	}
	
	@Override
	public void hyperlinkUpdate(final HyperlinkEvent hyperlinkEvent) {
		if(hyperlinkEvent.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
			try {
				SwingViewUtility.DESKTOP.browse(hyperlinkEvent.getURL().toURI());
			}
			catch (URISyntaxException e) {
				throw new RuntimeException(e);
			}
			catch (IOException e) {
				throw new RuntimeException(e);
			}
		}
	}

	public ConfigurationModel getModel() {
		return model;
	}

	JLabel getStatusLabel() {
		return statusLabel;
	}

	public synchronized void setStatusLabelText(final String text) {
		statusLabel.setText(text);
		statusClearer.setActive();
	}

	public ConnectionChecker getConnectionChecker() {
		return connectionChecker;
	}

	public FileHandler getFileHandler() {
		return fileHandler;
	}

	public JFrame getWindow() {
		return window;
	}

	public synchronized void setWindowTitleAmendment(final String text) {
		if(text == null)
			window.setTitle(mainTitle);
		else
			window.setTitle(mainTitle + ": " + text);
	}
	
	public String getMainTitle() {
		return mainTitle;
	}
	
	private void registerActionHandlers(final Configuration configuration) {
		actionListenerHandler.registerActionListener("loadFile", fileHandler.createLoadFileActionListener());
		actionListenerHandler.registerActionListener("saveFile", fileHandler.createSaveFileActionListener());
		actionListenerHandler.registerActionListener("saveAsFile", fileHandler.createSaveAsFileActionListener());
		actionListenerHandler.registerActionListener("closeFile", fileHandler.createCloseFileActionListener());
		actionListenerHandler.registerActionListener("exit", new ExitActionListener(this));
		actionListenerHandler.registerActionListener("save", new SaveActionListener(this));
		actionListenerHandler.registerActionListener("load", new LoadActionListener(this));
		actionListenerHandler.registerActionListener("defaults", new DefaultsActionListener(this));
		actionListenerHandler.registerActionListener("tabDefaults", new TabDefaultsActionListener(this));
		actionListenerHandler.registerActionListener("help", new HelpActionListener(this, extractLocalizedInfo(configuration.getHelp(), true)));
		actionListenerHandler.registerActionListener("about", new AboutMessageActionListener(this));
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
		glassPanel.setBackground(SwingViewUtility.MILK_GLASS);
		glass.add(glassPanel, BorderLayout.CENTER);
	}
	
	private void populateWindow(final ConfigurationModel model, final Configuration configuration) {
		GraphicsEnvironment graphicsEnvironment;
		Rectangle maximumWindowBounds;
		int maximumWidth;
		int maximumHeight;
		Dimension windowSize;
		
		createMenuBar();
		createContentPane(model, configuration);
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

	private void createContentPane(final ConfigurationModel model, final Configuration configuration) {
		JPanel contentPanel;
		JComponent buttons;
		JComponent tabs;
		JComponent statusPanel;
		
		buttons = createToolBar();
		tabs = createTabs(model, configuration);
		statusPanel = createStatusPanel(configuration);

		contentPanel = (JPanel)window.getContentPane();
		contentPanel.add(buttons, BorderLayout.PAGE_START);
		contentPanel.add(tabs, BorderLayout.CENTER);
		contentPanel.add(statusPanel, BorderLayout.PAGE_END);

	}
	
	private JComponent createTabs(final ConfigurationModel model, final Configuration configuration) {
		JTabbedPane tabs;
		TabDefaultsActionListener tabDefaults;
		
		tabs = new JTabbedPane(JTabbedPane.TOP, JTabbedPane.SCROLL_TAB_LAYOUT);
		tabDefaults = (TabDefaultsActionListener)actionListenerHandler.getActionListener("tabDefaults");
		tabDefaults.setTabs(tabs);
		
		for(Page page: configuration.getPage())
			tabs.addTab(extractLocalizedInfo(page.getTitle(), true), createTabContent(model, page, tabDefaults));
		
		return tabs;
	}
	
	private Component createTabContent(final ConfigurationModel model, final Page page, final TabDefaultsActionListener tabDefaults) {
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
		
		tabHelpLabel = createLabel(page.getHelp(), SwingViewUtility.DESCRIPTION_FONT);
		tabHelpLabel.setBorder(SwingViewUtility.BOTTOM_SPACER_BORDER);
		tabHelpLabel.setAlignmentX(Component.LEFT_ALIGNMENT);
		tabHelpLabel.setAlignmentY(Component.TOP_ALIGNMENT);
		panel.add(tabHelpLabel);

		for(Object setting: page.getChoiceOrBooleanOrInteger()) {
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
		settingPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		settingPanel.setAlignmentY(Component.TOP_ALIGNMENT);
		settingPanel.setBorder(BorderFactory.createBevelBorder(1));

		if(settingObject instanceof BooleanSetting) {
			BooleanSetting setting;

			setting = (BooleanSetting)settingObject;
			title = createLabel(setting.getTitle(), SwingViewUtility.TITLE_FONT);
			helpString = extractLocalizedInfo(setting.getHelp(), true);
			selectorComponent = BooleanRadioButtonsView.createPanel(setting, pageModel, defaultLanguage);
			imageLabel = createImageLabel(setting.getImage());
		}
		else if(settingObject instanceof ChoiceSetting) {
			ChoiceSetting setting;
			String defaultOptionID;
			
			setting = (ChoiceSetting)settingObject;
			title = createLabel(setting.getTitle(), SwingViewUtility.TITLE_FONT);
			helpString = extractLocalizedInfo(setting.getHelp(), true);
			defaultOptionID = setting.getDefault();
			
			if(setting.getOption().size() <= 3)
				selectorComponent = ChoiceRadioButtonsView.createPanel(this, setting, defaultOptionID, pageModel);
			else
				selectorComponent = ChoiceComboBoxView.createPanel(this, setting, defaultOptionID, pageModel);
			
			imageLabel = createImageLabel(setting.getImage());
		}
		else if(settingObject instanceof IntegerSetting) {
			IntegerSetting setting;
			
			setting = (IntegerSetting)settingObject;
			title = createLabel(setting.getTitle(), SwingViewUtility.TITLE_FONT);
			helpString = extractLocalizedInfo(setting.getHelp(), true);
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
			help.setFont(SwingViewUtility.DESCRIPTION_FONT);
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

	private JComponent createStatusPanel(final Configuration configuration) {
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
		connectionHelp = new JLabel(extractLocalizedInfo(configuration.getConnectionHelp(), true));
		disconnectedPanel = new JPanel();
		disconnectedPanel.add(connectionHelp);
		disconnectedPanel.add(disconnectedLabel);
		
		connectionStatusPanel = new JPanel();
		connectionStatusPanel.add(connectedLabel);
		connectionStatusPanel.add(disconnectedPanel);
		
		connectionChecker = new ConnectionChecker(connectedLabel, disconnectedPanel, connectionDependingComponents);
		statusLabel = new JLabel(" ", JLabel.LEFT);
		statusLabel.setBorder(BorderFactory.createEmptyBorder(1, 4, 1, 4));
		statusLabel.setFont(SwingViewUtility.DESCRIPTION_FONT);
		
		statusPanel = new JPanel();
		statusPanel.setLayout(new BorderLayout());
		statusPanel.add(statusLabel, BorderLayout.CENTER);
		statusPanel.add(connectionStatusPanel, BorderLayout.LINE_END);
		
		return statusPanel;
	}

	public JEditorPane createHTMLPane(final String message) {
		JEditorPane editorPane;
		
		editorPane = new JEditorPane("text/html", message);
		editorPane.setEditable(false);
		editorPane.putClientProperty(JEditorPane.HONOR_DISPLAY_PROPERTIES, true);
		editorPane.setCaretPosition(0);
		
		if(SwingViewUtility.DESKTOP != null)
			editorPane.addHyperlinkListener(this);

		return editorPane;
	}
	
	private JLabel createLabel(List<? extends Info> info, Font font) {
		JLabel label;
	
		label = new JLabel(extractLocalizedInfo(info, true), JLabel.LEFT);
		label.setFont(font);
		
		return label;
	}

	private JLabel createImageLabel(final DescriptionImage descriptionImage) {
		JLabel label;
		ImageIcon imageIcon;
		
		if(descriptionImage == null)
			return null;
		
		imageIcon = SwingViewUtility.createFileImageIcon(descriptionImage.getPath());
		imageIcon.setDescription(extractLocalizedInfo(descriptionImage.getTitle(), true));
		label = new JLabel(imageIcon);
		label.setToolTipText(extractLocalizedInfo(descriptionImage.getHelp(), true));
		
		return label;
	}

	public <T extends Info> String extractLocalizedInfo(final List<T> infos, final boolean convertToHTML) {
		String value;
		String defaultValue;
		
		value = null;
		defaultValue = null;
		
		for(T currentInfo: infos) {
			String currentLang;
			String currentValue;
			
			currentLang = currentInfo.getLang();
			currentValue = extractInfoText(currentInfo, convertToHTML);
			
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
	
	private <T extends Info> String extractInfoText(final T info, final boolean convertToHTML) {
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
					value = SwingViewUtility.readUTF8String(inputStream);
					
					if(convertToHTML)
						value = SwingViewUtility.convertTextToHTML(value, file.getAbsoluteFile().getParentFile().toURI().toURL().toExternalForm());
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
				value = SwingViewUtility.convertTextToHTML(value, configurationDefDirectoryURL.toExternalForm());
		}
		
		return value;
	}

	public void showErrorDialog(String title, String message) {
		String[] options;
		
		options = new String[]{"OK"};
		JOptionPane.showOptionDialog(window, message, title, JOptionPane.OK_OPTION , JOptionPane.ERROR_MESSAGE, null, options, options[0]);
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

	public synchronized void exit() {
		System.exit(0);
	}
}
