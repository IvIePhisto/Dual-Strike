package mccf.model.reference;

import mccf.definition.BooleanReference;
import mccf.model.BooleanModel;
import mccf.model.ConfigurationModel;


public class BooleanReferenceModel extends ReferenceModel<BooleanModel> {
	private final boolean value;
	
	public BooleanReferenceModel(final ConfigurationModel configuration, final BooleanReference reference) {
		super(configuration, reference.getRef(), BooleanModel.class);
		this.value = reference.isValue();
	}

	public BooleanReferenceModel(final ConfigurationModel configuration, final BooleanModel referencedSetting, final boolean value) {
		super(configuration, referencedSetting.getID(), referencedSetting);
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
