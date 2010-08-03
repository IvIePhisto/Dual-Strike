package mccf.model;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComboBox;

import mccf.definition.ChoiceSetting;
import mccf.file.FileHandler;


public class ComboBoxChoiceModel extends ChoiceModel implements ActionListener {
	private final JComboBox comboBox;
	
	ComboBoxChoiceModel(final FileHandler fileHandler, final ChoiceSetting choiceSetting, final JComboBox comboBox) {
		super(fileHandler, choiceSetting);
		
		this.comboBox = comboBox;
		comboBox.addActionListener(this);
	}
	
	@Override
	public synchronized void actionPerformed(ActionEvent e) {
		int selectedIndex;
		
		selectedIndex = comboBox.getSelectedIndex();
		
		if(selectedIndex != getCurrentOption()) {
			super.setCurrentOption(selectedIndex);
			getFileHandler().setModelChanged();
		}
	}

	@Override
	public synchronized void setCurrentOption(final int currentOption) throws IndexOutOfBoundsException {
		if(super.getCurrentOption() != currentOption) {
			super.setCurrentOption(currentOption);
			comboBox.setSelectedIndex(currentOption);
			getFileHandler().setModelChanged();
		}
	}
}