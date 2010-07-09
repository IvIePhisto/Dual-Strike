package dualstrike.configuration.action_listeners;

import java.awt.Dialog.ModalityType;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
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
		this.message = MessageHelper.get(this, "aboutMessage");
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		String[] options;
		JDialog dialog;
		JOptionPane optionPane;
		JScrollPane scrollPane;
		JPanel panel;
		JLabel label;
		
		controller.makeWindowInactive();
		/*
		scrollPane = new JScrollPane(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		scrollPane.add(label);
		*/
		optionPane = new JOptionPane(message, JOptionPane.INFORMATION_MESSAGE);
		dialog = new JDialog(controller.getWindow(), title, ModalityType.APPLICATION_MODAL);
		dialog.add(optionPane);
		dialog.setLocationRelativeTo(null);
		dialog.setResizable(true);
		dialog.setVisible(true);
		
		
		/*
		options = new String[]{"OK"};
		JOptionPane.showOptionDialog(controller.getWindow(), message, title, JOptionPane.OK_OPTION , JOptionPane.INFORMATION_MESSAGE, null, options, options[0]);
		*/
		controller.makeWindowActive();
	}
}
