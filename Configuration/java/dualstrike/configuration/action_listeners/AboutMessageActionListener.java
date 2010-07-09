package dualstrike.configuration.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JOptionPane;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.MessageHelper;

public class AboutMessageActionListener implements ActionListener {
	private final ConfigurationEditor controller;
	private final String title;
	private final String message;
	
	public AboutMessageActionListener(final ConfigurationEditor controller) {
		this.controller = controller;
		this.title = MessageHelper.get(this, "aboutTitle");
		this.message = MessageHelper.get(this, "aboutMessage");
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		String[] options;
		
		controller.makeWindowInactive();
		options = new String[]{"OK"};
		JOptionPane.showOptionDialog(controller.getWindow(), message, title, JOptionPane.OK_OPTION , JOptionPane.INFORMATION_MESSAGE, null, options, options[0]);
		controller.makeWindowActive();
	}
}
