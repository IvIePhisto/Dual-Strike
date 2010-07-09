package dualstrike.configuration.model;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractButton;
import javax.swing.ButtonGroup;
import javax.swing.ButtonModel;

import dualstrike.configuration.definition.ChoiceSetting;
import dualstrike.configuration.file.FileHandler;

public class RadioButtonsChoiceModel extends ChoiceModel  implements ActionListener {
	private final ButtonGroup buttons;
	private final ButtonModel[] buttonModels;
	private final Map<ButtonModel, Integer> buttonModel2Index;
	private ButtonModel currentButtonModel;
	
	RadioButtonsChoiceModel(final FileHandler fileHandler, final ChoiceSetting choiceSetting, final ButtonGroup buttons) {
		super(fileHandler, choiceSetting);
		
		int buttonCount;
		Enumeration<AbstractButton> buttonEnumeration;
		
		buttonCount = buttons.getButtonCount();
		buttonModels = new ButtonModel[buttonCount];
		buttonEnumeration = buttons.getElements();
		buttonModel2Index = new HashMap<ButtonModel, Integer>(buttonCount, 1);
		
		for(int i = 0; i < buttonCount; i++) {
			ButtonModel
			
			buttonModel = buttonEnumeration.nextElement().getModel();
			buttonModels[i] = buttonModel;
			buttonModel2Index.put(buttonModel, i);
			buttonModel.addActionListener(this);
		}
		
		currentButtonModel = buttons.getSelection();
		this.buttons = buttons;
	}
	
	@Override
	public synchronized void actionPerformed(final ActionEvent action) {
		ButtonModel selection;
		
		selection = buttons.getSelection();
		
		if(selection != currentButtonModel) {
			super.setCurrentOption(buttonModel2Index.get(selection));
			currentButtonModel = selection;
			getFileHandler().setModelChanged();
		}
	}

	@Override
	public synchronized void setCurrentOption(final int currentOption) throws IndexOutOfBoundsException {
		if(super.getCurrentOption() != currentOption) {
			super.setCurrentOption(currentOption);
			currentButtonModel = buttonModels[currentOption];
			buttons.setSelected(buttonModels[currentOption], true);
			getFileHandler().setModelChanged();
		}
	}
}