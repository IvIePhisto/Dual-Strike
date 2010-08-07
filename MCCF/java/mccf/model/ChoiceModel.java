package mccf.model;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import javax.xml.bind.JAXBElement;

import mccf.definition.ChoiceSetting;
import mccf.definition.Option;
import mccf.definition.Setting;


public abstract class ChoiceModel extends SettingModel {
	private final int[] byteDividers;
	private final int bitWidth;
	private final boolean[][] optionValues;
	private final String[] optionIDs;
	private final String[][] requirements;
	private final List<String>[] requiredBy;
	private final int[] activeRequiringSettings;
	private final int[] enabledRequiredSettings;
	private final int defaultValue;
	private int currentOption;

	protected final Map<String, Integer> idOptions;
	
	@SuppressWarnings("unchecked")
	ChoiceModel(final ConfigurationModel configuration, final ChoiceSetting choiceSetting) {
		super(configuration, (int)choiceSetting.getByteNo(), (int)choiceSetting.getBitNo());
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
		optionIDs = new String[optionCount];
		requirements = new String[optionCount][];
		requiredBy = new List[optionCount];
		idOptions = new HashMap<String, Integer>(optionCount, 1);
		activeRequiringSettings = new int[optionCount];
		enabledRequiredSettings = new int[optionCount];
		defaultValue = 0;
		defaultID = ((Option)choiceSetting.getDefault()).getId();
		
		for(int i = 0; i < optionCount; i++) {
			String bitPattern;
			Option option;
			String id;
			List<JAXBElement<Object>> requirementsList;
			int j;
			
			option = options.get(i);
			bitPattern = option.getBitPattern();
			id = option.getId();
			optionIDs[i] = id;
			idOptions.put(id, i);
			configuration.registerSetting(id, this);
			
			if(option.getId().equals(defaultID)) {
				defaultValue = i;
			}
			
			requirementsList = option.getRequires();
			requirements[i] = new String[requirementsList.size()];
			requiredBy[i] = new Vector<String>();
			j = 0;
			
			for(JAXBElement<Object> requirement: requirementsList) {
				Setting setting;
				
				setting = (Setting)requirement.getValue();
				requirements[i][j++] = setting.getId();
			}
			
			for(j = 0; j < bitWidth; j++) {
				if(bitPattern.charAt(j) == '1')
					optionValues[i][j] = true;
				else
					optionValues[i][j] = false;
			}
		}

		currentOption = defaultValue;			
		this.defaultValue = defaultValue;
	}

	synchronized void setCurrentOption(final int currentOption) throws IndexOutOfBoundsException {
		if(currentOption < 0 || currentOption >= optionValues.length)
			throw new IndexOutOfBoundsException();
		
		for(String requirement: requirements[this.currentOption]) {
			SettingModel setting;

			setting = getConfiguration().getSetting(requirement);
			setting.requiringSettingIsInactive(requirement);
		}
		
		for(String requirement: requirements[currentOption]) {
			SettingModel setting;

			setting = getConfiguration().getSetting(requirement);
			setting.requiringSettingIsActive(requirement);
		}

		this.currentOption = currentOption;
	}

	public synchronized final int getCurrentOption() {
		return currentOption;
	}

	public int getDefaultValue() {
		return defaultValue;
	}
	
	@Override
	synchronized void loadDefaults() {
		setCurrentOption(getDefaultValue());
	}
	
	@Override
	synchronized void loadBytes(byte[] bytes) {
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
		
		for(int currentBit = 0, currentBitInByte = getBitNo(); currentBit < bitWidth; currentBit++, currentBitInByte++) {
			boolean bit;
			
			if(currentBit == currentByteDivider) {
				currentByte++;

				currentBitInByte = 0;
				
				if(nextByteDividerIndex < byteDividers.length)
					currentByteDivider = byteDividers[nextByteDividerIndex++];
				else
					currentByteDivider = Integer.MAX_VALUE;
			}
			
			bit = ConfigurationModel.getBit(bytes, currentByte, currentBitInByte);
			
			if(bit)
				option += 1<<currentBit;
		}
		
		setCurrentOption(option);
	}

