package mccf.model;

import java.util.LinkedList;
import java.util.List;


public abstract class SettingModel {
	private final int byteNo;
	private final int bitNo;
	private final ConfigurationModel configuration;
	private final List<SettingChangeListener> changeListeners = new LinkedList<SettingChangeListener>();

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
	
	protected void changed() {
		for(SettingChangeListener changeListener: changeListeners)
			changeListener.settingChanged(this);
	}
	
	public void registerChangeListener(final SettingChangeListener changeListener) {
		changeListeners.add(changeListener);
	}
	
	abstract void loadDefaults();
	abstract void initConstraints();
	abstract void addRequiredBy(String source, String requirement, int value);
	
	ConfigurationModel getConfiguration() {
		return configuration;
	}
}
