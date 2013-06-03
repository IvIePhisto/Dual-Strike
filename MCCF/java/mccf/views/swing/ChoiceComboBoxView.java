package mccf.views.swing;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.LinkedList;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.JComboBox;
import javax.swing.JPanel;

import mccf.MessageHelper;
import mccf.definition.ChoiceSetting;
import mccf.definition.Option;
import mccf.model.ChoiceModel;
import mccf.model.PageModel;
import mccf.model.SettingChangeListener;
import mccf.model.SettingModel;

class ChoiceComboBoxView implements SettingChangeListener, ActionListener {
	static JPanel createPanel(final ConfigurationFrameView parentView, final ChoiceSetting setting, final String defaultOptionID, final PageModel pageModel) {
		List<String> titles;
		List<String> helps;
		final String[] helpsArray;
		JComboBox comboBox;
		int selectedIndex;
		ItemDisablerListCellRenderer listCellRenderer;
		JPanel panel;
		ChoiceModel model;
		
		titles = new LinkedList<String>();
		helps = new LinkedList<String>();
		selectedIndex = -1;
		
		for(Option o: setting.getOption()) {
			String help;
			
			help = parentView.extractLocalizedInfo(o.getHelp(), false);
	
			if(o.getId().equals(defaultOptionID)) {
				String defaultPostfix;
				
				selectedIndex = titles.size();
				defaultPostfix = MessageHelper.get(parentView, "default");
				
				if(help == null || help.equals(""))
					help = defaultPostfix;
				else
					help += " " + defaultPostfix;
			}
	
			titles.add(parentView.extractLocalizedInfo(o.getTitle(), true));
			
			if(help == null)
				helps.add(null);
			else
				helps.add(SwingViewUtility.convertTextToHTML(help, null));
		}
		
		helpsArray = helps.toArray(new String[]{});
		listCellRenderer = new ItemDisablerListCellRenderer(helpsArray);
		comboBox = new JComboBox(titles.toArray());
		comboBox.setEditable(true);
		comboBox.setRenderer(listCellRenderer);
		comboBox.setSelectedIndex(selectedIndex);
		comboBox.setBorder(BorderFactory.createEmptyBorder(2, 2, 2, 2));
		panel = new JPanel();
		panel.add(comboBox);
		
		model = pageModel.createChoice(setting);
		new ChoiceComboBoxView(model, comboBox, listCellRenderer);
	
		return panel;
	}

	private final ChoiceModel setting;
	private final JComboBox comboBox;
	private final ItemDisablerListCellRenderer listCellRenderer;

	private ChoiceComboBoxView(final ChoiceModel setting, final JComboBox comboBox, final ItemDisablerListCellRenderer listCellRenderer) {
		this.setting = setting;
		this.comboBox = comboBox;
		this.listCellRenderer = listCellRenderer;
		comboBox.addActionListener(this);
		setting.registerChangeListener(this);
	}

	@Override
	public synchronized void actionPerformed(ActionEvent arg0) {
		int selectedIndex;
		
		selectedIndex = comboBox.getSelectedIndex();

		if(listCellRenderer.isItemDisabled(selectedIndex)) {
			comboBox.setSelectedIndex(setting.getCurrentOption());
		}
		else {
			if(selectedIndex != setting.getCurrentOption()) {
				setting.setCurrentOption(selectedIndex);
			}
		}
	}

	@Override
	public synchronized void settingChanged(SettingModel setting) {
		if(setting == this.setting)
			comboBox.setSelectedIndex(this.setting.getCurrentOption());
		else
			throw new UnsupportedOperationException("Change notification of wrong setting.");
	}
}
