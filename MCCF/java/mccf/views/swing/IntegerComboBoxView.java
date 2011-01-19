package mccf.views.swing;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.LinkedList;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.JComboBox;
import javax.swing.JPanel;

import mccf.MessageHelper;
import mccf.definition.IntegerSetting;
import mccf.model.IntegerDomainModel;
import mccf.model.IntegerModel;
import mccf.model.PageModel;
import mccf.model.SettingChangeListener;
import mccf.model.SettingModel;

public class IntegerComboBoxView implements SettingChangeListener, ActionListener {
	static JPanel createPanel(final ConfigurationFrameView parentView, final IntegerSetting setting, final String defaultOptionID, final PageModel pageModel) {
		List<String> titles;
		List<String> helps;
		final String[] helpsArray;
		JComboBox comboBox;
		int selectedIndex;
		ItemDisablerListCellRenderer listCellRenderer;
		JPanel panel;
		IntegerModel model;
		IntegerDomainModel domain;
		
		titles = new LinkedList<String>();
		helps = new LinkedList<String>();
		selectedIndex = -1;
		model = pageModel.createValue(setting);
		domain = model.getValueDomain();
		
		for(int i = 0; i < domain.getEntryCount(); i++) {
			String help; 
			
			help = parentView.extractLocalizedInfo(domain.getHelps(i), false);
			
			if(i == model.getDefaultValue()) {
				String defaultPostfix;
				
				selectedIndex = titles.size();
				defaultPostfix = MessageHelper.get(parentView, "default");
				
				if(help == null || help.equals(""))
					help = defaultPostfix;
				else
					help += " " + defaultPostfix;
			}
			
			titles.add(parentView.extractLocalizedInfo(domain.getTitles(i), true));
			
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
		
		new IntegerComboBoxView(model, comboBox, listCellRenderer);
	
		return panel;
	}

	private final IntegerModel setting;
	private final JComboBox comboBox;
	private final ItemDisablerListCellRenderer listCellRenderer;

	private IntegerComboBoxView(final IntegerModel setting, final JComboBox comboBox, final ItemDisablerListCellRenderer listCellRenderer) {
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
			comboBox.setSelectedIndex(setting.getValue());
		}
		else {
			if(selectedIndex != setting.getValue()) {
				setting.setValue(selectedIndex);
			}
		}
	}

	@Override
	public synchronized void settingChanged(SettingModel setting) {
		if(setting == this.setting)
			comboBox.setSelectedIndex(this.setting.getValue());
		else
			throw new UnsupportedOperationException("Change notification of wrong setting.");
	}
}
