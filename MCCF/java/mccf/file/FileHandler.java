package mccf.file;

import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;

import mccf.HexFilesUtility;
import mccf.MessageHelper;
import mccf.views.ByteLoadingException;
import mccf.views.ByteView;
import mccf.views.swing.ConfigurationFrameView;


public class FileHandler {
	private static final String DEFAULT_EXTENSION = "mcc";
	private File file;
	private final ConfigurationFrameView view;
	private final JFileChooser fileChooser;
	private boolean modelChanged;
	
	public FileHandler(final ConfigurationFrameView view) {
		this.view = view;
		fileChooser = new JFileChooser();
		fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		fileChooser.setMultiSelectionEnabled(false);
		fileChooser.addChoosableFileFilter(
				new FileFilter() {
					@Override
					public boolean accept(File file) {
						if(file.isFile())
							return file.getName().endsWith("." + DEFAULT_EXTENSION);
						
						if(file.isDirectory())
							return true;
						
						return false;
					}

					@Override
					public String getDescription() {
						return "*." + DEFAULT_EXTENSION;
					}
					
				}
		);
	}
	
	public ActionListener createLoadFileActionListener() {
		return new LoadFileActionListener(this);
	}
	
	public ActionListener createCloseFileActionListener() {
		return new CloseFileActionListener(this);
	}

	public ActionListener createSaveFileActionListener() {
		return new SaveFileActionListener(this, false);
	}

	public ActionListener createSaveAsFileActionListener() {
		return new SaveFileActionListener(this, true);
	}
	
	public synchronized void setModelChanged() {
		if(file != null && !modelChanged) {
			view.setWindowTitleAmendment(file.getAbsolutePath() + "*");
			modelChanged = true;
		}
	}

	public synchronized boolean isModelChanged() {
		return modelChanged;
	}

	ConfigurationFrameView getView() {
		return view;
	}

	JFileChooser getFileChooser() {
		return fileChooser;
	}

	synchronized boolean isFileActive() {
		return file != null;
	}

	synchronized void load(final File file) {
		if(!file.exists()) {
			view.showErrorDialog(MessageHelper.get(this, "fileMissingErrorTitle"), MessageHelper.get(this, "fileMissingErrorMessage", file));
			view.setStatusLabelText(MessageHelper.get(this, "loadFileErrorStatus"));
		}
		else {
			try {
				byte[] data;
				
				data = HexFilesUtility.readSimpleHexFile(file);
				ByteView.loadBytes(view.getModel(), data);
				this.file = file;
				view.setWindowTitleAmendment(file.getAbsolutePath());
				view.setStatusLabelText(MessageHelper.get(this, "fileLoadedStatus"));
				modelChanged = false;
			}
			catch(IOException e) {
				view.showErrorDialog(MessageHelper.get(this, "fileLoadingErrorTitle"), MessageHelper.get(this, "fileLoadingErrorMessage", file, e.getLocalizedMessage()));
				view.setStatusLabelText(MessageHelper.get(this, "loadFileErrorStatus"));
			}
			catch(ByteLoadingException e) {
				view.showErrorDialog(e.getTitle(), e.getLocalizedMessage());
				view.setStatusLabelText(MessageHelper.get(this, "loadFileErrorStatus"));
			}
		}
	}

	synchronized void forget() {
		if(file != null) {
			file = null;
			view.setWindowTitleAmendment(null);
			view.setStatusLabelText(MessageHelper.get(this, "fileForgottenStatus"));
			modelChanged = false;
		}
	}
	
	private static File addFileExtension(File file) {
		String name;
		boolean containsDot;
		boolean endsWithDot;
		boolean hasExtension;
		
		name = file.getName();
		containsDot = name.contains(".");
		endsWithDot = name.charAt(name.length() - 1) == '.';
		hasExtension = containsDot && !endsWithDot;
		
		if(!hasExtension)
			file = new File(file.getPath() + (endsWithDot ? "" : ".") + DEFAULT_EXTENSION);
		
		return file;
	}

	synchronized void save(File file) {
		try {
			byte[] data;
			
			file = addFileExtension(file);			
			data = ByteView.saveBytes(view.getModel());
			HexFilesUtility.saveSimpleHexFile(file, data);
			view.setWindowTitleAmendment(file.getAbsolutePath());
			view.setStatusLabelText(MessageHelper.get(this, "fileSavedStatus"));
			modelChanged = false;
		}
		catch(IOException e) {
			view.showErrorDialog(MessageHelper.get(this, "fileSavingErrorTitle"), MessageHelper.get(this, "fileSavingErrorMessage", file, e.getLocalizedMessage()));
			view.setStatusLabelText(MessageHelper.get(this, "saveFileErrorStatus"));
		}
	}

	public synchronized void save() {
		save(file);
	}
}
