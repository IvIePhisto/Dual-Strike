package dualstrike.configuration;

import java.net.URL;

import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.Unmarshaller;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Result;
import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.w3c.dom.Document;

import dualstrike.configuration.model.Configuration;

public class ConfigurationUtility {
	public final static String ANNOTATE_CONFIGURATION_STYLESHEET = "annotate-configuration.xsl";
	public final static String CONFIGURATION_TO_HEADER_FILE_STYLESHEET = "configuration2header-file.xsl";
	public final static String CONFIGURATION_SCHEMA = "configuration.xsd";
	public final static String ANNOTATED_CONFIGURATION_SCHEMA = "annotated-configuration.xsd";
	
	private final static SchemaFactory SCHEMA_FACTORY = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
	private final static TransformerFactory TRANSFORMER_FACTORY = TransformerFactory.newInstance();
	
	public final static Unmarshaller MODEL_UNMARSHALLER = createUnmarshaller();

	private final static Unmarshaller createUnmarshaller() {
		try {
			URL schemaURL;
			Schema schema;
			JAXBContext context;
			Unmarshaller unmarshaller;
			
			schemaURL = ConfigurationUtility.class.getResource("annotated-configuration.xsd");
			schema = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI).newSchema(schemaURL);
			context = JAXBContext.newInstance(Configuration.class.getPackage().getName());
			unmarshaller = context.createUnmarshaller();
			unmarshaller.setSchema(schema);
			
			return unmarshaller;
		}
		catch(Exception e) {
			throw new Error(e);
		}
	}
	
	public static Configuration unmarshallConfiguration(final String urlString) throws ConfigurationDefinitionException {
		try {
			URL url;
			Configuration configuration;
			
			url = new URL(urlString);
			configuration = (Configuration)MODEL_UNMARSHALLER.unmarshal(url);
			
			return configuration;
		}
		catch(Exception e) {
			throw new ConfigurationDefinitionException(urlString, e);
		}
	}

	private static Transformer createTransformerFromResource(final String resourceName) throws Error {
		try {
			Transformer transformer;
			Source source;
			URL resourceURL;
			
			resourceURL = ConfigurationUtility.class.getResource(resourceName);			
			source = new StreamSource(resourceURL.toExternalForm());
			transformer = TRANSFORMER_FACTORY.newTransformer(source);
			
			return transformer;
		}
		catch(Exception e) {
			throw new Error(e);
		}
	}
	
	private static Schema createSchemaFromResource(final String resourceName) throws Error {
		try {
			Schema schema;
			URL resourceURL;
			
			resourceURL = ConfigurationUtility.class.getResource(resourceName);
			schema = SCHEMA_FACTORY.newSchema(resourceURL);
			
			return schema;
		}
		catch(Exception e) {
			throw new Error(e);
		}
	}
	
	public static Transformer createAnnotateConfigurationTransformer() {
		return createTransformerFromResource(ANNOTATE_CONFIGURATION_STYLESHEET);
	}

	public static Transformer createConfigurationToHeaderFileTransformer() {
		return createTransformerFromResource(CONFIGURATION_TO_HEADER_FILE_STYLESHEET);
	}
	
	public static Schema createConfigurationSchema() {
		return createSchemaFromResource(CONFIGURATION_SCHEMA);
	}

	public static Schema createAnnotatedConfigurationSchema() {
		return createSchemaFromResource(ANNOTATED_CONFIGURATION_SCHEMA);
	}

	public static void main(String[] args) {
		if(args.length != 3 || !(args[0].equals("annotate") || args[0].equals("2header-file"))) {
			System.err.print("args: (annotate|2header-file) <configuration file> <output file>");
			System.exit(1);
		}
		
		try {
			Source source;
			Result result;
			Transformer annotateConfigurationTransformer;
			Schema configurationSchema;

			source = new StreamSource(args[1]);			
			result = new StreamResult(args[2]);
			configurationSchema = createConfigurationSchema();
			configurationSchema.newValidator().validate(source);
			annotateConfigurationTransformer = createAnnotateConfigurationTransformer();
			
			if(args[0].equals("annotate")) {
				annotateConfigurationTransformer.transform(source, result);
			}
			else {
				Transformer toHeaderFileTransformer;
				Schema annotatedConfigurationSchema;
				DOMResult docResult;
				Source docSource;
				Document doc;
				
				doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
				docResult = new DOMResult(doc);
				annotateConfigurationTransformer.transform(source, docResult);
				docSource = new DOMSource(doc);
				annotatedConfigurationSchema = createAnnotatedConfigurationSchema();
				annotatedConfigurationSchema.newValidator().validate(docSource);
				toHeaderFileTransformer = createConfigurationToHeaderFileTransformer();
				toHeaderFileTransformer.transform(docSource, result);
			}
		}
		catch(Throwable t) {
			t.printStackTrace();
			System.exit(2);
		}
	}
}
