package dualstrike.configuration;

import java.io.File;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JFrame;

import dualstrike.configuration.device.DeviceHelper;
import dualstrike.configuration.device.ExecutionResult;
import dualstrike.configuration.model.ConfigurationException;
import dualstrike.configuration.model.ConfigurationModel;

public class LoadExecHandler extends ExecHandler {
	private File file;

	LoadExecHandler(JFrame view, ConfigurationModel model, JButton button) {
		super(view, model, button);
	}

	@Override
	void exec() {
		file = DeviceHelper.loadConfiguration(this);
	}

	@Override
	public void executionFinished(ExecutionResult result) {
		if(result.isError())
			showError(MessageHelper.get(this, "loadErrorTitle"), MessageHelper.get(this, "loadErrorMessage", ConfigurationEditor.convertTextToHTML(result.getMessage())));
		else {
			try {
				byte[] dataBytes;

				dataBytes = HexFilesUtility.readSimpleHexFile(file);
				file = null;
				getModel().loadBytes(dataBytes);
			}
			catch(ConfigurationException e) {
				switch(e.getType()) {
				case DEVICE:
					showError(MessageHelper.get(this, "deviceErrorTitle"), MessageHelper.get(this, "deviceErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage())));
				case VERSION:
					showError(MessageHelper.get(this, "versionErrorTitle"), MessageHelper.get(this, "versionErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage())));
				case BYTE_COUNT:
					showError(MessageHelper.get(this, "byteCountErrorTitle"), MessageHelper.get(this, "byteCountErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage())));
				case UNITITIALIZED_DATA:
					showError(MessageHelper.get(this, "unititializedDataErrorTitle"), MessageHelper.get(this, "unititializedDataErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage())));
				default:
					throw new Error("Unsuspected ConfigurationException type.");
				}
			}
			catch(IOException e) {
				showError(MessageHelper.get(this, "fileAccessErrorTitle"), MessageHelper.get(this, "fileAccessErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage())));
			}
		}
		
		super.executionFinished(result);
	}
}
