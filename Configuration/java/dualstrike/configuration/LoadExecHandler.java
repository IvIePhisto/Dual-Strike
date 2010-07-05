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
		DeviceHelper.loadConfiguration(this);
	}

	@Override
	public void executionFinished(ExecutionResult result) {
		if(result.isError())
			showError(MessageHelper.get(this, "loadErrorTitle"), MessageHelper.get(this, "loadErrorMessage", ConfigurationEditor.convertTextToHTML(result.getMessage())));
		else {
			try {
				byte[] dataBytes;

				dataBytes = HexFilesUtility.readSimpleHexFile(file);
				getModel().loadBytes(dataBytes);
			}
			catch(ConfigurationException e) {
				switch(e.getType()) {
				case DEVICE:
				case VERSION:
				case BYTE_COUNT:
				case UNITITIALIZED_DATA:
				}
				//TODO
			}
			catch(IOException e) {
				showError(MessageHelper.get(this, "fileAccessErrorTitle"), MessageHelper.get(this, "fileAccessErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage())));
			}
		}
		
		super.executionFinished(result);
	}
}
