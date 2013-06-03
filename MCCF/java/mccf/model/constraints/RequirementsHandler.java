package mccf.model.constraints;

import java.util.LinkedList;
import java.util.List;

import mccf.model.ConfigurationModel;
import mccf.model.SettingModel;
import mccf.model.reference.ReferenceModel;

public class RequirementsHandler {
	private final List<Requirement<?,?>> requirements = new LinkedList<Requirement<?,?>>();
	private final ConfigurationModel configuration;
	
	public RequirementsHandler(final ConfigurationModel configuration) {
		this.configuration = configuration;
	}
	
	public <SourceSettingType extends SettingModel, DestinationSettingType extends SettingModel>
	void  addRequirement(final ReferenceModel<SourceSettingType> source, ReferenceModel<DestinationSettingType> destination) {
		final Requirement<SourceSettingType, DestinationSettingType> requirement;
		
		requirement = new Requirement<SourceSettingType, DestinationSettingType>(configuration, source, destination);
		requirements.add(requirement);
	}
	
	public void init() {
		for(Requirement<?,?> requirement: requirements)
			requirement.init();
	}
}
