package mccf.model.constraints;

import mccf.model.ConfigurationModel;
import mccf.model.SettingChangeListener;
import mccf.model.SettingModel;
import mccf.model.reference.ReferenceModel;

public class Requirement<SourceSettingType extends SettingModel, DestinationSettingType extends SettingModel> implements SettingChangeListener {
	private final ReferenceModel<SourceSettingType> source;
	private final ReferenceModel<DestinationSettingType> destination;
	private SourceSettingType sourceSetting = null;
	private DestinationSettingType destinationSetting = null;
	
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
	
	void init() {
		sourceSetting = source.getSetting();
		destinationSetting = destination.getSetting();
		updateFromSource();
		updateFromDestination();
		sourceSetting.registerChangeListener(this);
		destinationSetting.registerChangeListener(this);
	}

	@Override
	public void settingChanged(final SettingModel setting) {
		if(setting == sourceSetting)
			updateFromSource();
		else if(setting == destinationSetting)
			updateFromDestination();
		else
			throw new Error("Received change event for unknown setting.");
	}
	
	private void updateFromSource() {
		if(source.isSet())
			destination.disableOtherStates();
		else
			destination.enableOtherStates();
	}

	private void updateFromDestination() {
		if(destination.isSet())
			source.disableState();
		else
			source.enableState();
	}
}
