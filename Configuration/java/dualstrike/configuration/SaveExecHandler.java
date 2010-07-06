package dualstrike.configuration;

import java.io.File;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JFrame;

import dualstrike.configuration.device.DeviceHelper;
import dualstrike.configuration.device.ExecutionResult;
import dualstrike.configuration.model.ConfigurationModel;

public class SaveExecHandler extends ExecHandler {
	SaveExecHandler(JFrame view, ConfigurationModel model, JButton button) {
		super(view, model, button);
	}

	@Override
	void exec() {
		try {
			byte[] bytes;
			File file;
			
			bytes = getModel().saveBytes();
			file = File.createTempFile("configuration-saving-data", ".txt");
			file.deleteOnExit();
			HexFilesUtility.writeI8HEXFile(file, bytes, (byte)-1);
			DeviceHelper.saveConfiguration(this, file);
		}
		catch(IOException e) {
			throw new Error(e);
		}
	}

	@Override
	public void executionFinished(ExecutionResult result) {
		if(result.isError())
			showError(MessageHelper.get(this, "saveErrorTitle"), MessageHelper.get(this, "saveErrorMessage", ConfigurationEditor.convertTextToHTML(result.getMessage())));
		
		super.executionFinished(result);
	}
}
