package dualstrike.configuration.action_listeners;

import java.awt.BorderLayout;
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
import java.text.MessageFormat;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JEditorPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.MessageHelper;

public class AboutMessageActionListener implements ActionListener {
	private final ConfigurationEditor controller;
	private final String title;
	private final String message;
	
	public AboutMessageActionListener(final ConfigurationEditor controller) {
		this.controller = controller;
		this.title = MessageHelper.get(this, "aboutTitle");
		this.message = MessageFormat.format(readResource(MessageHelper.get(this, "aboutMessagePath")), ConfigurationEditor.MAJOR_VERSION_NO, ConfigurationEditor.MINOR_VERSION_NO, ConfigurationEditor.BUGFIX_VERSION_NO);
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
		JScrollPane scrollPane;
		JPanel buttonPanel;
		Point windowLocation;
		Dimension windowSize;
		Dimension dialogSize;
		int x;
		int y;
		JButton button;
		
		controller.makeWindowInactive();
		dialog = new JDialog(controller.getWindow(), title, ModalityType.APPLICATION_MODAL);
		editorPane = controller.createHTMLPane(message);
		editorPane.setPreferredSize(new Dimension(640, 400));
		editorPane.setMargin(new Insets(10, 10, 10, 10));
		editorPane.setFont(ConfigurationEditor.DESCRIPTION_FONT);
		editorPane.setBackground(dialog.getBackground());
		scrollPane = new JScrollPane(editorPane, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		scrollPane.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		dialog.add(scrollPane, BorderLayout.CENTER);
		button = new JButton(MessageHelper.get(this, "okButtonTitle"));
		button.addActionListener(
				new ActionListener() {
					@Override
					public void actionPerformed(ActionEvent arg0) {
						dialog.dispose();
					}
				}
		);
		buttonPanel = new JPanel();
		buttonPanel.add(button);
		dialog.add(buttonPanel, BorderLayout.PAGE_END);
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
}
