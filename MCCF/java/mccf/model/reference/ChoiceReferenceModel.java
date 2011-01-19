package mccf.model.reference;

import mccf.model.ChoiceModel;
import mccf.model.ConfigurationModel;


public class ChoiceReferenceModel extends ReferenceModel<ChoiceModel> {
	private int option = -1;
	
	public ChoiceReferenceModel(final ConfigurationModel configuration, final String id) {
		super(configuration, id, ChoiceModel.class);
	}

	public int getOption() {
		if(option == -1)
			option = getSetting().getOption(getID());
		
		return option;
	}
	

	@Override
	public ChoiceReferenceModel asChoiceReference() {
		return this;
	}
}
