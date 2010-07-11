package dualstrike.configuration.action_listeners;

import java.io.File;
import java.io.IOException;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.HexFilesUtility;
import dualstrike.configuration.MessageHelper;
import dualstrike.configuration.device.DeviceHelper;
import dualstrike.configuration.device.ExecutionResult;
import dualstrike.configuration.device.ReturnCode;

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
			throw new RuntimeException(e);
		}
	}

	@Override
	public void executionFinished(ExecutionResult result) {
		ReturnCode returnCode;
		
		returnCode = result.getReturnCode();
		
		switch(returnCode) {
		case COMPLETED_SUCCESSFULLY:
			break;
		case OPEN_DEVICE_ERROR:
		case EEPROM_PROGRAMMING_ERROR:
			getController().showErrorDialog(returnCode.getTitle(), ConfigurationEditor.convertTextToHTML(returnCode.getMessage(), null));
			break;
		default:
			getController().showErrorDialog(MessageHelper.get(this, "saveErrorTitle"), MessageHelper.get(this, "saveErrorMessage", result.saveMessage()));
		}
		
		if(returnCode.isError())
			getController().setStatusLabelText(MessageHelper.get(this, "saveErrorStatus"));
		else
			getController().setStatusLabelText(MessageHelper.get(this, "savedStatus"));

		super.executionFinished(result);
	}
}
