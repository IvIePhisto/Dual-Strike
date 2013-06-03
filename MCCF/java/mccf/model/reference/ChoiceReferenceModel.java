package mccf.model.reference;

import mccf.definition.ChoiceReference;
import mccf.model.ChoiceModel;
import mccf.model.ConfigurationModel;


public class ChoiceReferenceModel extends ReferenceModel<ChoiceModel> {
	private int option = -1;
	
	public ChoiceReferenceModel(final ConfigurationModel configuration, final ChoiceReference choiceReference) {
		super(configuration, choiceReference.getRef(), ChoiceModel.class);
	}
	
	public ChoiceReferenceModel(final ConfigurationModel configuration, final ChoiceModel referencedSetting, final String id) {
		super(configuration, id, referencedSetting);
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

	@Override
	public boolean isSet() {
		return getSetting().getCurrentOption() == option;
	}

	@Override
	public void enableState() {
		getSetting().enable(option);
	}

	@Override
	public void disableState() {
		getSetting().disable(option);
	}

	@Override
	public void enableOtherStates() {
		ChoiceModel setting;
		
		setting = getSetting();
		
		for(int i = 0; i < setting.getOptionCount(); i++)
			if(i != option)
				setting.enable(i);
	}

	@Override
	public void disableOtherStates() {
		ChoiceModel setting;
		
		setting = getSetting();
		
		for(int i = 0; i < setting.getOptionCount(); i++)
			if(i != option)
				setting.disable(i);
	}
}
