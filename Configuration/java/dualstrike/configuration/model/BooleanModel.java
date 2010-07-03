package dualstrike.configuration.model;

import dualstrike.configuration.definition.BooleanSetting;

public class BooleanModel extends SettingModel {
	private final boolean defaultValue;
	private boolean value;
	
	BooleanModel(BooleanSetting booleanSetting) {
		super((int)booleanSetting.getByteNo(), (int)booleanSetting.getBitNo());
		defaultValue = booleanSetting.isDefault();
		value = defaultValue;
	}

	public boolean isValue() {
		return value;
	}

	public void setValue(boolean value) {
		this.value = value;
	}

	public boolean isDefaultValue() {
		return defaultValue;
	}
}
