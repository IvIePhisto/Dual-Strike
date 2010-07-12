package mccf;



class StatusClearer {
	private static final long MAXIMUM_CLEAREANCE_DURATION = 15000L;
	private final ConfigurationEditor controller;
	private Thread cleareanceThread;
	
	StatusClearer(final ConfigurationEditor controller) {
		this.controller = controller;
	}
		
	private synchronized void clear() {
		controller.getStatusLabel().setText(" ");
		cleareanceThread = null;
	}

	public synchronized void setActive() {
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
		
		cleareanceThread.start();
	}
}
