package dualstrike.configuration.model;

import dualstrike.configuration.definition.Configuration;

public class ConfigurationModel {
	private final int byteWidth;
	
	public ConfigurationModel(Configuration configuration) {
		this.byteWidth = (int)configuration.getByteWidth();
	}

	public int getByteWidth() {
		return byteWidth;
	}
}
