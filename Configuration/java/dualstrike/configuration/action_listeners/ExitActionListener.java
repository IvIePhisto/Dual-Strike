package dualstrike.configuration.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import dualstrike.configuration.ConfigurationEditor;

public class ExitActionListener implements ActionListener {
	private final ConfigurationEditor controller;
	
	public ExitActionListener(final ConfigurationEditor controller) {
		this.controller = controller;
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		controller.exit();
	}
}
