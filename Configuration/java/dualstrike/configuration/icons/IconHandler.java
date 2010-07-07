package dualstrike.configuration.icons;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.Properties;

import javax.swing.ImageIcon;

public class IconHandler {
	private static final Properties mappings = getMappings(); 
	
	private static Properties getMappings() {
		InputStream mappingsInputStream;
		Properties mappingsProperties;
		
		mappingsProperties = new Properties();
		mappingsInputStream = IconHandler.class.getResourceAsStream("Mappings.properties");
		
		try {
			mappingsProperties.load(mappingsInputStream);
		}
		catch (IOException e) {
			throw new Error(e);
		}
		
		return mappingsProperties;
	}
	
	public static ImageIcon getIcon(final String key, final String description, final int width, Integer height) {
		ImageIcon icon;
		String name;
		String path;
		URL resource;
		
		name = mappings.getProperty(key);
		
		if(name == null)
			name = key;
		
		if(height == null)
			height = width;
		
		path = String.format("%dx%d/%s", width, height, name);
		resource = IconHandler.class.getResource(path);
		
		if(resource == null)
			icon = null;
		else
			icon = new ImageIcon(resource, description);
		
		return icon;
	}
	
	public ImageIcon getIcon(final File file, final String description) {
		if(file.exists())
			return new ImageIcon(file.getPath());
		else
			return null;
	}
}