	@Override
	synchronized void saveBytes(byte[] bytes) {
		int currentByteDivider;
		int nextByteDividerIndex;
		int currentByte;
		boolean[] option;
		
		nextByteDividerIndex = 0;
		
		if(byteDividers.length == 0)
			currentByteDivider = Integer.MAX_VALUE;
		else
			currentByteDivider = byteDividers[nextByteDividerIndex++];
		
		currentByte = getByteNo();
		option = optionValues[currentOption];
		
		for(int currentBit = 0, currentBitInByte = getBitNo(); currentBit < bitWidth; currentBit++, currentBitInByte++) {
			int optionIndex;
						
			if(currentBit == currentByteDivider) {
				currentByte++;
				
				currentBitInByte = 0;
				
				if(nextByteDividerIndex < byteDividers.length)
					currentByteDivider = byteDividers[nextByteDividerIndex++];
				else
					currentByteDivider = Integer.MAX_VALUE;
			}

			optionIndex = bitWidth - currentBit - 1;
			
			if(option[optionIndex])
				bytes[currentByte] |= 1 << currentBitInByte;

		}
	}
	
	@Override
	synchronized void addRequiredBy(final String source, final String target) {
		requiredBy[idOptions.get(target)].add(source);
	}
	
	@Override
	synchronized void initConstraints() {
		initRequirements();
	}
	
	synchronized void initRequirements() {
		for(int option = 0; option < requirements.length; option++) {
			String[] optionRequirements;
			
			optionRequirements = requirements[option];
			
			for(String requirement: optionRequirements) {
				String optionID;
				SettingModel setting;
			
				setting = getConfiguration().getSetting(requirement);
				optionID = optionIDs[option];
				setting.addRequiredBy(optionID, requirement);
				
				if(option == currentOption)
					setting.requiringSettingIsActive(requirement);
				else
					setting.requiringSettingIsInactive(requirement);
			}
		}
	}
	
	synchronized boolean checkNoOtherRequiringOptions(final String optionID) {
		int option;
		
		option = idOptions.get(optionID);
		
		for(int i = 0; i < optionIDs.length; i++)
			if(i != option)
				if(activeRequiringSettings[i] > 0)
					return false;
		
		return true;
	}
	
	@Override
	synchronized void requiringSettingIsActive(final String requirement) {
		int requiredOption;
		
		requiredOption = idOptions.get(requirement);
		
		if(activeRequiringSettings[requiredOption] == 0) {
			for(int i = 0; i < optionIDs.length; i++)
				if(i != requiredOption)
					setDisabled(i);
		}

		activeRequiringSettings[requiredOption] = activeRequiringSettings[requiredOption] + 1;
	}

	@Override
	synchronized void requiringSettingIsInactive(final String requirement) {
		int requiredOption;
		
		requiredOption = idOptions.get(requirement);
		
		if(activeRequiringSettings[requiredOption] > 0) {
			activeRequiringSettings[requiredOption] = activeRequiringSettings[requiredOption] -1;
			
			if(activeRequiringSettings[requiredOption] == 0)
				for(int i = 0; i < optionIDs.length; i++)
					if(i != requiredOption)
						if(enabledRequiredSettings[requiredOption] > 1)
							setEnabled(i);
		}
	}
	
	synchronized void requiredSettingIsEnabled(final String requirement) {
		int requiredOption;
		
		requiredOption = idOptions.get(requirement);
		enabledRequiredSettings[requiredOption] = enabledRequiredSettings[requiredOption] + 1;

		if(enabledRequiredSettings[requiredOption] > 1)
			setEnabled(requiredOption);
	}
	
	synchronized boolean requiredSettingIsDisabled(final String requirement) {
		int requiredOption;
		
		requiredOption = idOptions.get(requirement);
		enabledRequiredSettings[requiredOption] = enabledRequiredSettings[requiredOption] - 1;

		if(enabledRequiredSettings[requiredOption] == 0) {
			if(activeRequiringSettings[requiredOption] > 0)
				return false;
			
			setDisabled(requiredOption);
		}
		
		return true;
	}
	
	abstract void setEnabled(int option);
	abstract void setDisabled(int option);
	
	int getOptionCount() {
		return optionIDs.length;
	}
}