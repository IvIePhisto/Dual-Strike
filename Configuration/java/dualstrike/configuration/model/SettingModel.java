package dualstrike.configuration.model;

import dualstrike.configuration.file.FileHandler;

public abstract class SettingModel {
	private final int byteNo;
	private final int bitNo;
	private final FileHandler fileHandler;

	protected SettingModel(final FileHandler fileHandler, final int byteNo, final int bitNo) {
		this.fileHandler = fileHandler;
		this.byteNo = byteNo;
		this.bitNo = bitNo;
	}

	public final int getByteNo() {
		return byteNo;
	}

	public final int getBitNo() {
		return bitNo;
	}
	
	abstract void loadDefaults();
	abstract void loadBytes(byte[] bytes);
	abstract void saveBytes(byte[] bytes);

	FileHandler getFileHandler() {
		return fileHandler;
	}
}
