package dualstrike.configuration.model;

import dualstrike.configuration.ConfigurationEditor;
import dualstrike.configuration.MessageHelper;

public class ByteLoadingException extends Exception {
	private static final long serialVersionUID = 1L;
	private final Type type;
	private final String title;
	
	public static enum Type {
		DEVICE,
		VERSION,
		BYTE_COUNT,
		UNITITIALIZED_DATA
	}
	
	public ByteLoadingException(final Type type) {
		super(createMessage(type));
		this.type = type;
		title = createTitle(type);
	}

	public Type getType() {
		return type;
	}
	
	private static String createTitle(final Type type) {
		switch(type) {
		case DEVICE:
			return MessageHelper.get(ByteLoadingException.class, "deviceTitle");
		case VERSION:
			return MessageHelper.get(ByteLoadingException.class, "versionTitle");
		case BYTE_COUNT:
			return MessageHelper.get(ByteLoadingException.class, "byteCountTitle");
		case UNITITIALIZED_DATA:
			return MessageHelper.get(ByteLoadingException.class, "unititializedDataTitle");
		default:
			throw new Error("Unsuspected ByteLoadingException type.");
		}		
	}

	private static String createMessage(final Type type) {
		switch(type) {
		case DEVICE:
			return MessageHelper.get(ByteLoadingException.class, "deviceMessage");
		case VERSION:
			return MessageHelper.get(ByteLoadingException.class, "versionMessage");
		case BYTE_COUNT:
			return MessageHelper.get(ByteLoadingException.class, "byteCountMessage");
		case UNITITIALIZED_DATA:
			return MessageHelper.get(ByteLoadingException.class, "unititializedDataMessage");
		default:
			throw new Error("Unsuspected ByteLoadingException type.");
		}		
	}
	
	public void showErrorDialog(final ConfigurationEditor controller) {
		controller.showErrorDialog(title, getLocalizedMessage());
	}
}
