package mccf.file;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JFileChooser;


public class SaveFileActionListener implements ActionListener {
	private final FileHandler fileHandler;
	private final boolean alwaysAsk;
	
	SaveFileActionListener(final FileHandler fileHandler, final boolean alwaysAsk) {
		this.fileHandler = fileHandler;
		this.alwaysAsk = alwaysAsk;
	}

	@Override
	public void actionPerformed(final ActionEvent event) {
		File file;
		
		fileHandler.getView().makeWindowInactive();

		if(alwaysAsk || !fileHandler.isFileActive()) {
			if(fileHandler.getFileChooser().showSaveDialog(fileHandler.getView().getWindow()) == JFileChooser.APPROVE_OPTION) {
				file = fileHandler.getFileChooser().getSelectedFile();
				
				fileHandler.save(file);
			}
		}
		else
			fileHandler.save();
	
		fileHandler.getView().makeWindowActive();
	}
}
