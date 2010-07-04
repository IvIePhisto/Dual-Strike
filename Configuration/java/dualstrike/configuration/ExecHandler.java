package dualstrike.configuration;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;

import dualstrike.configuration.device.ExecutionListener;
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
		exec();
		view.setEnabled(true);
	}
	
	abstract void exec();

	protected ConfigurationModel getModel() {
		return model;
	}
}
