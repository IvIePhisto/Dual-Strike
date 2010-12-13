package mccf.model;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComboBox;

import mccf.ItemDisablerListCellRenderer;
import mccf.definition.ValueSetting;


public class ComboBoxValueModel extends SettingModel implements ActionListener {
	private final int[] byteDividers;
	private final int bitWidth;
	private final JComboBox comboBox;
	private final ItemDisablerListCellRenderer listCellRenderer;
	private final int defaultValue;
	private int currentValue;

	ComboBoxValueModel(final ConfigurationModel configuration, final ValueSetting valueSetting, final JComboBox comboBox, final ItemDisablerListCellRenderer listCellRenderer) {
		super(configuration, valueSetting.getByteNo(), valueSetting.getBitNo());
		
		int i;
		
		this.comboBox = comboBox;
		this.listCellRenderer = listCellRenderer;
		defaultValue = (int)valueSetting.getDefault();
		bitWidth = (int)valueSetting.getBitWidth();
		
		i = 0;
		byteDividers = new int[valueSetting.getByteDividers().size()];
		
		for(Long byteDivider: valueSetting.getByteDividers())
			byteDividers[i++] = byteDivider.intValue();
		
		comboBox.addActionListener(this);
	}
	
	@Override
	public synchronized void actionPerformed(ActionEvent e) {
		int selectedIndex;
		
		selectedIndex = comboBox.getSelectedIndex();

		if(listCellRenderer.isItemDisabled(selectedIndex)) {
			comboBox.setSelectedIndex(currentValue);
		}
		else {
			if(selectedIndex != currentValue) {
				setCurrentValue(selectedIndex);
				getConfiguration().getFileHandler().setModelChanged();
			}
		}
	}

	public synchronized void setCurrentValue(final int currentValue) throws IndexOutOfBoundsException {
		if(this.currentValue != currentValue) {
			this.currentValue = currentValue;
			comboBox.setSelectedIndex(currentValue);
			getConfiguration().getFileHandler().setModelChanged();
		}
	}

	@Override
	void loadDefaults() {
		setCurrentValue(defaultValue);
	}

	@Override
	synchronized void loadBytes(byte[] bytes) {
		setCurrentValue(BytesUtility.loadIntBytes(bytes, byteDividers, getByteNo(), getBitNo(), bitWidth));
	}

	@Override
	synchronized void saveBytes(byte[] bytes) {
		BytesUtility.saveBytes(currentValue, bytes, byteDividers, getByteNo(), getBitNo(), bitWidth);
	}


	@Override
	void initConstraints() {
		// TODO Auto-generated method stub
		
	}

	@Override
	void addRequiredBy(String source, String requirement) {
		// TODO Auto-generated method stub
		
	}
}