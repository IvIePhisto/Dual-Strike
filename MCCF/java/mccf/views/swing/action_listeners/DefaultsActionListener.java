package mccf.views.swing.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import mccf.MessageHelper;
import mccf.views.swing.ConfigurationFrameView;


public class DefaultsActionListener implements ActionListener {
	private final ConfigurationFrameView view;
	
	public DefaultsActionListener(final ConfigurationFrameView view) {
		this.view = view;
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		view.getWindow().setEnabled(false);
		view.getModel().loadDefaults();
		view.setStatusLabelText(MessageHelper.get(this, "defaultsLoadedStatus"));
		view.getWindow().setEnabled(true);
	}
}
