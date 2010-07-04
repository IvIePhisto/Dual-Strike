package dualstrike.configuration.device;

public class ExecutionResult {
	private final String message;
	private final boolean isError;
	
	ExecutionResult(final String message, final boolean isError) {
		this.message = message;
		this.isError = isError;
	}

	public String getMessage() {
		return message;
	}

	public boolean isError() {
		return isError;
	}
}
