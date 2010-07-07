package dualstrike.configuration.action_listeners;

import java.io.File;
import java.io.IOException;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.HexFilesUtility;
import dualstrike.configuration.MessageHelper;
import dualstrike.configuration.device.DeviceHelper;
import dualstrike.configuration.device.ExecutionResult;

public class SaveActionListener extends ExecActionListener {
	public SaveActionListener(final ConfigurationEditor controller) {
		super(controller);
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
