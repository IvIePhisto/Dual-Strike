package mccf.views.swing;


class StatusClearer {
	private static final long MAXIMUM_CLEAREANCE_DURATION = 15000L;
	private final ConfigurationFrameView view;
	private Thread cleareanceThread;
	
	StatusClearer(final ConfigurationFrameView view) {
		this.view = view;
	}
		
	private synchronized void clear() {
		view.getStatusLabel().setText(" ");
		cleareanceThread = null;
	}

	synchronized void setActive() {
		if(cleareanceThread != null)
			cleareanceThread.interrupt();
		
		cleareanceThread = new Thread() {
			@Override
			public void run() {
				try {
					Thread.sleep(MAXIMUM_CLEAREANCE_DURATION);
					clear();
				}
				catch(InterruptedException e) {}
			}
		};
		
		cleareanceThread.setDaemon(true);
		cleareanceThread.start();
	}
}
