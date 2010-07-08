package dualstrike.configuration;

import java.awt.Dimension;
import java.awt.LayoutManager;
import java.awt.Rectangle;

import javax.swing.JPanel;
import javax.swing.Scrollable;
import javax.swing.SwingConstants;

class ResizingJPanel extends JPanel implements Scrollable {
	private static final long serialVersionUID = 1L;

	ResizingJPanel() {
		super();
	}

	public ResizingJPanel(boolean isDoubleBuffered) {
		super(isDoubleBuffered);
	}

	public ResizingJPanel(LayoutManager layout, boolean isDoubleBuffered) {
		super(layout, isDoubleBuffered);
	}

	public ResizingJPanel(LayoutManager layout) {
		super(layout);
	}

	@Override
	public Dimension getPreferredScrollableViewportSize() {
		Dimension preferredSize;
		
		preferredSize = getPreferredSize();
		preferredSize.setSize(preferredSize.getWidth() - 20, preferredSize.getHeight() + 10);
		
		return preferredSize;
	}

	@Override
	public int getScrollableBlockIncrement(Rectangle visibleRect, int orientation, int direction) {
		return ((orientation == SwingConstants.VERTICAL) ? visibleRect.height : visibleRect.width) - 10;
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
