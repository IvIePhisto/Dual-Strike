package dualstrike.configuration;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import javax.swing.JButton;
import javax.swing.JFrame;

import dualstrike.configuration.model.ConfigurationModel;

abstract class ExecHandler implements ActionListener {
	private final JFrame view;
	private final ConfigurationModel model;
	private final String command;
	
	ExecHandler(final JFrame view, final ConfigurationModel model, final JButton button, final String command) {
		this.view = view;
		this.model = model;
		this.command = command;
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
	
	public void execCommand() {
		try {
			Process p;
			
			p = Runtime.getRuntime().exec(command);
			BufferedReader stdIn = new BufferedReader(new InputStreamReader(p
					.getInputStream()));
			BufferedReader errIn = new BufferedReader(new InputStreamReader(p
					.getErrorStream()));

			while (isProcessRunning(p) || stdIn.ready() || errIn.ready()) {
				String s;

				if (stdIn.ready()) {
					s = stdIn.readLine();

					if (s != null)
						System.out.println(s);
				} else if (errIn.ready()) {
					s = errIn.readLine();

					if (s != null)
						System.out.println("ERR: " + s);
				}
			}

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private static boolean isProcessRunning(Process process) {
		try {
			process.exitValue();

			return false;
		} catch (IllegalThreadStateException e) {
			return true;
		}
	}
}
