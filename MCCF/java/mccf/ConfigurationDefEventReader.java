package mccf;

import javax.xml.stream.XMLEventReader;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.events.ProcessingInstruction;
import javax.xml.stream.events.XMLEvent;
import javax.xml.stream.util.EventReaderDelegate;
import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;

class ConfigurationDefEventReader extends EventReaderDelegate {
	private static final XMLInputFactory XML_INPUT_FACTORY = XMLInputFactory.newInstance();
	
	private ConfigurationDefEventReader includingEventReader;
	private final String url;
	protected XMLEvent nextEvent = null;
	
	ConfigurationDefEventReader(String url) throws XMLStreamException {
		super(createEventReader(url));
		this.url = url;
	}
	
	private static XMLEventReader createEventReader(String url) throws XMLStreamException {
		Source source;
		XMLEventReader eventReader;
		
		source = new StreamSource(url);
		eventReader = XML_INPUT_FACTORY.createXMLEventReader(source);
		
		return eventReader;
	}
	
	protected XMLEvent updateNextEvent() throws XMLStreamException {
		XMLEvent event;
		
		event = null;
		
		if(includingEventReader == null) {
			if(super.hasNext()) {
				event = super.nextEvent();
				
				if(event.isProcessingInstruction()) {
					ProcessingInstruction pi;
					
					pi = (ProcessingInstruction)event;
					
					if(pi.getTarget().equals("mcc:include")) {
						includingEventReader = new ConfigurationDefIncludeEventReader(pi.getData());
						event = updateNextEvent();
					}
				}
			}
		}
		else {
			event = includingEventReader.nextEvent();
			
			if(event == null) {
				closeIncludingEventReader();
				event = updateNextEvent();
			}
		}
		
		nextEvent = event;
		
		return event;
	}

	@Override
	public XMLEvent nextEvent() throws XMLStreamException {
		XMLEvent event;

		if(nextEvent == null)
			updateNextEvent();

		event = nextEvent;
		nextEvent = null;
		
		return event;
	}

	@Override
	public boolean hasNext() {
		try {
			if(nextEvent == null)
				updateNextEvent();
			
			return nextEvent != null;
		}
		catch(XMLStreamException e) {
			throw new RuntimeException(e);
		}
	}

	@Override
	public XMLEvent peek() throws XMLStreamException {
		if(nextEvent == null)
			updateNextEvent();
		
		return nextEvent;
	}
	
	private void closeIncludingEventReader() throws XMLStreamException {
		if(includingEventReader != null) {
			includingEventReader.close();
			includingEventReader = null;
		}
	}

	void reset() throws XMLStreamException {
		closeIncludingEventReader();
		nextEvent = null;
		setParent(createEventReader(url));
	}

	@Override
	public void close() throws XMLStreamException {
		closeIncludingEventReader();
		nextEvent = null;
		super.close();
	}
}
