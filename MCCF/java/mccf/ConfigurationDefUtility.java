package mccf;

import java.io.IOException;
import java.net.URL;
import java.util.Locale;

import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Unmarshaller;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.stream.XMLEventReader;
import javax.xml.stream.XMLStreamException;
import javax.xml.transform.Result;
import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stax.StAXSource;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;
import javax.xml.validation.Validator;

import mccf.definition.Configuration;

import org.w3c.dom.Document;
import org.xml.sax.SAXException;


public class ConfigurationDefUtility {
	private final static String ANNOTATE_CONFIGURATION_DEF_STYLESHEET = "annotate-configuration-def.xsl";
	private final static String CONFIGURATION_DEF_TO_HEADER_FILE_STYLESHEET = "configuration-def2header-file.xsl";
	private final static String CONFIGURATION_DEF_SCHEMA_NAME = "configuration-def.xsd";
	private final static String ANNOTATED_CONFIGURATION_DEF_SCHEMA_NAME = "annotated-configuration-def.xsd";
	private final static SchemaFactory SCHEMA_FACTORY = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
	private final static TransformerFactory TRANSFORMER_FACTORY = TransformerFactory.newInstance();
	private final static DocumentBuilderFactory DOCUMENT_BUILDER_FACTORY = DocumentBuilderFactory.newInstance();
	
	static {		
		DOCUMENT_BUILDER_FACTORY.setNamespaceAware(true);
		DOCUMENT_BUILDER_FACTORY.setXIncludeAware(true);
	}
	
	public final static Schema CONFIGURATION_DEF_SCHEMA = createConfigurationDefSchema();
	public final static Schema ANNOTATED_CONFIGURATION_DEF_SCHEMA = createAnnotatedConfigurationDefSchema();
	
	public final static Unmarshaller DEFINITION_UNMARSHALLER = createUnmarshaller();

	private final static Unmarshaller createUnmarshaller() {
		try {
			JAXBContext context;
			Unmarshaller unmarshaller;
			
			context = JAXBContext.newInstance(Configuration.class.getPackage().getName());
			unmarshaller = context.createUnmarshaller();
			unmarshaller.setSchema(ANNOTATED_CONFIGURATION_DEF_SCHEMA);
			
			return unmarshaller;
		}
		catch(Exception e) {
			throw new Error(e);
		}
	}
	
	public static Document annotateConfiguration(final URL url) throws IOException, ConfigurationDefException {
		try {
			DocumentBuilder documentBuilder;
			Source configurationSource;
			Validator configurationValidator;
			Result annotatedConfigurationResult;
			Document annotatedConfigurationDocument;
			Transformer annotateConfigurationTransformer;
			XMLEventReader eventReader;
			
			eventReader = new ConfigurationDefEventReader(url.toExternalForm());
			configurationSource = new StAXSource(eventReader);
			configurationValidator = CONFIGURATION_DEF_SCHEMA.newValidator();
			configurationValidator.validate(configurationSource);
			documentBuilder = DOCUMENT_BUILDER_FACTORY.newDocumentBuilder();
			annotatedConfigurationDocument = documentBuilder.newDocument();
			annotatedConfigurationResult = new DOMResult(annotatedConfigurationDocument);
			annotateConfigurationTransformer = createAnnotateConfigurationDefTransformer();
			annotateConfigurationTransformer.transform(configurationSource, annotatedConfigurationResult);
			
			return annotatedConfigurationDocument;
		}
		catch(ParserConfigurationException e) {
			throw new Error(e);
		}
		catch(TransformerException e) {
			throw new Error(e);
		}
		catch(XMLStreamException e) {
			throw new ConfigurationDefException(url.toExternalForm(), e, Locale.getDefault());
		}
		catch(SAXException e) {
			throw new ConfigurationDefException(url.toExternalForm(), e, Locale.getDefault());
		}
	}
	
	@SuppressWarnings("unchecked")
	public static Configuration unmarshallConfigurationDef(final URL url) throws IOException, ConfigurationDefException {
		try {
			Object configurationObject;
			Configuration configuration;
			Document annotatedConfigurationDocument;
			Source annotatedConfigurationSource;
			JAXBElement<Configuration> jaxbElement;
			
			annotatedConfigurationDocument = annotateConfiguration(url);
			annotatedConfigurationSource = new DOMSource(annotatedConfigurationDocument);
			configurationObject = DEFINITION_UNMARSHALLER.unmarshal(annotatedConfigurationSource);
			jaxbElement = (JAXBElement<Configuration>)configurationObject;
			configuration = jaxbElement.getValue();
			
			return configuration;
		}
		catch(JAXBException e) {
			throw new Error(e);
		}
	}

	private static Transformer createTransformerFromResource(final String resourceName) throws Error {
		try {
			Transformer transformer;
			Source source;
			URL resourceURL;
			
			resourceURL = ConfigurationDefUtility.class.getResource(resourceName);			
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
			
			resourceURL = ConfigurationDefUtility.class.getResource(resourceName);
			schema = SCHEMA_FACTORY.newSchema(resourceURL);
			
			return schema;
		}
		catch(Exception e) {
			throw new Error(e);
		}
	}
	
	public static Transformer createAnnotateConfigurationDefTransformer() {
		return createTransformerFromResource(ANNOTATE_CONFIGURATION_DEF_STYLESHEET);
	}

	public static Transformer createConfigurationDefToHeaderFileTransformer() {
		return createTransformerFromResource(CONFIGURATION_DEF_TO_HEADER_FILE_STYLESHEET);
	}
	
	public static Schema createConfigurationDefSchema() {
		return createSchemaFromResource(CONFIGURATION_DEF_SCHEMA_NAME);
	}

	public static Schema createAnnotatedConfigurationDefSchema() {
		return createSchemaFromResource(ANNOTATED_CONFIGURATION_DEF_SCHEMA_NAME);
	}
}
