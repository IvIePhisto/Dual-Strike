package dualstrike.configuration;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

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
		view.getGlassPane().setVisible(true);
		view.setEnabled(false);
		exec();
	}
	
	abstract void exec();

	protected ConfigurationModel getModel() {
		return model;
	}

	@Override
	public void executionFinished(ExecutionResult result) {
		JPanel glass;

		glass = (JPanel)view.getGlassPane();
		glass.setVisible(false);
		glass.removeAll();
		view.toFront();
		view.getGlassPane().setVisible(false);
		view.setEnabled(true);
	}
	
	protected void showError(String title, String message) {
		String[] options;
		
		options = new String[]{"OK"};
		JOptionPane.showOptionDialog(view, message, title, JOptionPane.OK_OPTION , JOptionPane.ERROR_MESSAGE, null, options, options[0]);
	}
}
