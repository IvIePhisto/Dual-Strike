package mccf.views.swing;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Locale;

import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JPanel;
import javax.swing.JRadioButton;

import mccf.ConfigurationEditor;
import mccf.MessageHelper;
import mccf.definition.BooleanSetting;
import mccf.model.BooleanModel;
import mccf.model.PageModel;
import mccf.model.SettingChangeListener;
import mccf.model.SettingModel;

class BooleanRadioButtonsView implements SettingChangeListener, ActionListener {
	static JPanel createPanel(final BooleanSetting setting, final PageModel pageModel, final Locale language) {
		JPanel selectorPanel;
		ButtonGroup buttons;
		JRadioButton enableButton;
		JRadioButton disableButton;
		BooleanModel model;
		boolean isEnabled;
	
		buttons = new ButtonGroup();
		selectorPanel = new JPanel();
		selectorPanel.setLayout(new BoxLayout(selectorPanel, BoxLayout.Y_AXIS));
		selectorPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		isEnabled = setting.isDefault();
		enableButton = SwingViewUtility.createRadioButton(MessageHelper.get(ConfigurationEditor.class, "trueButtonText", language), null, buttons, selectorPanel, isEnabled);
		disableButton = SwingViewUtility.createRadioButton(MessageHelper.get(ConfigurationEditor.class, "falseButtonText", language), null, buttons, selectorPanel, !isEnabled);
		model = pageModel.createBoolean(setting);
		new BooleanRadioButtonsView(model, enableButton, disableButton);
		
		return selectorPanel;
	}

	private final JRadioButton enableButton;
	//final JRadioButton disableButton;
	private final BooleanModel setting;

	private BooleanRadioButtonsView(final BooleanModel setting, final JRadioButton enableButton, final JRadioButton disableButton) {
		this.enableButton = enableButton;
		//this.disableButton = disableButton;
		this.setting = setting;
		
		enableButton.setEnabled(setting.getValue());
		//disableButton.setEnabled(!setting.getValue());
		setting.registerChangeListener(this);
		enableButton.addActionListener(this);
		disableButton.addActionListener(this);
	}

	@Override
	public synchronized void settingChanged(SettingModel setting) {
		if(setting == this.setting) {
			enableButton.setEnabled(this.setting.getValue());
			//disableButton.setEnabled(!this.setting.getValue());			
		}
		else throw new UnsupportedOperationException("Change notification of wrong setting.");
	}

	@Override
	public synchronized void actionPerformed(ActionEvent event) {
		if(enableButton.isSelected() != setting.getValue())
			setting.setValue(enableButton.isSelected());
	}
}
