package dualstrike.configuration.file;

import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.HexFilesUtility;
import dualstrike.configuration.MessageHelper;
import dualstrike.configuration.model.ByteLoadingException;

public class FileHandler {
	private File file;
	private final ConfigurationEditor controller;
	private final JFileChooser fileChooser;
	
	public FileHandler(final ConfigurationEditor controller) {
		this.controller = controller;
		fileChooser = new JFileChooser();
		fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		fileChooser.setMultiSelectionEnabled(false);
		fileChooser.addChoosableFileFilter(
				new FileFilter() {
					@Override
					public boolean accept(File file) {
						if(file.isFile())
							return file.getName().endsWith(".plain-hex");
						
						if(file.isDirectory())
							return true;
						
						return false;
					}

					@Override
					public String getDescription() {
						return "*.plain-hex";
					}
					
				}
		);
	}
	
	synchronized void load(final File file) {
		if(!file.exists()) {
			controller.showErrorDialog(MessageHelper.get(this, "fileMissingErrorTitle"), MessageHelper.get(this, "fileMissingErrorMessage", file));
			controller.setStatusLabelText(MessageHelper.get(this, "loadFileErrorStatus"));
		}
		else {
			try {
				byte[] data;
				
				data = HexFilesUtility.readSimpleHexFile(file);
				controller.getModel().loadBytes(data);
				this.file = file;
				controller.setWindowTitleAmendment(file.getAbsolutePath());
				controller.setStatusLabelText(MessageHelper.get(this, "fileLoadedStatus"));
			}
			catch(IOException e) {
				controller.showErrorDialog(MessageHelper.get(this, "fileLoadingErrorTitle"), MessageHelper.get(this, "fileLoadingErrorMessage", file, e.getLocalizedMessage()));
				controller.setStatusLabelText(MessageHelper.get(this, "loadFileErrorStatus"));
			}
			catch(ByteLoadingException e) {
				e.showErrorDialog(controller);
				controller.setStatusLabelText(MessageHelper.get(this, "loadFileErrorStatus"));
			}
		}
	}
	
	synchronized void forget() {
		if(file != null) {
			file = null;
			controller.setWindowTitleAmendment(null);
			controller.setStatusLabelText(MessageHelper.get(this, "fileForgottenStatus"));
		}
	}

	public ActionListener createLoadFileActionListener() {
		return new LoadFileActionListener(this);
	}
	
	public ActionListener createForgetFileActionListener() {
		return new ForgetFileActionListener(this);
	}

	ConfigurationEditor getController() {
		return controller;
	}

	JFileChooser getFileChooser() {
		return fileChooser;
	}
}
