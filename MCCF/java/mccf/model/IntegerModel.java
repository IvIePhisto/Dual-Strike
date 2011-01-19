package mccf.model;

import mccf.definition.IntegerSetting;


public class IntegerModel extends SettingModel {
	private final IntegerDomainModel valueDomain;
	private final int[] byteDividers;
	private final int bitWidth;
	private final int defaultValue;
	private int value;

	IntegerModel(final ConfigurationModel configuration, final IntegerSetting integerSetting) {
		super(configuration, integerSetting.getByteNo(), integerSetting.getBitNo());
		
		int i;
		
		defaultValue = (int)integerSetting.getDefault();
		value = defaultValue;
		bitWidth = (int)integerSetting.getBitWidth();
		
		i = 0;
		byteDividers = new int[integerSetting.getByteDividers().size()];
		
		for(Long byteDivider: integerSetting.getByteDividers())
			byteDividers[i++] = byteDivider.intValue();
		
		valueDomain = configuration.getValueDomain(integerSetting.getDomain());
	}

	@Override
	void loadDefaults() {
		setValue(defaultValue);
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

	public IntegerDomainModel getValueDomain() {
		return valueDomain;
	}

	public int getValue() {
		return value;
	}

	public void setValue(int value) {
		if(value < 0 || value > valueDomain.getEntryCount())
			throw new Error(String.format("Value %d is out of range, must be greater than 0 and less than or equal %d.", value, valueDomain.getEntryCount()));
		
		if(this.value != value) {
			this.value = value;
			changed();
		}
	}

	public int[] getByteDividers() {
		return byteDividers;
	}

	public int getBitWidth() {
		return bitWidth;
	}

	public int getDefaultValue() {
		return defaultValue;
	}
}