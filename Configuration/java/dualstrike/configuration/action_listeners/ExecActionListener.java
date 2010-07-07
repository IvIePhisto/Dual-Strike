package dualstrike.configuration.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JOptionPane;

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
		controller.makeViewInactive();
		exec();
	}
	
	abstract void exec();

	protected ConfigurationModel getModel() {
		return controller.getModel();
	}

	@Override
	public void executionFinished(ExecutionResult result) {
		controller.makeViewActive();
	}
	
	protected void showError(String title, String message) {
		String[] options;
		
		options = new String[]{"OK"};
		JOptionPane.showOptionDialog(controller.getView(), message, title, JOptionPane.OK_OPTION , JOptionPane.ERROR_MESSAGE, null, options, options[0]);
	}
}
