package dualstrike.configuration.device;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

class ExecutionOutputBuffer {
	private final StringBuilder buffer = new StringBuilder();
	private final BufferedReader in;
	private final BufferedReader err;
	
	ExecutionOutputBuffer(final InputStream in, final InputStream err) {
		this.in = new BufferedReader(new InputStreamReader(in));
		this.err = new BufferedReader(new InputStreamReader(err));
	}
	
	boolean read() throws IOException {
		String text;
		boolean dataRead;
		
		text = in.readLine();
		dataRead = false;
		
		if(text != null) {
			buffer.append(text);
			dataRead = true;
		}

		text = err.readLine();
		
		if(text != null) {
			buffer.append(text);
			dataRead = true;
		}
		
		return dataRead;
	}
	
	@Override
	public String toString() {
		return buffer.toString();
	}
}
