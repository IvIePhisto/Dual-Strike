package dualstrike.configuration.model;

import javax.swing.JList;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import dualstrike.configuration.definition.ChoiceSetting;
import dualstrike.configuration.file.FileHandler;

public class ListChoiceModel extends ChoiceModel implements ListSelectionListener {
	private final JList choiceList;
	
	ListChoiceModel(final FileHandler fileHandler, final ChoiceSetting choiceSetting, final JList choiceList) {
		super(fileHandler, choiceSetting);
		
		this.choiceList = choiceList;
		choiceList.addListSelectionListener(this);
	}
	
	@Override
	public synchronized void valueChanged(final ListSelectionEvent event) {
		if(!event.getValueIsAdjusting() && choiceList.getSelectedIndex() != getCurrentOption()) {
			super.setCurrentOption(choiceList.getSelectedIndex());
			getFileHandler().setModelChanged();
		}
	}

	@Override
	public synchronized void setCurrentOption(final int currentOption) throws IndexOutOfBoundsException {
		if(super.getCurrentOption() != currentOption) {
			super.setCurrentOption(currentOption);
			choiceList.setSelectedIndex(currentOption);
			getFileHandler().setModelChanged();
		}
	}
}