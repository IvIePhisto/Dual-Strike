package dualstrike.configuration.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.MessageHelper;

public class DefaultsActionListener implements ActionListener {
	private final ConfigurationEditor controller;
	
	public DefaultsActionListener(final ConfigurationEditor controller) {
		this.controller = controller;
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		controller.getView().setEnabled(false);
		controller.getModel().loadDefaults();
		controller.setStatusLabelText(MessageHelper.get(this, "defaultsLoadedStatus"));
		controller.getView().setEnabled(true);
	}
}
