package mccf.model.reference;

import mccf.model.BooleanModel;
import mccf.model.ConfigurationModel;


public class BooleanReferenceModel extends ReferenceModel<BooleanModel> {
	private final boolean value;
	
	BooleanReferenceModel(final ConfigurationModel configuration, final String id, final boolean value) {
		super(configuration, id, BooleanModel.class);
		this.value = value;
	}

	public boolean getValue() {
		return value;
	}

	@Override
	public BooleanReferenceModel asBooleanReference() {
		return this;
	}
}
