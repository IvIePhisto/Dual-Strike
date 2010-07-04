package dualstrike.configuration.model;

public abstract class SettingModel {
	private final int byteNo;
	private final int bitNo;

	protected SettingModel(final int byteNo, final int bitNo) {
		this.byteNo = byteNo;
		this.bitNo = bitNo;
	}

	public int getByteNo() {
		return byteNo;
	}

	public int getBitNo() {
		return bitNo;
	}
}
