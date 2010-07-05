package dualstrike.configuration.model;

import java.util.Arrays;
import java.util.List;
import java.util.Vector;

import javax.swing.ButtonGroup;
import javax.swing.JList;
import javax.swing.JRadioButton;

import dualstrike.configuration.definition.BooleanSetting;
import dualstrike.configuration.definition.ChoiceSetting;
import dualstrike.configuration.definition.Configuration;

public class ConfigurationModel {
	private final int byteWidth;
	private final List<SettingModel> settings = new Vector<SettingModel>();
	private final byte device;
	private final byte version;
	
	public ConfigurationModel(final Configuration configuration) {
		this.byteWidth = (int)configuration.getByteWidth();
		this.device = configuration.getDevice();
		this.version = configuration.getVersion();
	}

	public int getByteWidth() {
		return byteWidth;
	}
	
	public void addChoice(final ChoiceSetting choiceSetting, final JList choiceList) {
		ListChoiceModel choiceModel;
		
		choiceModel = new ListChoiceModel(choiceSetting, choiceList);
		settings.add(choiceModel);
	}
	
	public void addChoice(final ChoiceSetting choiceSetting, final ButtonGroup buttons) {
		RadioButtonsChoiceModel choiceModel;
		
		choiceModel = new RadioButtonsChoiceModel(choiceSetting, buttons);
		settings.add(choiceModel);
	}

	public void addBoolean(final BooleanSetting booleanSetting, final JRadioButton enableButton) {
		BooleanModel booleanModel;
		
		booleanModel = new BooleanModel(booleanSetting, enableButton);
		settings.add(booleanModel);
	}
	
	public void loadDefaults() {
		for(SettingModel setting: settings)
			setting.loadDefaults();
	}
	
	public void loadBytes(byte[] bytes) throws ConfigurationException {
		if(bytes.length < byteWidth + 2)
			throw new ConfigurationException(ConfigurationException.Type.BYTE_COUNT);
		
		for(int i = 0; i < byteWidth + 2; i++)
			if(bytes[i] == 0xFF)
				throw new ConfigurationException(ConfigurationException.Type.UNITITIALIZED_DATA);				
		
		if(bytes[0] != device)
			throw new ConfigurationException(ConfigurationException.Type.DEVICE);

		if(bytes[1] != version)
			throw new ConfigurationException(ConfigurationException.Type.VERSION);
		
		bytes = Arrays.copyOfRange(bytes, 2, -1);

		for(SettingModel setting: settings)
			setting.loadBytes(bytes);
	}
	
	static boolean getBit(final byte[] bytes, final int byteNo, final int bitNo) {
		return (byte) ((byte) (bytes[byteNo] >> bitNo) & 0xFE) == 1;
	}
}
