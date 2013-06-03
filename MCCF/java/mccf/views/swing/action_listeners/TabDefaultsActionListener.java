package mccf.views.swing.action_listeners;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;
import java.util.Vector;

import javax.swing.JTabbedPane;

import mccf.MessageHelper;
import mccf.model.PageModel;
import mccf.views.swing.ConfigurationFrameView;


public class TabDefaultsActionListener implements ActionListener {
	private final ConfigurationFrameView view;
	private final List<PageModel> pageModels = new Vector<PageModel>();
	private JTabbedPane tabs;
	
	public TabDefaultsActionListener(final ConfigurationFrameView view) {
		this.view = view;
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
		
		view.getWindow().setEnabled(false);
		selectedIndex = tabs.getSelectedIndex();
		selected = pageModels.get(selectedIndex);
		selected.loadDefaults();
		view.setStatusLabelText(MessageHelper.get(this, "tabDefaultsLoadedStatus"));
		view.getWindow().setEnabled(true);
	}
}
