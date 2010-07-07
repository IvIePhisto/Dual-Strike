package dualstrike.configuration.action_listeners;

import java.awt.Cursor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import dualstrike.configuration.device.ExecutionListener;
import dualstrike.configuration.device.ExecutionResult;
import dualstrike.configuration.model.ConfigurationModel;

public abstract class ExecActionListener implements ActionListener, ExecutionListener {
	private final JFrame view;
	private final ConfigurationModel model;
	
	ExecActionListener(final JFrame view, final ConfigurationModel model) {
		this.view = view;
		this.model = model;
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		view.setEnabled(false);
		view.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
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
		view.setCursor(null);
		view.setEnabled(true);
		view.requestFocus();
	}
	
	protected void showError(String title, String message) {
		String[] options;
		
		options = new String[]{"OK"};
		JOptionPane.showOptionDialog(view, message, title, JOptionPane.OK_OPTION , JOptionPane.ERROR_MESSAGE, null, options, options[0]);
	}
}
