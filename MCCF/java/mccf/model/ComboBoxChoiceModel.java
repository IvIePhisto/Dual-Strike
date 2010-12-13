package mccf.model;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComboBox;

import mccf.ItemDisablerListCellRenderer;
import mccf.definition.ChoiceSetting;


public class ComboBoxChoiceModel extends ChoiceModel implements ActionListener {
	private final JComboBox comboBox;
	private final ItemDisablerListCellRenderer listCellRenderer;
	
	ComboBoxChoiceModel(final ConfigurationModel configuration, final ChoiceSetting choiceSetting, final JComboBox comboBox, final ItemDisablerListCellRenderer listCellRenderer) {
		super(configuration, choiceSetting);
		
		this.comboBox = comboBox;
		this.listCellRenderer = listCellRenderer;
		comboBox.addActionListener(this);
	}
	
	@Override
	public synchronized void actionPerformed(ActionEvent e) {
		int selectedIndex;
		
		selectedIndex = comboBox.getSelectedIndex();

		if(listCellRenderer.isItemDisabled(selectedIndex)) {
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
		listCellRenderer.setItemEnabled(option);
	}

	@Override
	synchronized void setDisabled(final int option) {
		listCellRenderer.setItemDisabled(option);
	}

	@Override
	synchronized boolean isDisabled(final int option) {
		return listCellRenderer.isItemDisabled(option);
	}
}