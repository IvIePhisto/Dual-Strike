package mccf.model.reference;

import mccf.model.ConfigurationModel;
import mccf.model.SettingModel;


public abstract class ReferenceModel<SettingType extends SettingModel> {
	private final ConfigurationModel configuration;
	private final String id;
	private final Class<?> settingClass;
	private SettingType referencedSetting = null;
	
	ReferenceModel(final ConfigurationModel configuration, final String id, final Class<?> settingClass) {
		this.configuration = configuration;
		this.id = id;
		this.settingClass = settingClass;
	}
	
	ReferenceModel(final ConfigurationModel configuration, final String id, final SettingType referencedSetting) {
		this.configuration = configuration;
		this.id = id;
		this.referencedSetting = referencedSetting;
		settingClass = referencedSetting.getClass();
	}
	
	ConfigurationModel getConfiguration() {
		return configuration;
	}

	String getID() {
		return id;
	}

	@SuppressWarnings("unchecked")
	public SettingType getSetting() throws NullPointerException, ClassCastException {
		if(referencedSetting == null) {
			SettingModel referencedSetting;
			
			referencedSetting = getConfiguration().getSetting(getID());
			
			if(referencedSetting == null)
				throw new NullPointerException();
			
			settingClass.cast(referencedSetting);
			
			this.referencedSetting = (SettingType)referencedSetting;
		}
		
		return referencedSetting;
	}
	
	public BooleanReferenceModel asBooleanReference() throws UnsupportedOperationException {
		throw new UnsupportedOperationException();
	}

	public ChoiceReferenceModel asChoiceReference() throws UnsupportedOperationException {
		throw new UnsupportedOperationException();
	}
	
	public IntegerReferenceModel asIntegerReference() throws UnsupportedOperationException {
		throw new UnsupportedOperationException();
	}
}
