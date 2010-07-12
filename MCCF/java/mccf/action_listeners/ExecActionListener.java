package mccf.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import mccf.ConfigurationEditor;
import mccf.device.ExecutionListener;
import mccf.device.ExecutionResult;
import mccf.model.ConfigurationModel;


public abstract class ExecActionListener implements ActionListener, ExecutionListener {
	private final ConfigurationEditor controller;
	
	ExecActionListener(final ConfigurationEditor controller) {
		this.controller = controller;
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		controller.makeWindowInactive();
		controller.getConnectionChecker().disable();
		exec();
	}
	
	abstract void exec();

	protected ConfigurationModel getModel() {
		return controller.getModel();
	}

	@Override
	public void executionFinished(ExecutionResult result) {
		controller.getConnectionChecker().enable();
		controller.makeWindowActive();
	}

	ConfigurationEditor getController() {
		return controller;
	}
}
