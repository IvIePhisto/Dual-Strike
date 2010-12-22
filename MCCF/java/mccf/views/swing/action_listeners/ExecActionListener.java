package mccf.views.swing.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import mccf.device.ExecutionListener;
import mccf.device.ExecutionResult;
import mccf.model.ConfigurationModel;
import mccf.views.swing.ConfigurationFrameView;


public abstract class ExecActionListener implements ActionListener, ExecutionListener {
	private final ConfigurationFrameView view;
	
	ExecActionListener(final ConfigurationFrameView view) {
		this.view = view;
	}

	@Override
	public synchronized void actionPerformed(ActionEvent action) {
		view.makeWindowInactive();
		view.getConnectionChecker().disable();
		exec();
	}
	
	abstract void exec();

	protected ConfigurationModel getModel() {
		return view.getModel();
	}

	@Override
	public void executionFinished(ExecutionResult result) {
		view.getConnectionChecker().enable();
		view.makeWindowActive();
	}

	ConfigurationFrameView getView() {
		return view;
	}
}
