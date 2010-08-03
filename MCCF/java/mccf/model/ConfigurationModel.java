package mccf.model;

import java.util.Arrays;
import java.util.List;
import java.util.Vector;

import javax.swing.ButtonGroup;
import javax.swing.JComboBox;
import javax.swing.JRadioButton;

import mccf.definition.BooleanSetting;
import mccf.definition.ChoiceSetting;
import mccf.definition.Configuration;
import mccf.file.FileHandler;


public class ConfigurationModel {
	private final FileHandler fileHandler;
	private final int byteWidth;
	private final List<SettingModel> settings = new Vector<SettingModel>();
	private final byte device;
	private final byte version;
	
	public ConfigurationModel(final FileHandler fileHandler, final Configuration configuration) {
		this.fileHandler = fileHandler;
		this.byteWidth = (int)configuration.getByteWidth();
		this.device = configuration.getDevice();
		this.version = configuration.getVersion();
	}

	public int getByteWidth() {
		return byteWidth;
	}
	
	public synchronized void addChoice(final ChoiceSetting choiceSetting, final JComboBox comboBox) {
		ComboBoxChoiceModel choiceModel;
		
		choiceModel = new ComboBoxChoiceModel(fileHandler, choiceSetting, comboBox);
		settings.add(choiceModel);
	}
	
	public synchronized void addChoice(final ChoiceSetting choiceSetting, final ButtonGroup buttons) {
		RadioButtonsChoiceModel choiceModel;
		
		choiceModel = new RadioButtonsChoiceModel(fileHandler, choiceSetting, buttons);
		settings.add(choiceModel);
	}

	public synchronized void addBoolean(final BooleanSetting booleanSetting, final JRadioButton enableButton, final JRadioButton disableButton) {
		BooleanModel booleanModel;
		
		booleanModel = new BooleanModel(fileHandler, booleanSetting, enableButton, disableButton);
		settings.add(booleanModel);
	}
	
	public synchronized void loadDefaults() {
		for(SettingModel setting: settings)
			setting.loadDefaults();
	}
	
	public synchronized void loadBytes(byte[] bytes) throws ByteLoadingException {
		if(bytes.length < byteWidth + 2)
			throw new ByteLoadingException(ByteLoadingException.Type.BYTE_COUNT);
		
		for(int i = 0; i < byteWidth + 2; i++)
			if(bytes[i] == 0xFF)
				throw new ByteLoadingException(ByteLoadingException.Type.UNITITIALIZED_DATA);				
		
		if(bytes[0] != device)
			throw new ByteLoadingException(ByteLoadingException.Type.DEVICE);

		if(bytes[1] != version)
			throw new ByteLoadingException(ByteLoadingException.Type.VERSION);
		
		bytes = Arrays.copyOfRange(bytes, 2, bytes.length);

		for(SettingModel setting: settings)
			setting.loadBytes(bytes);
	}
	
	public synchronized byte[] saveBytes() {
		byte[] bytes;
		
		bytes = new byte[byteWidth + 2];
		
		for(SettingModel setting: settings)
			setting.saveBytes(bytes);
		
		for(int i = 0; i < byteWidth; i++)
			bytes[bytes.length - i - 1] = bytes[bytes.length - i - 3];
		
		bytes[0] = device;
		bytes[1] = version;
		
		return bytes;
	}
	
	static boolean getBit(final byte[] bytes, final int byteNo, final int bitNo) {
		byte value;
		boolean bit;
		
		value = (byte)(bytes[byteNo] >> bitNo);
		value &= 1;
		bit = value == 1;
		
		return bit;
	}
}
