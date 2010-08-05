package mccf.model;

import java.util.List;
import java.util.Vector;

import javax.swing.ButtonGroup;
import javax.swing.JComboBox;
import javax.swing.JRadioButton;

import mccf.definition.BooleanSetting;
import mccf.definition.ChoiceSetting;
import mccf.file.FileHandler;

public class PageModel  {
	private final FileHandler fileHandler;
	private final List<SettingModel> settings = new Vector<SettingModel>();
	
	PageModel(final FileHandler fileHandler) {
		this.fileHandler = fileHandler;
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
	
	public synchronized void loadBytes(final byte[] bytes) throws ByteLoadingException {
		for(SettingModel setting: settings)
			setting.loadBytes(bytes);
	}
	
	public synchronized void saveBytes(final byte[] bytes) {
		for(SettingModel setting: settings)
			setting.saveBytes(bytes);
	}
}
