package mccf.model.requirements;

import mccf.model.ConfigurationModel;
import mccf.model.SettingChangeListener;
import mccf.model.SettingModel;

public class Requirement implements SettingChangeListener {
	private final SettingModel source;
	private final SettingModel destination;
	
	Requirement(final ConfigurationModel configuration, final SettingModel source, final String destinationID) {
		this.source = source;
		destination = configuration.getSetting(destinationID);
	}

	@Override
	public void settingChanged(SettingModel setting) {
		// TODO Auto-generated method stub
		
	}
}
