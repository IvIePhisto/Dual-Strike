package mccf.model.reference;

import mccf.definition.IntegerReference;
import mccf.model.ConfigurationModel;
import mccf.model.IntegerModel;


public class IntegerReferenceModel extends ReferenceModel<IntegerModel> {
	private final int value;

	public IntegerReferenceModel(final ConfigurationModel configuration, final IntegerReference reference) {
		super(configuration, reference.getRef(), IntegerModel.class);
		this.value = reference.getValue();
	}
	
	public IntegerReferenceModel(final ConfigurationModel configuration, final IntegerModel referencedSetting, final int value) {
		super(configuration, referencedSetting.getID(), referencedSetting);
		this.value = value;
	}

	public int getValue() {
		return value;
	}

	@Override
	public IntegerReferenceModel asIntegerReference() {
		return this;
	}

	@Override
	public boolean isSet() {
		return getSetting().getValue() == value;
	}

	@Override
	public void enableState() {
		// TODO Auto-generated method stub
	}

	@Override
	public void disableState() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void enableOtherStates() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void disableOtherStates() {
		// TODO Auto-generated method stub
		
	}
}
