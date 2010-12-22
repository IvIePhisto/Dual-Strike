package mccf.views.swing;

import java.awt.Color;
import java.awt.Desktop;
import java.awt.Font;
import java.awt.Image;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;

import mccf.ConfigurationEditor;
import mccf.MessageHelper;
import mccf.icons.IconHandler;

public class SwingViewUtility {

	static final Desktop DESKTOP = Desktop.isDesktopSupported()?Desktop.getDesktop():null;
	static final int FONT_SIZE = 14;
	static final Font TITLE_FONT = new Font(Font.SANS_SERIF, Font.BOLD, FONT_SIZE);
	static final Border BOTTOM_SPACER_BORDER = new EmptyBorder(0, 0, 10, 0);
	static final Color MILK_GLASS = new Color(255, 255, 255, 150);
	public static final Font DESCRIPTION_FONT = new Font(Font.SANS_SERIF, Font.PLAIN, FONT_SIZE);

	public static String readUTF8String(final InputStream inputStream) {
		StringBuilder sb;
		Reader reader;
		
		sb = new StringBuilder();
		
		try {
			int readByte;
			
			reader = new InputStreamReader(new BufferedInputStream(inputStream), "UTF-8");
			
			try {
				while((readByte = reader.read()) != -1) {
					sb.append((char)readByte);
				}
			}
			finally {
				reader.close();
			}
		}
		catch(UnsupportedEncodingException e) {
			throw new Error(e);
		}
		catch(IOException e) {
			throw new Error(e);
		}
		
		return sb.toString();
	}

    public static String convertTextToHTML(String value, final String baseURL) {
		
		if(!value.matches(".*<html>.*</html>.*"))
			value = "<html>" + value + "</html>";
		
		if(baseURL != null && !value.matches("<html>.*<head>.*<base .*/>.*</head>.*</html>.*"))
			value = value.replace("<html>", "<html><head><base href=\"" + baseURL + "\"/></head>");
	
		return value;
	}

	static ImageIcon createFileImageIcon(final String path) {
		try {
			ImageIcon imageIcon;
			File file;
			URL url;
			
			file = new File(path);
			
			if(!file.exists())
				throw new Error(String.format("Image \"%s\" not found.", path));
	
			url = file.toURI().toURL();
			imageIcon = new ImageIcon(url);
			
			return imageIcon;
		}
		catch(MalformedURLException e) {
			throw new Error(e);
		}
	}

	static List<Image> createApplicationImages(final String path) {
		List<Image> images;
		Image currentImage;
		
		images = new LinkedList<Image>();
		
		if(path == null || path.equals("")) {
			currentImage = IconHandler.getIcon("application", null, 64, null).getImage();
			images.add(currentImage);
			currentImage = IconHandler.getIcon("application", null, 22, null).getImage();
			images.add(currentImage);
			currentImage = IconHandler.getIcon("application", null, 16, null).getImage();
			images.add(currentImage);
		}
		else {
			int mainImageFileLastDot;
			int nextImageNumber;
			File currentImageFile;
			File imageFilesDir;
			String mainImageFileName;
			String imageFilesPrefix;
			String imageFilesPostfix;
			
			currentImageFile = new File(path);
			imageFilesDir = currentImageFile.getParentFile();
			mainImageFileName = currentImageFile.getName();
			mainImageFileLastDot = mainImageFileName.lastIndexOf('.');
			imageFilesPrefix = mainImageFileName.substring(0, mainImageFileLastDot);
			imageFilesPostfix = mainImageFileName.substring(mainImageFileLastDot);
			nextImageNumber = 1;
			
			do {
				currentImage = createFileImageIcon(currentImageFile.getAbsolutePath()).getImage();
				images.add(currentImage);
				currentImageFile = new File(imageFilesDir, imageFilesPrefix + "_" + nextImageNumber++ + imageFilesPostfix);
			} while(currentImageFile.exists());
		}
		
		return images;
	}

	static JRadioButton createRadioButton(final String text, final String toolTip, final ButtonGroup buttons, final JPanel panel, final boolean selected) {
		JRadioButton button;
		
		button = new JRadioButton(text);
		button.setFont(DESCRIPTION_FONT);
		button.setSelected(selected);
		
		if(toolTip != null)
			button.setToolTipText(toolTip);
		
		if(selected)
			makeDefault(button);
		
		buttons.add(button);
		panel.add(button);
		
		return button;
	}
	
	static void makeDefault(final JComponent defaultSetting) {
		String tooltip;
		String postfix;
		
		defaultSetting.setFont(defaultSetting.getFont().deriveFont(Font.ITALIC));
		tooltip = defaultSetting.getToolTipText();
		postfix = MessageHelper.get(ConfigurationEditor.class, "default");
		
		if(tooltip == null || tooltip.equals(""))
			tooltip = postfix;
		else {
			Matcher matcher;
			
			matcher = Pattern.compile("(.*<html>.*)(</html>.*)").matcher(tooltip);
			
			if(matcher.matches()) {
				tooltip = matcher.group(1) + " " + postfix + matcher.group(2);
			}
			else
				tooltip += " " + postfix;
		}
		
		defaultSetting.setToolTipText(tooltip);
	}
}
