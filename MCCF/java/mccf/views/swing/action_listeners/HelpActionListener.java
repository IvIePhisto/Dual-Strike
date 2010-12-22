package mccf.views.swing.action_listeners;

import java.awt.BorderLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JScrollPane;

import mccf.MessageHelper;
import mccf.icons.IconHandler;
import mccf.views.swing.ConfigurationFrameView;
import mccf.views.swing.SwingViewUtility;


public class HelpActionListener implements ActionListener {
	private final ConfigurationFrameView view;
	private final String title;
	private final String message;
	private JFrame helpWindow;
	
	public HelpActionListener(final ConfigurationFrameView view, final String message) {
		this.view = view;
		this.title = view.getMainTitle() + " - " + MessageHelper.get(this, "helpTitle");
		this.message = message;
	}
	
	@Override
	public synchronized void actionPerformed(final ActionEvent actionEvent) {
		if(helpWindow == null) {
			JEditorPane editorPane;
			JScrollPane scrollPane;
			
			editorPane = view.createHTMLPane(message);
			editorPane.setMargin(new Insets(10, 10, 10, 10));
			scrollPane = new JScrollPane(editorPane, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
			helpWindow = new JFrame(title);
			helpWindow.setIconImage(IconHandler.getIcon("help", null, 16, null).getImage());
			helpWindow.getContentPane().add(scrollPane, BorderLayout.CENTER);
			helpWindow.setPreferredSize(view.getWindow().getSize());
			helpWindow.setLocationByPlatform(true);
			helpWindow.pack();
			editorPane.setFont(SwingViewUtility.DESCRIPTION_FONT.deriveFont(16));
			helpWindow.setVisible(true);
		}
		else  {
			if(!helpWindow.isVisible())
				helpWindow.setVisible(true);
			
			helpWindow.requestFocus();
		}
	}
}
