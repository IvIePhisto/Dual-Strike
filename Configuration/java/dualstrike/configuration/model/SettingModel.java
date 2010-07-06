package dualstrike.configuration.model;

public abstract class SettingModel {
	private final int byteNo;
	private final int bitNo;

	protected SettingModel(final int byteNo, final int bitNo) {
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
}
