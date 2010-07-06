package dualstrike.configuration.device;

import java.io.File;
import java.io.IOException;

public class DeviceHelper {
	private static final File executableFile = new File("device-interface" + (System.getProperty("os.name").contains("Windows")?".exe":""));
		
	private static ExecutionResult execute(boolean reset, String option, File targetFile) {
		try {
			Process p;
			ExecutionOutputBuffer buffer;
			String[] cmd;
			int cmdIndex;
			boolean proccessRunning;
			
			cmdIndex = 1;
			
			if(reset) {
				cmd = new String[4];
				cmd[cmdIndex++] = "-r"; 
			}
			else
				cmd = new String[3];
			
			cmd[0] = executableFile.getPath();
			cmd[cmdIndex++] = option;
			cmd[cmdIndex++] = targetFile.getPath();
			p = Runtime.getRuntime().exec(cmd);
			buffer = new ExecutionOutputBuffer(p.getInputStream(), p.getErrorStream());
			
			while (buffer.read() && (proccessRunning = isProcessRunning(p)))
				if(proccessRunning)
					try {
						Thread.sleep(200);
					}
					catch(InterruptedException e) {}
			
			return new ExecutionResult(buffer.toString(), p.exitValue() > 0);
		} catch (IOException e) {
			throw new Error(e);
		}
	}

	private static boolean isProcessRunning(final Process process) {
		try {
			process.exitValue();

			return false;
		} catch (IllegalThreadStateException e) {
			return true;
		}
	}
	
	public static File loadConfiguration(final ExecutionListener listener) {
		final File tempFile;
		
		try {
			tempFile = File.createTempFile("configuration-loading-data", ".txt");
			tempFile.deleteOnExit();
		}
		catch(IOException e) {
			throw new Error(e);
		}
		
		new Thread() {
			@Override
			public void run() {
				listener.executionFinished(execute(false, "-de", tempFile));
			}
		}.start();		
		
		return tempFile;
	}

	public static void saveConfiguration(final ExecutionListener listener, final File inputFile) {
		new Thread() {
			@Override
			public void run() {
				execute(false, "-e", inputFile);
				listener.executionFinished(execute(false, "-e", inputFile));
			}
		}.start();
	}
}
