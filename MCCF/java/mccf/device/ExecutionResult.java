package mccf.device;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;

public class ExecutionResult {
	private static final File MESSAGE_FILE = new File("eepromProgrammerHID_output.txt").getAbsoluteFile();
	private final String message;
	private final ReturnCode returnCode;
	
	ExecutionResult(final String message, final int returnCode) {
		this.message = message;
		this.returnCode = ReturnCode.newInstance(returnCode);
	}

	public String getMessage() {
		return message;
	}

	public ReturnCode getReturnCode() {
		return returnCode;
	}
	
	public File saveMessage() {
		try {
			Writer writer;
	
			writer = new OutputStreamWriter(new BufferedOutputStream(new FileOutputStream(MESSAGE_FILE)), "UTF-8");
				
			try {
				writer.write(message);
			}
			finally {
				writer.flush();
				writer.close();
			}
			
			return MESSAGE_FILE;
		}
		catch(IOException e) {
			throw new RuntimeException(e);
		}
	}
}
