package dualstrike.configuration;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;

import dualstrike.configuration.device.ExecutionListener;
import dualstrike.configuration.device.ExecutionResult;
import dualstrike.configuration.model.ConfigurationModel;

abstract class ExecHandler implements ActionListener, ExecutionListener {
	private final JFrame view;
	private final ConfigurationModel model;
	
	ExecHandler(final JFrame view, final ConfigurationModel model, final JButton button) {
		this.view = view;
		this.model = model;
		button.addActionListener(this);
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		view.setEnabled(false);
		view.getGlassPane().setVisible(true);
		exec();
	}
	
	abstract void exec();

	protected ConfigurationModel getModel() {
		return model;
	}

	@Override
	public void executionFinished(ExecutionResult result) {
		//view.toFront();
		view.getGlassPane().setVisible(false);
		view.setEnabled(true);
		view.requestFocus();
	}
	
	protected void showError(String title, String message) {
		String[] options;
		
		options = new String[]{"OK"};
		JOptionPane.showOptionDialog(view, message, title, JOptionPane.OK_OPTION , JOptionPane.ERROR_MESSAGE, null, options, options[0]);
	}
}
