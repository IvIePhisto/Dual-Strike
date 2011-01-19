package mccf.model.constraints;

import mccf.model.ConfigurationModel;
import mccf.model.SettingChangeListener;
import mccf.model.SettingModel;
import mccf.model.reference.ReferenceModel;

public class Requirement<SourceSettingType extends SettingModel, DestinationSettingType extends SettingModel> implements SettingChangeListener {
	private final ReferenceModel<SourceSettingType> source;
	private final ReferenceModel<DestinationSettingType> destination;
	
	Requirement(final ConfigurationModel configuration, final ReferenceModel<SourceSettingType> source, final ReferenceModel<DestinationSettingType> destination) {
		this.source = source;
		this.destination = destination;
	}

	public ReferenceModel<SourceSettingType> getSource() {
		return source;
	}

	public ReferenceModel<DestinationSettingType> getDestination() {
		return destination;
	}

	@Override
	public void settingChanged(SettingModel setting) {
		// TODO Auto-generated method stub
		
	}
}
