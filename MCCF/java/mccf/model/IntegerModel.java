package mccf.model;

import mccf.definition.IntegerSetting;


public class IntegerModel extends SettingModel {
	private final IntegerDomainModel valueDomain;
	private final int[] byteDividers;
	private final int bitWidth;
	private final int defaultValue;
	private int value;
	private final String id;

	IntegerModel(final ConfigurationModel configuration, final IntegerSetting integerSetting) {
		super(configuration, integerSetting.getByteNo(), integerSetting.getBitNo());
		
		int i;
		
		defaultValue = (int)integerSetting.getDefault();
		value = defaultValue;
		bitWidth = (int)integerSetting.getBitWidth();
		id = integerSetting.getId();
		
		i = 0;
		byteDividers = new int[integerSetting.getByteDividers().size()];
		
		for(Long byteDivider: integerSetting.getByteDividers())
			byteDividers[i++] = byteDivider.intValue();
		
		valueDomain = configuration.getValueDomain(integerSetting.getDomain());
	}

	@Override
	public void loadDefaults() {
		setValue(defaultValue);
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

	public String getID() {
		return id;
	}

	@Override
	public int getNormalizedSetting() {
		return getValue();
	}
}