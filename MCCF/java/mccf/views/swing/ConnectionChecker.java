package mccf.views.swing;

import java.util.List;

import javax.swing.JComponent;

import mccf.device.DeviceHelper;


public class ConnectionChecker {
	private static final long CHECKING_INTERVAL = 1000L;
	private final JComponent connectedLabel;
	private final JComponent disconnectedLabel;
	private final List<JComponent> dependingComponents;
	private boolean enabled = true;
	
	ConnectionChecker(final JComponent connectedLabel, final JComponent disconnectedLabel, final List<JComponent> dependingComponents) {
		this.connectedLabel = connectedLabel;
		this.disconnectedLabel = disconnectedLabel;
		this.dependingComponents = dependingComponents;
		scheduleCheck();

		for(JComponent dependingComponent: dependingComponents)
			dependingComponent.setEnabled(false);
	}
	
	public synchronized void disable() {
		enabled = false;
	}
	
	public synchronized void enable() {
		enabled = true;
	}
		
	private synchronized void check() {
		if(enabled) {
			if(DeviceHelper.isConnected()) {
				if(disconnectedLabel.isVisible()) {
					disconnectedLabel.setVisible(false);
					connectedLabel.setVisible(true);

					for(JComponent dependingComponent: dependingComponents)
						dependingComponent.setEnabled(true);
				}
			}
			else {
				if(connectedLabel.isVisible()) {
					connectedLabel.setVisible(false);
					disconnectedLabel.setVisible(true);
					
					for(JComponent dependingComponent: dependingComponents)
						dependingComponent.setEnabled(false);
				}
			}
		}
	}
	
	private void scheduleCheck() {
		Thread checkingThread;
		
		checkingThread = new Thread() {
			@Override
			public void run() {
				try {
					while(true) {
						Thread.sleep(CHECKING_INTERVAL);
						check();	
					}
				}
				catch(InterruptedException e) {}
			}
		};
		
		checkingThread.setDaemon(true);
		checkingThread.start();
	}
}
