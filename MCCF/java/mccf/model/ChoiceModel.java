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
	private final int defaultValue;
	private int currentOption;
	private int optionCount;
	
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
	ChoiceModel(final ConfigurationModel configuration, final ChoiceSetting choiceSetting) {
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
		optionValues = new boolean[optionCount][bitWidth];
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
		
		notifyRequiringSettings(this.currentOption, false);
		notifyRequiredSettings(this.currentOption, false);
		notifyRequiringSettings(currentOption, true);
		notifyRequiredSettings(currentOption, true);
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
		setCurrentOption(BytesUtility.loadIntBytes(bytes, byteDividers, getByteNo(), getBitNo(), bitWidth));
	}

	@Override
	synchronized void saveBytes(byte[] bytes) {
		BytesUtility.saveBytes(currentOption, bytes, byteDividers, getByteNo(), getBitNo(), bitWidth);
	}
	
	@Override
	synchronized void addRequiredBy(final String source, final String target) {
		ChoiceModel requiringChoice;
		int requiredOption;
		
		requiredOption = id2Option.get(target);
		requiringChoice = (ChoiceModel)getConfiguration().getSetting(source);
		requiringChoices[requiredOption].add(requiringChoice);
		requiringOptions[requiredOption].add(requiringChoice.getOption(source));
	}
	
	@Override
	synchronized void initConstraints() {
		initRequirements();
	}
	
	synchronized void initRequirements() {
		for(int requiringOption = 0; requiringOption < requirements.length; requiringOption++) {
			String[] requiredSettingIDs;
			String requiringOptionID;
			
			requiredSettingIDs = requirements[requiringOption];
			requiringOptionID = option2ID[requiringOption];
			
			for(String requiredSettingID: requiredSettingIDs) {
				SettingModel requiredSetting;
			
				requiredSetting = getConfiguration().getSetting(requiredSettingID);
				requiredSetting.addRequiredBy(requiringOptionID, requiredSettingID);
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
	}
	
	synchronized void requiringSettingIsActive(final int requiredOption) {
		System.out.println(activeRequiringSettings[requiredOption]);
		if(activeRequiringSettings[requiredOption] == 0) {
			setCurrentOption(requiredOption);
			
			for(int i = 0; i < optionCount; i++)
				if(i != requiredOption)
					setDisabled(i);
		}

		activeRequiringSettings[requiredOption] = activeRequiringSettings[requiredOption] + 1;
	}

	synchronized void requiringSettingIsInactive(final int requiredOption) {
		if(activeRequiringSettings[requiredOption] > 0) {
			activeRequiringSettings[requiredOption] = activeRequiringSettings[requiredOption] -1;
			
			if(activeRequiringSettings[requiredOption] == 0)
				for(int i = 0; i < option2ID.length; i++)
					if(i != requiredOption)
						setEnabled(i);
		}
	}
	
	synchronized void notifyRequiringSettings(final int requiredOption, final boolean active) {
		List<ChoiceModel> currentRequiringChoices;
		List<Integer> currentRequiringOptions;
		
		currentRequiringChoices = requiringChoices[requiredOption];
		currentRequiringOptions = requiringOptions[requiredOption];
		
		for(int i = 0; i < currentRequiringChoices.size(); i++) {
			ChoiceModel requiringChoice;
			int requiringOption;
			
			requiringChoice = currentRequiringChoices.get(i);
			requiringOption = currentRequiringOptions.get(i);
			
			if(active)
				requiringChoice.requiredSettingIsActive(requiringOption);
			else
				requiringChoice.requiredSettingIsInactive(requiringOption);
		}
	}
	
	synchronized void requiredSettingIsActive(final int requiredOption) {
		if(enabledRequiredSettings[requiredOption] == 0)
			setEnabled(requiredOption);

		enabledRequiredSettings[requiredOption] = enabledRequiredSettings[requiredOption] + 1;
	}
	
	synchronized void requiredSettingIsInactive(final int requiredOption) {
		enabledRequiredSettings[requiredOption] = enabledRequiredSettings[requiredOption] - 1;

		if(enabledRequiredSettings[requiredOption] == 0)
			setDisabled(requiredOption);
	}
	
	synchronized void notifyRequiredSettings(final int requiringOption, final boolean active) {
		List<SettingModel> requiredSettings;
		List<Integer> requiredOptions;
		
		requiredSettings = requiredSettingsByOption[requiringOption];
		requiredOptions = requiredOptionsByOption[requiringOption];
		
		for(int i = 0; i < requiredSettings.size(); i++) {
			SettingModel setting;
			
			setting = requiredSettings.get(i);
			
			if(setting instanceof BooleanModel) {
				BooleanModel booleanModel;
				
				booleanModel = (BooleanModel)setting;

				if(active)
					booleanModel.requiringSettingIsActive();
				else
					booleanModel.requiringSettingIsInactive();
			}
			else if(setting instanceof ChoiceModel) {
				ChoiceModel choiceModel;
				int option;
				
				choiceModel = (ChoiceModel)setting;
				option = requiredOptions.get(i);
				
				if(active)
					choiceModel.requiringSettingIsActive(option);
				else
					choiceModel.requiringSettingIsInactive(option);
			}
			else {
				throw new Error("Unsupported SettingModel type.");
			}
		}
	}
	
	abstract void setEnabled(int option);
	abstract void setDisabled(int option);
	abstract boolean isDisabled(int option);
	
	Integer getOption(final String id) {
		return id2Option.get(id);
	}
}