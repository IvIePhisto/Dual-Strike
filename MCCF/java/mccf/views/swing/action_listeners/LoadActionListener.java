package mccf.views.swing.action_listeners;

import java.io.File;
import java.io.IOException;

import mccf.HexFilesUtility;
import mccf.MessageHelper;
import mccf.device.DeviceHelper;
import mccf.device.ExecutionResult;
import mccf.device.ReturnCode;
import mccf.views.ByteLoadingException;
import mccf.views.ByteView;
import mccf.views.swing.ConfigurationFrameView;
import mccf.views.swing.SwingViewUtility;


public class LoadActionListener extends ExecActionListener {
	private File file;

	public LoadActionListener(final ConfigurationFrameView view) {
		super(view);
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
				ByteView.loadBytes(getModel(), dataBytes);
				getView().setStatusLabelText(MessageHelper.get(this, "loadedStatus"));
			}
			catch(ByteLoadingException e) {
				getView().showErrorDialog(e.getTitle(), e.getLocalizedMessage());
				getView().setStatusLabelText(MessageHelper.get(this, "loadErrorStatus"));
			}
			catch(IOException e) {
				getView().showErrorDialog(MessageHelper.get(this, "fileAccessErrorTitle"), MessageHelper.get(this, "fileAccessErrorMessage", SwingViewUtility.convertTextToHTML(e.getLocalizedMessage(), null)));
				getView().setStatusLabelText(MessageHelper.get(this, "loadErrorStatus"));
			}
			break;
		case OPEN_DEVICE_ERROR:
		case EEPROM_DUMPING_ERROR:
			getView().showErrorDialog(returnCode.getTitle(), SwingViewUtility.convertTextToHTML(returnCode.getMessage(), null));
			break;
		default:
			getView().showErrorDialog(MessageHelper.get(this, "loadErrorTitle"), MessageHelper.get(this, "loadErrorMessage", result.saveMessage()));
		}
		
		if(returnCode.isError())
			getView().setStatusLabelText(MessageHelper.get(this, "loadErrorStatus"));
		
		super.executionFinished(result);
	}
}
