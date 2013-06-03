package mccf.views.swing;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractButton;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.ButtonModel;
import javax.swing.JPanel;

import mccf.definition.ChoiceSetting;
import mccf.definition.Option;
import mccf.model.ChoiceModel;
import mccf.model.PageModel;
import mccf.model.SettingChangeListener;
import mccf.model.SettingModel;

public class ChoiceRadioButtonsView implements SettingChangeListener, ActionListener {
	static JPanel createPanel(final ConfigurationFrameView parentView, final ChoiceSetting setting, final String defaultOptionID, final PageModel pageModel) {
		ButtonGroup buttons;
		JPanel panel;
		ChoiceModel model;
	
		buttons = new ButtonGroup();
		panel = new JPanel();
		panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
		panel.setAlignmentX(Component.LEFT_ALIGNMENT);
	
		for(Option o: setting.getOption()) {
			boolean selected;
			String title;
			String help;
			
			selected = o.getId().equals(defaultOptionID);
			title = parentView.extractLocalizedInfo(o.getTitle(), true);
			help = parentView.extractLocalizedInfo(o.getHelp(), true);
			SwingViewUtility.createRadioButton(title, help, buttons, panel, selected);
		}
		
		model = pageModel.createChoice(setting);
		new ChoiceRadioButtonsView(model, buttons);
	
		return panel;
	}

	private final ChoiceModel setting;
	private final ButtonGroup buttons;
	private final ButtonModel[] buttonModels;
	private final Map<ButtonModel, Integer> buttonModel2Index;
	private ButtonModel currentButtonModel;

	private ChoiceRadioButtonsView(final ChoiceModel setting, final ButtonGroup buttons) {
		this.setting = setting;
		this.buttons = buttons;
		
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
		setting.registerChangeListener(this);
	}

	@Override
	public synchronized void actionPerformed(ActionEvent e) {
		ButtonModel selection;
		
		selection = buttons.getSelection();
		
		if(selection != currentButtonModel) {
			setting.setCurrentOption(buttonModel2Index.get(selection));
			currentButtonModel = selection;
		}
	}

	@Override
	public synchronized void settingChanged(SettingModel setting) {
		if(setting == this.setting) {
			currentButtonModel = buttonModels[this.setting.getCurrentOption()];
			buttons.setSelected(currentButtonModel, true);	
		}
		else throw new UnsupportedOperationException("Change notification of wrong setting.");
	}
}
