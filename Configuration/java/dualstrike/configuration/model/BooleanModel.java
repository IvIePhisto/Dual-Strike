package dualstrike.configuration.model;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JRadioButton;

import dualstrike.configuration.definition.BooleanSetting;

public class BooleanModel extends SettingModel implements ActionListener {
	private final boolean defaultValue;
	private final JRadioButton enableButton;
	private boolean value;
	
	BooleanModel(final BooleanSetting booleanSetting, final JRadioButton enableButton) {
		super((int)booleanSetting.getByteNo(), (int)booleanSetting.getBitNo());
		defaultValue = booleanSetting.isDefault();
		value = defaultValue;
		this.enableButton = enableButton;
		enableButton.addActionListener(this);
	}

	@Override
	public void actionPerformed(final ActionEvent event) {
		if(enableButton.isSelected() != value) {
			value = enableButton.isSelected();
		}
	}

	public boolean isValue() {
		return value;
	}

	public void setValue(final boolean value) {
		this.value = value;
		enableButton.setSelected(value);
	}

	public boolean isDefaultValue() {
		return defaultValue;
	}

	@Override
	synchronized void loadDefaults() {
		setValue(defaultValue);
	}

	@Override
	void loadBytes(final byte[] bytes) {
		setValue(ConfigurationModel.getBit(bytes, getByteNo(), getBitNo()));
	}

	@Override
	void saveBytes(byte[] bytes) {
		if(value)
			bytes[getByteNo()] |= (1 << getBitNo());
	}
}
