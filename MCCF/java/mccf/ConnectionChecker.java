package mccf;

import javax.swing.JLabel;

import mccf.device.DeviceHelper;


public class ConnectionChecker {
	private static final long CHECKING_INTERVAL = 1000L;
	private final JLabel connectedLabel;
	private final JLabel disconnectedLabel;
	private boolean enabled = true;
	
	ConnectionChecker(final JLabel connectedLabel, final JLabel disconnectedLabel) {
		this.connectedLabel = connectedLabel;
		this.disconnectedLabel = disconnectedLabel;
		scheduleCheck();
	}
	
	public synchronized void disable() {
		enabled = false;
	}
	
	public synchronized void enable() {
		enabled = true;
		scheduleCheck();
	}
		
	private synchronized void check() {
		if(enabled) {
			if(DeviceHelper.isConnected()) {
				if(disconnectedLabel.isVisible()) {
					disconnectedLabel.setVisible(false);
					connectedLabel.setVisible(true);
				}
			}
			else {
				if(connectedLabel.isVisible()) {
					connectedLabel.setVisible(false);
					disconnectedLabel.setVisible(true);
				}
			}
			
			scheduleCheck();
		}		
	}
	
	private void scheduleCheck() {
		new Thread() {
			@Override
			public void run() {
				try {
					Thread.sleep(CHECKING_INTERVAL);
					check();
				}
				catch(InterruptedException e) {}
			}
		}.start();
	}
}
