package mccf.model;


public abstract class SettingModel {
	private final int byteNo;
	private final int bitNo;
	private final ConfigurationModel configuration;

	protected SettingModel(final ConfigurationModel configuration, final long byteNo, final long bitNo) {
		this.configuration = configuration;
		this.byteNo = (int)byteNo;
		this.bitNo = (int)bitNo;
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
	abstract void initConstraints();
	abstract void addRequiredBy(String source, String requirement);
	
	ConfigurationModel getConfiguration() {
		return configuration;
	}
}
