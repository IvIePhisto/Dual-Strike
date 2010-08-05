package mccf.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;
import java.util.Vector;

import javax.swing.JTabbedPane;

import mccf.ConfigurationEditor;
import mccf.MessageHelper;
import mccf.model.PageModel;


public class TabDefaultsActionListener implements ActionListener {
	private final ConfigurationEditor controller;
	private final List<PageModel> pageModels = new Vector<PageModel>();
	private JTabbedPane tabs;
	
	public TabDefaultsActionListener(final ConfigurationEditor controller) {
		this.controller = controller;
	}
	
	public void setTabs(JTabbedPane tabs) {
		this.tabs = tabs;
	}

	public synchronized void addPageModel(final PageModel page) {
		pageModels.add(page);
	}

	@Override
	public synchronized void actionPerformed(final ActionEvent action) {
		int selectedIndex;
		PageModel selected;
		
		controller.getWindow().setEnabled(false);
		selectedIndex = tabs.getSelectedIndex();
		selected = pageModels.get(selectedIndex);
		selected.loadDefaults();
		controller.setStatusLabelText(MessageHelper.get(this, "tabDefaultsLoadedStatus"));
		controller.getWindow().setEnabled(true);
	}
}
