package dualstrike.configuration.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.device.ExecutionListener;
import dualstrike.configuration.device.ExecutionResult;
import dualstrike.configuration.model.ConfigurationModel;

public abstract class ExecActionListener implements ActionListener, ExecutionListener {
	private final ConfigurationEditor controller;
	
	ExecActionListener(final ConfigurationEditor controller) {
		this.controller = controller;
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		controller.makeWindowInactive();
		exec();
	}
	
	abstract void exec();

	protected ConfigurationModel getModel() {
		return controller.getModel();
	}

	@Override
	public void executionFinished(ExecutionResult result) {
		controller.makeWindowActive();
	}

	ConfigurationEditor getController() {
		return controller;
	}
}
