package mccf.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import mccf.ConfigurationEditor;
import mccf.MessageHelper;


public class DefaultsActionListener implements ActionListener {
	private final ConfigurationEditor controller;
	
	public DefaultsActionListener(final ConfigurationEditor controller) {
		this.controller = controller;
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		controller.getWindow().setEnabled(false);
		controller.getModel().loadDefaults();
		controller.setStatusLabelText(MessageHelper.get(this, "defaultsLoadedStatus"));
		controller.getWindow().setEnabled(true);
	}
}
