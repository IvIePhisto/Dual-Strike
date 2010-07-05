package dualstrike.configuration.model;

import javax.swing.JList;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import dualstrike.configuration.definition.ChoiceSetting;

public class ListChoiceModel extends ChoiceModel implements ListSelectionListener {
	private final JList choiceList;
	
	ListChoiceModel(final ChoiceSetting choiceSetting, final JList choiceList) {
		super(choiceSetting);
		
		this.choiceList = choiceList;
		choiceList.addListSelectionListener(this);
	}
	
	@Override
	public synchronized void valueChanged(final ListSelectionEvent event) {
		if(!event.getValueIsAdjusting() && choiceList.getSelectedIndex() != getCurrentOption()) {
			super.setCurrentOption(choiceList.getSelectedIndex());
		}
	}

	@Override
	public synchronized void setCurrentOption(final int currentOption) throws IndexOutOfBoundsException {
		super.setCurrentOption(currentOption);
		choiceList.setSelectedIndex(currentOption);
	}
}