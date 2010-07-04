package dualstrike.configuration;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;

import dualstrike.configuration.model.ConfigurationModel;

class LoadDefaultsHandler implements ActionListener {
	private final JFrame view;
	private final ConfigurationModel model;
	
	LoadDefaultsHandler(final JFrame view, final ConfigurationModel model, final JButton button) {
		this.view = view;
		this.model = model;
		button.addActionListener(this);
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		view.setEnabled(false);
		model.loadDefaults();
		view.setEnabled(true);
	}
}
