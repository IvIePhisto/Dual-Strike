package mccf.model;

import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Vector;

import mccf.definition.Configuration;


public class ConfigurationModel {
	private final int byteWidth;
	private final List<PageModel> pages = new Vector<PageModel>();
	private final byte device;
	private final byte version;
	private final Map<String, SettingModel> settingsByID = new HashMap<String, SettingModel>();
	private final Set<SettingModel> settingsSet = new HashSet<SettingModel>();
	
	public ConfigurationModel(final Configuration configuration) {
		this.byteWidth = (int)configuration.getByteWidth();
		this.device = configuration.getDevice();
		this.version = configuration.getVersion();
		// TODO load value domains
	}

	public byte getDevice() {
		return device;
	}

	public byte getVersion() {
		return version;
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
	
	public List<PageModel> getPages() {
		return pages;
	}

	public synchronized void loadDefaults() {
		for(PageModel page: pages)
			page.loadDefaults();
	}

	synchronized void registerSetting(final String id, final SettingModel setting) {
		settingsByID.put(id, setting);
		settingsSet.add(setting);
	}

	public synchronized SettingModel getSetting(final String id) {
		return settingsByID.get(id);
	}
	
	public synchronized void initConstraints() {
		for(SettingModel setting: settingsSet)
			setting.initConstraints();
	}
}
