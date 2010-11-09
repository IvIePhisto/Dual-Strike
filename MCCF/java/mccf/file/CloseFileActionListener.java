package mccf.file;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;


public class CloseFileActionListener implements ActionListener {
	private final FileHandler fileHandler;
	
	CloseFileActionListener(final FileHandler fileHandler) {
		this.fileHandler = fileHandler;
	}

	@Override
	public void actionPerformed(ActionEvent arg0) {
		fileHandler.forget();
	}
}
