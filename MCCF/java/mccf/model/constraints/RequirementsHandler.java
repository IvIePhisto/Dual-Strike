package mccf.model.constraints;

import java.util.LinkedList;
import java.util.List;

import mccf.model.ConfigurationModel;

public class RequirementsHandler {
	private final List<Requirement> requirements = new LinkedList<Requirement>();
	private final ConfigurationModel configuration;
	
	public RequirementsHandler(final ConfigurationModel configuration) {
		this.configuration = configuration;
	}
	
	public void init() {
		for(Requirement requirement: requirements) {
			// TODO
		}
	}
}
