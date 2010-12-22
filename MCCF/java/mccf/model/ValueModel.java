package mccf.model;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComboBox;

import mccf.definition.ValueSetting;
import mccf.views.swing.ItemDisablerListCellRenderer;


public class ValueModel extends SettingModel implements ActionListener {
	private final int[] byteDividers;
	private final int bitWidth;
	private final int defaultValue;
	private int currentValue;

	ValueModel(final ConfigurationModel configuration, final ValueSetting valueSetting) {
		super(configuration, valueSetting.getByteNo(), valueSetting.getBitNo());
		
		int i;
		
		defaultValue = (int)valueSetting.getDefault();
		bitWidth = (int)valueSetting.getBitWidth();
		
		i = 0;
		byteDividers = new int[valueSetting.getByteDividers().size()];
		
		for(Long byteDivider: valueSetting.getByteDividers())
			byteDividers[i++] = byteDivider.intValue();
	}
	
	@Override
	public synchronized void actionPerformed(ActionEvent e) {
		int selectedIndex;
		
		selectedIndex = comboBox.getSelectedIndex();

		if(listCellRenderer.isItemDisabled(selectedIndex)) {
			comboBox.setSelectedIndex(currentValue);
		}
		else {
			if(selectedIndex != currentValue) {
				setCurrentValue(selectedIndex);
				getConfiguration().getFileHandler().setModelChanged();
			}
		}
	}

	public synchronized void setCurrentValue(final int currentValue) throws IndexOutOfBoundsException {
		if(this.currentValue != currentValue) {
			this.currentValue = currentValue;
			comboBox.setSelectedIndex(currentValue);
			getConfiguration().getFileHandler().setModelChanged();
		}
	}

	@Override
	void loadDefaults() {
		setCurrentValue(defaultValue);
	}


	@Override
	void initConstraints() {
		initRequirements();
	}
	
	synchronized void initRequirements() {
		/* TODO
		for(int requiringOption = 0; requiringOption < requirements.length; requiringOption++) {
			String[] requiredSettingIDs;
			String requiringOptionID;
			
			requiredSettingIDs = requirements[requiringOption];
			requiringOptionID = option2ID[requiringOption];
			
			for(String requiredSettingID: requiredSettingIDs) {
				SettingModel requiredSetting;
			
				requiredSetting = getConfiguration().getSetting(requiredSettingID);
				requiredSetting.addRequiredBy(requiringOptionID, requiredSettingID, -1);
				requiredSettingsByOption[requiringOption].add(requiredSetting);
				
				if(requiredSetting instanceof ChoiceModel) {
					ChoiceModel requiredChoice;
					int requiredOption;
					
					requiredChoice = (ChoiceModel)requiredSetting;
					requiredOption = requiredChoice.getOption(requiredSettingID);
					requiredOptionsByOption[requiringOption].add(requiredOption);
				}
				else {
					requiredOptionsByOption[requiringOption].add(-1);
				}
			}
			
			notifyRequiredSettings(requiringOption, currentOption == requiringOption);
		}
		*/
	}
	
	@Override
	void addRequiredBy(String source, String requirement, final int value) {
		// TODO Auto-generated method stub
		
	}
}