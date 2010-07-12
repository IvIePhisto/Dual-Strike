package mccf.action_listeners;

import java.io.File;
import java.io.IOException;

import mccf.ConfigurationEditor;
import mccf.HexFilesUtility;
import mccf.MessageHelper;
import mccf.device.DeviceHelper;
import mccf.device.ExecutionResult;
import mccf.device.ReturnCode;
import mccf.model.ByteLoadingException;


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
		ReturnCode returnCode;
		
		returnCode = result.getReturnCode();
		
		switch(returnCode) {
		case COMPLETED_SUCCESSFULLY:
			try {
				byte[] dataBytes;

				dataBytes = HexFilesUtility.readSimpleHexFile(file);
				file = null;
				getModel().loadBytes(dataBytes);
				getController().setStatusLabelText(MessageHelper.get(this, "loadedStatus"));
			}
			catch(ByteLoadingException e) {
				e.showErrorDialog(getController());
				getController().setStatusLabelText(MessageHelper.get(this, "loadErrorStatus"));
			}
			catch(IOException e) {
				getController().showErrorDialog(MessageHelper.get(this, "fileAccessErrorTitle"), MessageHelper.get(this, "fileAccessErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage(), null)));
				getController().setStatusLabelText(MessageHelper.get(this, "loadErrorStatus"));
			}
			break;
		case OPEN_DEVICE_ERROR:
		case EEPROM_DUMPING_ERROR:
			getController().showErrorDialog(returnCode.getTitle(), ConfigurationEditor.convertTextToHTML(returnCode.getMessage(), null));
			break;
		default:
			getController().showErrorDialog(MessageHelper.get(this, "loadErrorTitle"), MessageHelper.get(this, "loadErrorMessage", result.saveMessage()));
		}
		
		if(returnCode.isError())
			getController().setStatusLabelText(MessageHelper.get(this, "loadErrorStatus"));
		
		super.executionFinished(result);
	}
}
