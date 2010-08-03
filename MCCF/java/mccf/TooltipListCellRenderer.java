package mccf;

import java.awt.Component;

import javax.swing.DefaultListCellRenderer;
import javax.swing.JComponent;
import javax.swing.JList;

class TooltipListCellRenderer extends DefaultListCellRenderer {
	private static final long serialVersionUID = 1L;
	private final String[] helps;
	
	TooltipListCellRenderer(final String[] helps) {
		this.helps = helps;
	}

	public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
		JComponent component;
		
		component = (JComponent) super.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
		
		if(index > 0) {
			String help;

			help = helps[index];

			if(help != null)
				component.setToolTipText(help);
		}

		return component;
	}
}
