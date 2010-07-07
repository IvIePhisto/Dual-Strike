package dualstrike.configuration.action_listeners;

import java.io.File;
import java.io.IOException;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.HexFilesUtility;
import dualstrike.configuration.MessageHelper;
import dualstrike.configuration.device.DeviceHelper;
import dualstrike.configuration.device.ExecutionResult;
import dualstrike.configuration.model.ConfigurationException;

public class LoadActionListener extends ExecActionListener {
	private File file;

	public LoadActionListener(final ConfigurationEditor controller) {
		super(controller);
	}

	@Override
	void exec() {
		file = DeviceHelper.loadConfiguration(this);
	}

	@Override
	public void executionFinished(ExecutionResult result) {
		if(result.isError()) {
			showError(MessageHelper.get(this, "loadErrorTitle"), MessageHelper.get(this, "loadErrorMessage", ConfigurationEditor.convertTextToHTML(result.getMessage())));
			getController().setStatusLabelText(MessageHelper.get(this, "loadErrorStatus"));
		}
		else {
			try {
				byte[] dataBytes;

				dataBytes = HexFilesUtility.readSimpleHexFile(file);
				file = null;
				getModel().loadBytes(dataBytes);
				getController().setStatusLabelText(MessageHelper.get(this, "loadedStatus"));
			}
			catch(ConfigurationException e) {
				switch(e.getType()) {
				case DEVICE:
					showError(MessageHelper.get(this, "deviceErrorTitle"), MessageHelper.get(this, "deviceErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage())));
					break;
				case VERSION:
					showError(MessageHelper.get(this, "versionErrorTitle"), MessageHelper.get(this, "versionErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage())));
					break;
				case BYTE_COUNT:
					showError(MessageHelper.get(this, "byteCountErrorTitle"), MessageHelper.get(this, "byteCountErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage())));
					break;
				case UNITITIALIZED_DATA:
					showError(MessageHelper.get(this, "unititializedDataErrorTitle"), MessageHelper.get(this, "unititializedDataErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage())));
					break;
				default:
					throw new Error("Unsuspected ConfigurationException type.");
				}
				
				getController().setStatusLabelText(MessageHelper.get(this, "loadErrorStatus"));
			}
			catch(IOException e) {
				showError(MessageHelper.get(this, "fileAccessErrorTitle"), MessageHelper.get(this, "fileAccessErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage())));
				getController().setStatusLabelText(MessageHelper.get(this, "loadErrorStatus"));
			}
		}
		
		super.executionFinished(result);
	}
}
