package mccf.model;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComboBox;

import mccf.ChoiceListCellRenderer;
import mccf.definition.ChoiceSetting;


public class ComboBoxChoiceModel extends ChoiceModel implements ActionListener {
	private final JComboBox comboBox;
	private final ChoiceListCellRenderer listCellRenderer;
	
	ComboBoxChoiceModel(final ConfigurationModel configuration, final ChoiceSetting choiceSetting, final JComboBox comboBox, final ChoiceListCellRenderer listCellRenderer) {
		super(configuration, choiceSetting);
		
		this.comboBox = comboBox;
		this.listCellRenderer = listCellRenderer;
		comboBox.addActionListener(this);
	}
	
	@Override
	public synchronized void actionPerformed(ActionEvent e) {
		int selectedIndex;
		
		selectedIndex = comboBox.getSelectedIndex();

		if(listCellRenderer.getItemDisabled(selectedIndex)) {
			comboBox.setSelectedIndex(getCurrentOption());
		}
		else {
			if(selectedIndex != getCurrentOption()) {
				super.setCurrentOption(selectedIndex);
				getConfiguration().getFileHandler().setModelChanged();
			}
		}
	}

	@Override
	public synchronized void setCurrentOption(final int currentOption) throws IndexOutOfBoundsException {
		if(super.getCurrentOption() != currentOption) {
			super.setCurrentOption(currentOption);
			comboBox.setSelectedIndex(currentOption);
			getConfiguration().getFileHandler().setModelChanged();
		}
	}

	@Override
	synchronized void setEnabled(final int option) {
		listCellRenderer.setItemDisabled(option, false);
	}

	@Override
	synchronized void setDisabled(final int option) {
		listCellRenderer.setItemDisabled(option, true);
	}
}