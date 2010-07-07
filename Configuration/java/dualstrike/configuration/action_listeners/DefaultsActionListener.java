package dualstrike.configuration.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JFrame;

import dualstrike.configuration.model.ConfigurationModel;

public class DefaultsActionListener implements ActionListener {
	private final JFrame view;
	private final ConfigurationModel model;
	
	public DefaultsActionListener(final JFrame view, final ConfigurationModel model) {
		this.view = view;
		this.model = model;
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		view.setEnabled(false);
		model.loadDefaults();
		view.setEnabled(true);
	}
}
