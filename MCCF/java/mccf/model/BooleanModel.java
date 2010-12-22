package mccf.model;

import mccf.definition.BooleanSetting;


public class BooleanModel extends SettingModel {
	private final boolean defaultValue;
	private boolean value;
	private final String id;
	
	BooleanModel(final ConfigurationModel configuration, final BooleanSetting booleanSetting) {
		super(configuration, booleanSetting.getByteNo(), booleanSetting.getBitNo());
		defaultValue = booleanSetting.isDefault();
		value = defaultValue;
		this.id = booleanSetting.getId();
		configuration.registerSetting(id, this);
	}

	public synchronized boolean getValue() {
		return value;
	}

	public synchronized void setValue(final boolean value) {
		if(this.value != value) {
			this.value = value;
			changed();
		}
	}

	public boolean getDefaultValue() {
		return defaultValue;
	}

	@Override
	synchronized void loadDefaults() {
		setValue(defaultValue);
	}

	@Override
	synchronized void addRequiredBy(final String source, final String target, final int value) {
		// TODO
	}

	@Override
	void initConstraints() {
	}
}
