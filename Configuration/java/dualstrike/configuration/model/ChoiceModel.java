package dualstrike.configuration.model;

import java.util.List;

import dualstrike.configuration.definition.ChoiceSetting;
import dualstrike.configuration.definition.Option;

public abstract class ChoiceModel extends SettingModel {
	private final int[] byteDividers;
	private final int bitWidth;
	private final boolean[][] optionValues;
	private final int defaultValue;
	private int currentOption;
	
	ChoiceModel(final ChoiceSetting choiceSetting) {
		super((int)choiceSetting.getByteNo(), (int)choiceSetting.getBitNo());
		List<Option> options;
		int optionCount;
		int defaultValue;
		String defaultID;
		List<Long> rawByteDividers;
		
		bitWidth = (int)choiceSetting.getBitWidth();
		rawByteDividers = choiceSetting.getByteDividers();
		byteDividers = new int[rawByteDividers.size()];
		
		for(int i = 0; i < byteDividers.length; i++)
			byteDividers[i] = rawByteDividers.get(i).intValue();
		
		options = choiceSetting.getOption();
		optionCount = options.size();
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
	
	@Override
	synchronized void loadDefaults() {
		setCurrentOption(getDefaultValue());
	}
	
	@Override
	void loadBytes(byte[] bytes) {
		int currentByteDivider;
		int nextByteDividerIndex;
		int currentByte;
		int option;
		
		nextByteDividerIndex = 0;
		
		if(byteDividers.length == 0)
			currentByteDivider = Integer.MAX_VALUE;
		else
			currentByteDivider = byteDividers[nextByteDividerIndex++];
		
		currentByte = getByteNo();
		option = 0;
		
		for(int currentBit = 0; currentBit < bitWidth; currentBit++) {
			if(currentBit == currentByteDivider) {
				currentByte++;

				if(nextByteDividerIndex < byteDividers.length)
					currentByteDivider = byteDividers[nextByteDividerIndex++];
				else
					currentByteDivider = Integer.MAX_VALUE;
			}
			
			option += ConfigurationModel.getBit(bytes, currentByte, currentBit)?1:0 << currentBit; 
		}
		
		setCurrentOption(option);
	}
}