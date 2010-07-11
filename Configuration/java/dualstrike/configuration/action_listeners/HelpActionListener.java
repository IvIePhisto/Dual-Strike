package dualstrike.configuration.action_listeners;

import java.awt.BorderLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JScrollPane;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.MessageHelper;
import dualstrike.configuration.icons.IconHandler;

public class HelpActionListener implements ActionListener {
	private final ConfigurationEditor controller;
	private final String title;
	private final String message;
	private JFrame helpWindow;
	
	public HelpActionListener(final ConfigurationEditor controller, final String message) {
		this.controller = controller;
		this.title = controller.getMainTitle() + " - " + MessageHelper.get(this, "helpTitle");
		this.message = message;
	}
	
	@Override
	public synchronized void actionPerformed(final ActionEvent actionEvent) {
		if(helpWindow == null) {
			JEditorPane editorPane;
			JScrollPane scrollPane;
			
			editorPane = controller.createHTMLPane(message);
			editorPane.setMargin(new Insets(10, 10, 10, 10));
			scrollPane = new JScrollPane(editorPane, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
			helpWindow = new JFrame(title);
			helpWindow.setIconImage(IconHandler.getIcon("help", null, 16, null).getImage());
			helpWindow.getContentPane().add(scrollPane, BorderLayout.CENTER);
			helpWindow.setPreferredSize(controller.getWindow().getSize());
			helpWindow.setLocationByPlatform(true);
			helpWindow.pack();
			editorPane.setFont(ConfigurationEditor.DESCRIPTION_FONT.deriveFont(16));
			helpWindow.setVisible(true);
		}
		else  {
			if(!helpWindow.isVisible())
				helpWindow.setVisible(true);
			
			helpWindow.requestFocus();
		}
	}
}
