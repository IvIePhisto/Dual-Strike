package dualstrike.configuration.model;

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
	
	public ConfigurationModel(final Configuration configuration) {
		this.byteWidth = (int)configuration.getByteWidth();
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
}
