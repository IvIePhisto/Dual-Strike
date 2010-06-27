package dualstrike.configuration;

import java.net.URL;

import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.Unmarshaller;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import dualstrike.configuration.model.Configuration;

public class ModelUtility {
	public final static Unmarshaller MODEL_UNMARSHALLER = createUnmarshaller();
	private final static String MODEL_SCHEMA_NAME = "configuration.xsd";
	
	private static Unmarshaller createUnmarshaller() {
		try {
			URL schemaURL;
			Schema schema;
			JAXBContext context;
			Unmarshaller unmarshaller;
			
			schemaURL = ModelUtility.class.getResource(MODEL_SCHEMA_NAME);
			schema = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI).newSchema(schemaURL);
			context = JAXBContext.newInstance(Configuration.class.getPackage().getName() + ".model");
			unmarshaller = context.createUnmarshaller();
			unmarshaller.setSchema(schema);
			
			return unmarshaller;
		}
		catch(Exception e) {
			throw new Error(e);
		}
	}
}
