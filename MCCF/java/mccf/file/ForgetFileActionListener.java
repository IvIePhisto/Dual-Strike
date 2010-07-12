package mccf.file;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;


public class ForgetFileActionListener implements ActionListener {
	private final FileHandler fileHandler;
	
	ForgetFileActionListener(final FileHandler fileHandler) {
		this.fileHandler = fileHandler;
	}

	@Override
	public void actionPerformed(ActionEvent arg0) {
		fileHandler.forget();
	}
}
