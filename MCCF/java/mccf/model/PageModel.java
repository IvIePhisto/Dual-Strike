package mccf.model;

import java.util.List;
import java.util.Vector;

import javax.swing.ButtonGroup;
import javax.swing.JComboBox;
import javax.swing.JRadioButton;

import mccf.ItemDisablerListCellRenderer;
import mccf.definition.BooleanSetting;
import mccf.definition.ChoiceSetting;
import mccf.definition.ValueSetting;

public class PageModel  {
	private final ConfigurationModel configuration;
	private final List<SettingModel> settings = new Vector<SettingModel>();
	
	PageModel(final ConfigurationModel configuration) {
		this.configuration = configuration;
	}
	
	public synchronized void addChoice(final ChoiceSetting choiceSetting, final JComboBox comboBox, final ItemDisablerListCellRenderer listCellRenderer) {
		ComboBoxChoiceModel choiceModel;
		
		choiceModel = new ComboBoxChoiceModel(configuration, choiceSetting, comboBox, listCellRenderer);
		settings.add(choiceModel);
	}
	
	public synchronized void addChoice(final ChoiceSetting choiceSetting, final ButtonGroup buttons) {
		RadioButtonsChoiceModel choiceModel;
		
		choiceModel = new RadioButtonsChoiceModel(configuration, choiceSetting, buttons);
		settings.add(choiceModel);
	}

	public synchronized void addBoolean(final BooleanSetting booleanSetting, final JRadioButton enableButton, final JRadioButton disableButton) {
		BooleanModel booleanModel;
		
		booleanModel = new BooleanModel(configuration, booleanSetting, enableButton, disableButton);
		settings.add(booleanModel);
	}
	
	public synchronized void addValue(final ValueSetting valueSetting, final JComboBox comboBox, final ItemDisablerListCellRenderer listCellRenderer) {
		//TODO
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
