package mccf.model;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import mccf.definition.ChoiceSetting;
import mccf.definition.Option;
import mccf.definition.OptionRequires;


public class ChoiceModel extends SettingModel {
	private final int[] byteDividers;
	private final int bitWidth;
	private final int defaultValue;
	private int currentOption;
	private final int optionCount;
	
	private final String[] option2ID;
	private final Map<String, Integer> id2Option;
	private final String[][] requirements;
	
	private final List<ChoiceModel>[] requiringChoices;
	private final List<Integer>[] requiringOptions;
	private final int[] activeRequiringSettings;
	private final List<SettingModel>[] requiredSettingsByOption;
	private final List<Integer>[] requiredOptionsByOption;
	private final int[] enabledRequiredSettings;
	
	@SuppressWarnings("unchecked")
	public ChoiceModel(final ConfigurationModel configuration, final ChoiceSetting choiceSetting) {
		super(configuration, choiceSetting.getByteNo(), choiceSetting.getBitNo());
		List<Option> options;
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
		option2ID = new String[optionCount];
		requirements = new String[optionCount][];
		requiredSettingsByOption = new List[optionCount];
		requiredOptionsByOption = new List[optionCount];
		requiringChoices = new List[optionCount];
		requiringOptions = new List[optionCount];
		activeRequiringSettings = new int[optionCount];
		enabledRequiredSettings = new int[optionCount];
		id2Option = new HashMap<String, Integer>(optionCount, 1);
		defaultValue = 0;
		defaultID = choiceSetting.getDefault();
		
		for(int i = 0; i < optionCount; i++) {
			Option option;
			String id;
			List<OptionRequires> requirementsList;
			int j;
			
			option = options.get(i);
			id = option.getId();
			option2ID[i] = id;
			id2Option.put(id, i);
			configuration.registerSetting(id, this);
			
			if(option.getId().equals(defaultID)) {
				defaultValue = i;
			}
			
			requirementsList = option.getRequires();
			requirements[i] = new String[requirementsList.size()];
			requiredSettingsByOption[i] = new Vector<SettingModel>();
			requiredOptionsByOption[i] = new Vector<Integer>();
			requiringChoices[i] = new Vector<ChoiceModel>();
			requiringOptions[i] = new Vector<Integer>();
			j = 0;
			
			for(OptionRequires requirement: requirementsList) {
				//TODO
			}
		}

		currentOption = defaultValue;			
		this.defaultValue = defaultValue;
	}

	public int[] getByteDividers() {
		return byteDividers;
	}

	public int getBitWidth() {
		return bitWidth;
	}

	public int getOptionCount() {
		return optionCount;
	}

	public synchronized void setCurrentOption(final int currentOption) throws IndexOutOfBoundsException {
		if(currentOption != this.currentOption) {
			if(currentOption < 0 || currentOption >= optionCount)
				throw new IndexOutOfBoundsException();
			
			this.currentOption = currentOption;
			changed();
		}
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
		
	public void enable(int option) {
		//TODO
	}
	
	public void disable(int option) {
		//TODO
	}
	
	public boolean isDisabled(int option) {
		//TODO
		return false;
	}
	
	public int getOption(final String id) {
		Integer option;
		
		option = id2Option.get(id);
		
		if(option == null)
			option = -1;
		
		return option;
	}

	@Override
	public int getNormalizedSetting() {
		return currentOption;
	}
}