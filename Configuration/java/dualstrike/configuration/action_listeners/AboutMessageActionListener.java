package dualstrike.configuration.action_listeners;

import java.awt.BorderLayout;
import java.awt.Desktop;
import java.awt.Dialog.ModalityType;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.URISyntaxException;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JEditorPane;
import javax.swing.JPanel;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.MessageHelper;

public class AboutMessageActionListener implements ActionListener, HyperlinkListener {
	private final ConfigurationEditor controller;
	private static final Desktop desktop = Desktop.isDesktopSupported()?Desktop.getDesktop():null;
	private final String title;
	private final String message;
	
	public AboutMessageActionListener(final ConfigurationEditor controller) {
		this.controller = controller;
		this.title = MessageHelper.get(this, "aboutTitle");
		this.message = readResource(MessageHelper.get(this, "aboutMessagePath"));
	}
	
	private static String readResource(final String path) {
		StringBuilder sb;
		InputStream inputStream;
		Reader reader;
		
		inputStream = AboutMessageActionListener.class.getResourceAsStream(path);
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
	
	@Override
	public synchronized void actionPerformed(final ActionEvent actionEvent) {
		final JDialog dialog;
		JEditorPane editorPane;
		JPanel panel;
		Point windowLocation;
		Dimension windowSize;
		Dimension dialogSize;
		int x;
		int y;
		JButton button;
		
		controller.makeWindowInactive();
		editorPane = new JEditorPane("text/html", message);
		editorPane.setEditable(false);
		editorPane.setMargin(new Insets(10, 10, 10, 10));
		editorPane.setPreferredSize(new Dimension(600, 300));
		editorPane.addHyperlinkListener(this);
		panel = new JPanel(new BorderLayout(10, 10));
		panel.add(editorPane);
		panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		dialog = new JDialog(controller.getWindow(), title, ModalityType.APPLICATION_MODAL);
		dialog.add(panel, BorderLayout.CENTER);
		button = new JButton(MessageHelper.get(this, "okButtonTitle"));
		button.addActionListener(
				new ActionListener() {
					@Override
					public void actionPerformed(ActionEvent arg0) {
						dialog.dispose();
					}
				}
		);
		panel = new JPanel();
		panel.add(button);
		dialog.add(panel, BorderLayout.PAGE_END);
		dialog.pack();
		windowLocation = controller.getWindow().getLocation();
		dialogSize = dialog.getSize();
		windowSize = controller.getWindow().getSize();
		x = windowLocation.x + (windowSize.width - dialogSize.width)/2;
		y = windowLocation.y + (windowSize.height - dialogSize.height)/2;
		dialog.setLocation(x, y);
		dialog.setVisible(true);
		controller.makeWindowActive();
	}

	@Override
	public void hyperlinkUpdate(final HyperlinkEvent hyperlinkEvent) {
		if (desktop != null && hyperlinkEvent.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
			try {
				desktop.browse(hyperlinkEvent.getURL().toURI());
			}
			catch (URISyntaxException e) {
				throw new RuntimeException(e);
			}
			catch (IOException e) {
				throw new RuntimeException(e);
			}
		}
	}
}
