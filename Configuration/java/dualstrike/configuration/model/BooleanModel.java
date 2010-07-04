package dualstrike.configuration.model;

import javax.swing.JRadioButton;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import dualstrike.configuration.definition.BooleanSetting;

public class BooleanModel extends SettingModel implements ChangeListener {
	private final boolean defaultValue;
	private final JRadioButton enableButton;
	private boolean value;
	
	BooleanModel(final BooleanSetting booleanSetting, final JRadioButton enableButton) {
		super((int)booleanSetting.getByteNo(), (int)booleanSetting.getBitNo());
		defaultValue = booleanSetting.isDefault();
		value = defaultValue;
		this.enableButton = enableButton;
		enableButton.addChangeListener(this);
	}

	@Override
	public void stateChanged(final ChangeEvent event) {
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
}
