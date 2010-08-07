package mccf.model;

import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Vector;

import mccf.definition.Configuration;
import mccf.file.FileHandler;


public class ConfigurationModel {
	private final FileHandler fileHandler;
	private final int byteWidth;
	private final List<PageModel> pages = new Vector<PageModel>();
	private final byte device;
	private final byte version;
	private final Map<String, SettingModel> settingsByID = new HashMap<String, SettingModel>();
	private final Set<SettingModel> settingsSet = new HashSet<SettingModel>();
	
	public ConfigurationModel(final FileHandler fileHandler, final Configuration configuration) {
		this.fileHandler = fileHandler;
		this.byteWidth = (int)configuration.getByteWidth();
		this.device = configuration.getDevice();
		this.version = configuration.getVersion();
	}

	public int getByteWidth() {
		return byteWidth;
	}
	
	public synchronized PageModel addPage() {
		PageModel pageModel;
		
		pageModel = new PageModel(this);
		pages.add(pageModel);
		
		return pageModel;
	}
	
	public synchronized void loadDefaults() {
		for(PageModel page: pages)
			page.loadDefaults();
	}
	
	public synchronized void loadBytes(byte[] bytes) throws ByteLoadingException {
		if(bytes.length < byteWidth + 2)
			throw new ByteLoadingException(ByteLoadingException.Type.BYTE_COUNT);
		
		for(int i = 0; i < byteWidth + 2; i++)
			if(bytes[i] == 0xFF)
				throw new ByteLoadingException(ByteLoadingException.Type.UNITITIALIZED_DATA);				
		
		if(bytes[0] != device)
			throw new ByteLoadingException(ByteLoadingException.Type.DEVICE);

		if(bytes[1] != version)
			throw new ByteLoadingException(ByteLoadingException.Type.VERSION);
		
		bytes = Arrays.copyOfRange(bytes, 2, bytes.length);

		for(PageModel page: pages)
			page.loadBytes(bytes);
	}
	
	public synchronized byte[] saveBytes() {
		byte[] bytes;
		
		bytes = new byte[byteWidth + 2];
		
		for(PageModel page: pages)
			page.saveBytes(bytes);
		
		for(int i = 0; i < byteWidth; i++)
			bytes[bytes.length - i - 1] = bytes[bytes.length - i - 3];
		
		bytes[0] = device;
		bytes[1] = version;
		
		return bytes;
	}

	synchronized void registerSetting(final String id, final SettingModel setting) {
		settingsByID.put(id, setting);
		settingsSet.add(setting);
	}

	synchronized SettingModel getSetting(final String id) {
		return settingsByID.get(id);
	}
	
	public synchronized void initConstraints() {
		for(SettingModel setting: settingsSet)
			setting.initConstraints();
	}

	static boolean getBit(final byte[] bytes, final int byteNo, final int bitNo) {
		byte value;
		boolean bit;
		
		value = (byte)(bytes[byteNo] >> bitNo);
		value &= 1;
		bit = value == 1;
		
		return bit;
	}

	FileHandler getFileHandler() {
		return fileHandler;
	}
}
