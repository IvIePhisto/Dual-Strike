package mccf.model;


public abstract class SettingModel {
	private final int byteNo;
	private final int bitNo;
	private final ConfigurationModel configuration;

	protected SettingModel(final ConfigurationModel configuration, final int byteNo, final int bitNo) {
		this.configuration = configuration;
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
	abstract void initConstraints();
	abstract void addRequiredBy(String source, String requirement);
	abstract void requiringSettingIsInactive(String requirement);
	abstract void requiringSettingIsActive(String requirement);
	
	ConfigurationModel getConfiguration() {
		return configuration;
	}
}
