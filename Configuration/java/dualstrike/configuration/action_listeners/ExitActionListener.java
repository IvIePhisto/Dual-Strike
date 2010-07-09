package dualstrike.configuration.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JOptionPane;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.MessageHelper;

public class ExitActionListener implements ActionListener {
	private final ConfigurationEditor controller;
	
	public ExitActionListener(final ConfigurationEditor controller) {
		this.controller = controller;
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		if(controller.getFileHandler().isModelChanged()) {
			int option;
			
			controller.makeWindowInactive();
			option = JOptionPane.showConfirmDialog(controller.getWindow(), MessageHelper.get(this, "saveFileQuestionMessage"), MessageHelper.get(this, "saveFileQuestionTitle"), JOptionPane.YES_NO_CANCEL_OPTION);
			
			switch(option) {
			case JOptionPane.YES_OPTION:
				controller.getFileHandler().save();
			case JOptionPane.NO_OPTION:
				controller.exit();
				break;
			case JOptionPane.CANCEL_OPTION:
			}

			controller.makeWindowActive();
		}
		else {
			controller.exit();
		}
	}
}
