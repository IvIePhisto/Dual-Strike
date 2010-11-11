package mccf;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.events.XMLEvent;

class ConfigurationDefIncludeEventReader extends ConfigurationDefEventReader {
	private int elementDepth = 0;
	private List<XMLEvent> namespaceBackup = new LinkedList<XMLEvent>();
	private Iterator<XMLEvent> namespaceIterator = null;

	ConfigurationDefIncludeEventReader(String url) throws XMLStreamException {
		super(url);
	}
	
	@Override
	protected XMLEvent updateNextEvent() throws XMLStreamException {
		XMLEvent event;

		event = null;
		
		if(namespaceIterator != null) {			
			if(namespaceIterator.hasNext())
				event = namespaceIterator.next();
			else
				namespaceIterator = null;
		}
		
		if(event == null) {
			if(elementDepth == 2 && namespaceBackup.size() > 0) {
				namespaceIterator = namespaceBackup.iterator();
				event = updateNextEvent();
			}
			else {
				event = super.updateNextEvent();
				
				switch(event.getEventType()) {
				case XMLEvent.START_DOCUMENT:
				case XMLEvent.DTD:
				case XMLEvent.ENTITY_DECLARATION:
				case XMLEvent.NOTATION_DECLARATION:
					event = updateNextEvent();
					break;
		
				case XMLEvent.END_DOCUMENT:
					event = null;
					break;
		
				case XMLEvent.START_ELEMENT:
					elementDepth++;
		
					if(elementDepth == 1)
						event = updateNextEvent();
		
					break;
					
				case XMLEvent.END_ELEMENT:
					elementDepth--;
		
					if(elementDepth == 0)
						event = updateNextEvent();
					
					break;
					
				case XMLEvent.ATTRIBUTE:
					if(elementDepth == 1)
						event = updateNextEvent();
					
					break;
					
				case XMLEvent.NAMESPACE:
					if(elementDepth == 1) {
						namespaceBackup.add(event);
						event = updateNextEvent();
					}
					
					break;
				}
			}
		}
		
		nextEvent = event;
		
		return event;
	}
}
