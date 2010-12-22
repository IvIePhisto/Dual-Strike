package mccf.views.swing.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JOptionPane;

import mccf.MessageHelper;
import mccf.views.swing.ConfigurationFrameView;


public class ExitActionListener implements ActionListener {
	private final ConfigurationFrameView view;
	
	public ExitActionListener(final ConfigurationFrameView view) {
		this.view = view;
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		if(view.getFileHandler().isModelChanged()) {
			int option;
			
			view.makeWindowInactive();
			option = JOptionPane.showConfirmDialog(view.getWindow(), MessageHelper.get(this, "saveFileQuestionMessage"), MessageHelper.get(this, "saveFileQuestionTitle"), JOptionPane.YES_NO_CANCEL_OPTION);
			
			switch(option) {
			case JOptionPane.YES_OPTION:
				view.getFileHandler().save();
			case JOptionPane.NO_OPTION:
				view.exit();
				break;
			case JOptionPane.CANCEL_OPTION:
			}

			view.makeWindowActive();
		}
		else {
			view.exit();
		}
	}
}
