package dualstrike.configuration.model;

import java.util.List;

import dualstrike.configuration.definition.ChoiceSetting;
import dualstrike.configuration.definition.Option;

public abstract class ChoiceModel extends SettingModel {
	private final boolean[][] optionValues;
	private final int defaultValue;
	private int currentOption;
	
	ChoiceModel(final ChoiceSetting choiceSetting) {
		super((int)choiceSetting.getByteNo(), (int)choiceSetting.getBitNo());
		List<Option> options;
		int bitWidth;
		int optionCount;
		int defaultValue;
		String defaultID;
		
		options = choiceSetting.getOption();
		optionCount = options.size();
		bitWidth = (int)choiceSetting.getBitWidth();
		optionValues = new boolean[optionCount][bitWidth];
		defaultValue = 0;
		defaultID = ((Option)choiceSetting.getDefault()).getId();
		
		for(int i = 0; i < optionCount; i++) {
			String bitPattern;
			Option option;
			
			option = options.get(i);
			bitPattern = option.getBitPattern();
			
			if(option.getId().equals(defaultID))
					defaultValue = i;
			
			for(int j = 0; j < bitWidth; j++) {
				if(bitPattern.charAt(j) == '1')
					optionValues[i][j] = true;
				else
					optionValues[i][j] = false;
			}
		}

		
		currentOption = defaultValue;			
		this.defaultValue = defaultValue;
	}


	public synchronized void setCurrentOption(final int currentOption) throws IndexOutOfBoundsException {
		if(currentOption < 0 || currentOption >= optionValues.length)
			throw new IndexOutOfBoundsException();
		
		this.currentOption = currentOption;
	}

	public final int getCurrentOption() {
		return currentOption;
	}


	public final int getDefaultValue() {
		return defaultValue;
	}
}