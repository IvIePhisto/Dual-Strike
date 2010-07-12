package mccf;

import java.awt.Dimension;
import java.awt.Rectangle;

import javax.swing.JPanel;
import javax.swing.Scrollable;

class ResizingJPanel extends JPanel implements Scrollable {
	private static final long serialVersionUID = 1L;

	ResizingJPanel() {
		super();
	}

	@Override
	public Dimension getPreferredScrollableViewportSize() {
		Dimension preferredSize;
		
		preferredSize = getPreferredSize();
		preferredSize.height += 5;
		
		return preferredSize;
	}

	@Override
	public int getScrollableBlockIncrement(Rectangle visibleRect, int orientation, int direction) {
		return 10;
	}

	@Override
	public boolean getScrollableTracksViewportHeight() {
		return false;
	}

	@Override
	public boolean getScrollableTracksViewportWidth() {
		return true;
	}

	@Override
	public int getScrollableUnitIncrement(Rectangle visibleRect, int orientation, int direction) {
		return 1;
	}
}
