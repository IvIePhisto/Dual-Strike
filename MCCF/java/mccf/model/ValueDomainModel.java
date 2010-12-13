package mccf.model;

import java.util.LinkedList;
import java.util.List;

import mccf.definition.Info;
import mccf.definition.PathInfo;
import mccf.definition.ValueDomain;
import mccf.definition.ValueDomainEntry;

public class ValueDomainModel {
	private final int entryCount;
	private final String id;
	private final List<Info>[] titles;
	private final List<PathInfo>[] helps;
	
	@SuppressWarnings("unchecked")
	public ValueDomainModel(final ConfigurationModel configuration, final ValueDomain valueDomain) {
		List<ValueDomainEntry> entries;
		
		id = valueDomain.getId();
		entries = valueDomain.getEntry();
		entryCount = entries.size();
		titles = (List<Info>[]) new List<?>[entryCount];
		helps = (List<PathInfo>[]) new List<?>[entryCount];
		
		for(int i = 0; i < entryCount; i++) {
			ValueDomainEntry entry;
			
			entry = entries.get(i);
			titles[i] = new LinkedList<Info>(entry.getTitle());
			helps[i] = new LinkedList<PathInfo>(entry.getHelp());
		}
		
		configuration.registerValueDomain(id, this);
	}
	
	public int getEntryCount() {
		return entryCount;
	}
	
	public List<Info> getTitles(final int index) throws IndexOutOfBoundsException {
	 	return titles[index];
	}

	public List<PathInfo> getHelps(final int index) throws IndexOutOfBoundsException {
	 	return helps[index];
	}
}
