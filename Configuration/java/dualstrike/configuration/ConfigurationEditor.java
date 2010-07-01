package dualstrike.configuration;

import javax.swing.JFrame;

public class ConfigurationEditor {
	
	private ConfigurationEditor() {
		
	}
	
	private void init() {
		//1. Create the frame.
		JFrame frame = new JFrame("Foo Bar");

		//2. Optional: What happens when the frame closes?
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		//4. Size the frame.
		frame.pack();

		//5. Show it.
		frame.setVisible(true);

	}
	
	public static void main(String[] args) {
		ConfigurationEditor ce;
		
		ce = new ConfigurationEditor();
		ce.init();
		
	}
}
