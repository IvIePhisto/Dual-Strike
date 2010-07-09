package dualstrike.configuration.action_listeners;

import java.io.File;
import java.io.IOException;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.HexFilesUtility;
import dualstrike.configuration.MessageHelper;
import dualstrike.configuration.device.DeviceHelper;
import dualstrike.configuration.device.ExecutionResult;
import dualstrike.configuration.model.ByteLoadingException;

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
			getController().showErrorDialog(MessageHelper.get(this, "loadErrorTitle"), MessageHelper.get(this, "loadErrorMessage", ConfigurationEditor.convertTextToHTML(result.getMessage())));
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
			catch(ByteLoadingException e) {
				e.showErrorDialog(getController());
				getController().setStatusLabelText(MessageHelper.get(this, "loadErrorStatus"));
			}
			catch(IOException e) {
				getController().showErrorDialog(MessageHelper.get(this, "fileAccessErrorTitle"), MessageHelper.get(this, "fileAccessErrorMessage", ConfigurationEditor.convertTextToHTML(e.getLocalizedMessage())));
				getController().setStatusLabelText(MessageHelper.get(this, "loadErrorStatus"));
			}
		}
		
		super.executionFinished(result);
	}
}
