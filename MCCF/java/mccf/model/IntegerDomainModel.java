package mccf.model;

import java.util.LinkedList;
import java.util.List;

import mccf.definition.Info;
import mccf.definition.IntegerDomain;
import mccf.definition.IntegerDomainEntry;
import mccf.definition.PathInfo;

public class IntegerDomainModel {
	private final int entryCount;
	private final List<Info>[] titles;
	private final List<PathInfo>[] helps;
	
	@SuppressWarnings("unchecked")
	public IntegerDomainModel(final IntegerDomain integerDomain) {
		List<IntegerDomainEntry> entries;
		
		entries = integerDomain.getEntry();
		entryCount = entries.size();
		titles = (List<Info>[]) new List<?>[entryCount];
		helps = (List<PathInfo>[]) new List<?>[entryCount];
		
		for(int i = 0; i < entryCount; i++) {
			IntegerDomainEntry entry;
			
			entry = entries.get(i);
			titles[i] = new LinkedList<Info>(entry.getTitle());
			helps[i] = new LinkedList<PathInfo>(entry.getHelp());
		}
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
