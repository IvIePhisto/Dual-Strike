package mccf.model.reference;

import mccf.definition.IntegerReference;
import mccf.model.ConfigurationModel;
import mccf.model.IntegerModel;


public class IntegerReferenceModel extends ReferenceModel<IntegerModel> {
	private final int value;

	IntegerReferenceModel(final ConfigurationModel configuration, final IntegerReference reference) {
		super(configuration, reference.getRef(), IntegerModel.class);
		this.value = reference.getValue();
	}
	
	public int getValue() {
		return value;
	}
	

	@Override
	public IntegerReferenceModel asIntegerReference() {
		return this;
	}
}
