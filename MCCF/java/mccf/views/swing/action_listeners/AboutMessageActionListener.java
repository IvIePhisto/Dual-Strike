package mccf.views.swing.action_listeners;

import java.awt.BorderLayout;
import java.awt.Dialog.ModalityType;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.InputStream;
import java.text.MessageFormat;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JEditorPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import mccf.ConfigurationEditor;
import mccf.MessageHelper;
import mccf.views.swing.ConfigurationFrameView;
import mccf.views.swing.SwingViewUtility;


public class AboutMessageActionListener implements ActionListener {
	private final ConfigurationFrameView view;
	private final String title;
	private final String message;
	
	public AboutMessageActionListener(final ConfigurationFrameView view) {
		this.view = view;
		this.title = MessageHelper.get(this, "aboutTitle");
		this.message = MessageFormat.format(readResource(MessageHelper.get(this, "aboutMessagePath")), ConfigurationEditor.MAJOR_VERSION_NO, ConfigurationEditor.MINOR_VERSION_NO, ConfigurationEditor.BUGFIX_VERSION_NO);
	}
	
	private static String readResource(final String path) {
		InputStream inputStream;
		
		inputStream = AboutMessageActionListener.class.getResourceAsStream(path);
		
		return SwingViewUtility.readUTF8String(inputStream);
	}
	
	@Override
	public synchronized void actionPerformed(final ActionEvent actionEvent) {
		final JDialog dialog;
		JEditorPane editorPane;
		JScrollPane scrollPane;
		JPanel buttonPanel;
		Point windowLocation;
		Dimension windowSize;
		Dimension dialogSize;
		int x;
		int y;
		JButton button;
		
		view.makeWindowInactive();
		dialog = new JDialog(view.getWindow(), title, ModalityType.APPLICATION_MODAL);
		editorPane = view.createHTMLPane(message);
		editorPane.setPreferredSize(new Dimension(640, 400));
		editorPane.setMargin(new Insets(10, 10, 10, 10));
		editorPane.setFont(SwingViewUtility.DESCRIPTION_FONT);
		editorPane.setBackground(dialog.getBackground());
		scrollPane = new JScrollPane(editorPane, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		scrollPane.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		dialog.add(scrollPane, BorderLayout.CENTER);
		button = new JButton(MessageHelper.get(this, "okButtonTitle"));
		button.addActionListener(
				new ActionListener() {
					@Override
					public void actionPerformed(ActionEvent arg0) {
						dialog.dispose();
					}
				}
		);
		buttonPanel = new JPanel();
		buttonPanel.add(button);
		dialog.add(buttonPanel, BorderLayout.PAGE_END);
		dialog.pack();
		windowLocation = view.getWindow().getLocation();
		dialogSize = dialog.getSize();
		windowSize = view.getWindow().getSize();
		x = windowLocation.x + (windowSize.width - dialogSize.width)/2;
		y = windowLocation.y + (windowSize.height - dialogSize.height)/2;
		dialog.setLocation(x, y);
		dialog.setVisible(true);
		view.makeWindowActive();
	}
}
