package mccf.model;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;
import java.util.Vector;

import javax.swing.JRadioButton;

import mccf.definition.BooleanSetting;


public class BooleanModel extends SettingModel implements ActionListener {
	private final boolean defaultValue;
	private final JRadioButton enableButton;
	private final JRadioButton disableButton;
	private boolean value;
	private final String id;
	private final List<String> requiredBySettings = new Vector<String>();
	private int activeRequiringSettings = 0;
	
	BooleanModel(final ConfigurationModel configuration, final BooleanSetting booleanSetting, final JRadioButton enableButton, final JRadioButton disableButton) {
		super(configuration, (int)booleanSetting.getByteNo(), (int)booleanSetting.getBitNo());
		defaultValue = booleanSetting.isDefault();
		value = defaultValue;
		this.enableButton = enableButton;
		this.disableButton = disableButton;
		this.id = booleanSetting.getId();
		configuration.registerSetting(id, this);
		enableButton.addActionListener(this);
		disableButton.addActionListener(this);
	}

	@Override
	public synchronized void actionPerformed(final ActionEvent event) {
		if(enableButton.isSelected() != value) {
			value = enableButton.isSelected();
			notifyRequiringSettings();
			getConfiguration().getFileHandler().setModelChanged();
		}
	}

	public synchronized boolean isValue() {
		return value;
	}

	public synchronized void setValue(final boolean value) {
		if(this.value != value) {
			this.value = value;
			enableButton.setSelected(value);
			disableButton.setSelected(!value);
			notifyRequiringSettings();
			getConfiguration().getFileHandler().setModelChanged();
		}
	}

	public boolean isDefaultValue() {
		return defaultValue;
	}

	@Override
	synchronized void loadDefaults() {
		setValue(defaultValue);
	}

	@Override
	synchronized void loadBytes(final byte[] bytes) {
		setValue(ConfigurationModel.getBit(bytes, getByteNo(), getBitNo()));
	}

	@Override
	synchronized void saveBytes(byte[] bytes) {
		if(value)
			bytes[getByteNo()] |= (1 << getBitNo());
	}

	@Override
	synchronized void addRequiredBy(final String source, final String target) {
		requiredBySettings.add(source);
	}

	@Override
	void initConstraints() {
	}
	
	boolean checkNoOtherRequiringOptions(final String optionID) {
		return true;
	}
	
	@Override
	synchronized void requiringSettingIsActive(final String requirement) {
		if(activeRequiringSettings == 0) {
			setValue(true);
			disableButton.setEnabled(false);
		}
		
		activeRequiringSettings++;
	}

	@Override
	synchronized void requiringSettingIsInactive(final String requirement) {
		if(activeRequiringSettings > 0) {
			activeRequiringSettings--;
			
			if(activeRequiringSettings == 0) {
				disableButton.setEnabled(true);
			}
		}
	}

	synchronized void requiringSettings() {
		for(String requiredBySettingID: requiredBySettings) {
			checkNoOtherRequiringOptions(requiredBySettingID);
		}
	}
	
	synchronized void notifyRequiringSettings() {
		for(String requiredBySettingID: requiredBySettings) {
			ChoiceModel requiredBySetting;
			
			requiredBySetting = (ChoiceModel)getConfiguration().getSetting(requiredBySettingID);
			
			if(value)
				requiredBySetting.requiredSettingIsEnabled(requiredBySettingID);
			else
				requiredBySetting.requiredSettingIsDisabled(requiredBySettingID);
		}
	}
}
