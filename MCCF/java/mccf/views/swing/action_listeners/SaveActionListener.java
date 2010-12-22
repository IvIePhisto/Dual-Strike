package mccf.views.swing.action_listeners;

import java.io.File;
import java.io.IOException;

import mccf.HexFilesUtility;
import mccf.MessageHelper;
import mccf.device.DeviceHelper;
import mccf.device.ExecutionResult;
import mccf.device.ReturnCode;
import mccf.views.ByteView;
import mccf.views.swing.ConfigurationFrameView;
import mccf.views.swing.SwingViewUtility;


public class SaveActionListener extends ExecActionListener {
	public SaveActionListener(final ConfigurationFrameView view) {
		super(view);
	}

	@Override
	void exec() {
		try {
			byte[] bytes;
			File file;
			
			bytes = ByteView.saveBytes(getModel());
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
			getView().showErrorDialog(returnCode.getTitle(), SwingViewUtility.convertTextToHTML(returnCode.getMessage(), null));
			break;
		default:
			getView().showErrorDialog(MessageHelper.get(this, "saveErrorTitle"), MessageHelper.get(this, "saveErrorMessage", result.saveMessage()));
		}
		
		if(returnCode.isError())
			getView().setStatusLabelText(MessageHelper.get(this, "saveErrorStatus"));
		else
			getView().setStatusLabelText(MessageHelper.get(this, "savedStatus"));

		super.executionFinished(result);
	}
}
