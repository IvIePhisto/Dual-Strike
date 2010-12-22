package mccf.views.swing;

import java.awt.Color;
import java.awt.Component;

import javax.swing.DefaultListCellRenderer;
import javax.swing.JComponent;
import javax.swing.JList;
import javax.swing.UIManager;

class ItemDisablerListCellRenderer extends DefaultListCellRenderer {
	private static final long serialVersionUID = 1L;
	private static final Color DISABLED_FOREGROUND = UIManager.getColor("Label.disabledForeground");
	private final String[] helps;
	private boolean[] itemDisabledStatus;
	
	ItemDisablerListCellRenderer(final String[] helps) {
		this.helps = helps;
		itemDisabledStatus = new boolean[helps.length];
	}
	
	@Override
	public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
		JComponent component;
		
		component = (JComponent) super.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
		
		if(index >= 0) {
			String help;

			help = helps[index];

			if(help != null)
				component.setToolTipText(help);

			if(itemDisabledStatus[index]) {
				component.setForeground(DISABLED_FOREGROUND);
			}
		}

		return component;
	}
	
	void setItemDisabled(final int index) {
		itemDisabledStatus[index] = true;
	}

	void setItemEnabled(final int index) {
		itemDisabledStatus[index] = false;
	}

	boolean isItemDisabled(final int index) {
		return itemDisabledStatus[index];
	}
}
