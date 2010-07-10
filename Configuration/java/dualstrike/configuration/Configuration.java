package dualstrike.configuration;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;
import java.util.Locale;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.xml.transform.Result;
import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;

import org.w3c.dom.Document;

public class Configuration {
	private enum Mode {
		EDIT,
		GENERATE_HEADER_FILE,
		ANNOTATE,
		HELP;
		
		static Mode newInstance(final String[] args, final int currentIndex) {
			if(args.length == currentIndex || (args.length == currentIndex + 1 || args.length == currentIndex + 2) && (args[currentIndex].equals("-e") || args[currentIndex].equals("--editor")))
				return EDIT;
			
			if((args.length == currentIndex + 2 || args.length == currentIndex + 3) && (args[currentIndex].equals("-ghf") || args[currentIndex].equals("--gen_headerfile")))
				return GENERATE_HEADER_FILE;
			
			if((args.length == currentIndex + 2 || args.length == currentIndex + 3) && (args[currentIndex].equals("-a") || args[currentIndex].equals("--annotate")))
				return ANNOTATE;

			if(args.length == currentIndex + 1 && (args[currentIndex].equals("-h") || args[currentIndex].equals("--help")))
				return HELP;

			return null;
		}
	}
	
	private static final boolean CONSOLE_AVAILABLE = System.console() != null;
	
	public static void main(String[] args) {
		Locale language;
		int currentIndex;
		Mode mode;
		String inPath;
		String outPath;
		
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		}
		catch(UnsupportedLookAndFeelException e) {
			throw new Error(e);
		}
		catch(IllegalAccessException e) {
			throw new Error(e);
		}
		catch(InstantiationException e) {
			throw new Error(e);
		}
		catch(ClassNotFoundException e) {
			throw new Error(e);
		}

		currentIndex = 0;		
		language = Locale.getDefault();
		
		if(args.length != 0 && (args[currentIndex].equals("-l") || args[currentIndex].equals("--language"))) {
			if(args.length < 2)
				showMessage(MessageHelper.get(Configuration.class, "argSyntax", Locale.getDefault()), Locale.getDefault(), true);
			else {
				language = new Locale(args[++currentIndex]);
				currentIndex++;
				Locale.setDefault(language);
			}
		}

		mode = Mode.newInstance(args, currentIndex);
		
		if(mode == null)
			showMessage(MessageHelper.get(Configuration.class, "argSyntax", language), language, true);

		switch(mode) {
		case HELP:
			String message;
			
			message = MessageHelper.get(Configuration.class, "argSyntax", language)
			 		+ MessageHelper.get(Configuration.class, "argHelp", language);
			showMessage(message, language, false);
			
		case EDIT:
			if(currentIndex == args.length)
				inPath = null;
			else
				inPath = args[++currentIndex];
			
			edit(inPath, language);
			
		case ANNOTATE:
		case GENERATE_HEADER_FILE:
			if(currentIndex == args.length + 1) {
				inPath = null;
				outPath = args[++currentIndex];
			}
			else {
				inPath = args[++currentIndex];
				outPath = args[++currentIndex];
			}
			
			transform(inPath, outPath, mode == Mode.ANNOTATE, language);
		}	
	}
	
	private static void edit(final String configurationPath, final Locale language) {
		URL configurationURL;
		
		configurationURL = createConfigurationURL(configurationPath);

		try {
			ConfigurationEditor.newInstance(configurationURL, language);
		}
		catch(ConfigurationDefinitionException e) {
			showMessage(e.getLocalizedMessage(), language, true);
		}
		catch(FileNotFoundException e) {
			showMessage(MessageHelper.get(Configuration.class, "configurationDefinitionNotFound", language, configurationURL), language, true);
		}
		catch(IOException e) {
			showMessage(MessageHelper.get(Configuration.class, "configurationDefinitionLoadingError", language, e.getLocalizedMessage()), language, true);
		}
	}
	
	private static URL createConfigurationURL(String configurationPath) {
		URL configurationURL;
		
		if(configurationPath == null)
			configurationURL = ConfigurationEditor.DEFAULT_CONFIGURATION_DEFINITION_FILE_URL;
		else
			configurationURL = ConfigurationEditor.createFileURL(configurationPath);
		
		return configurationURL;
	}
	
	private static void transform(final String configurationPath, final String destinationPath, final boolean onlyAnnotate, final Locale language) {
		URL configurationURL;

		configurationURL = createConfigurationURL(configurationPath);

		try {
			Source source;
			Result result;
			Transformer annotateConfigurationTransformer;
			Schema configurationSchema;
	
			source = new StreamSource(configurationURL.toExternalForm());			
			result = new StreamResult(destinationPath);
			configurationSchema = ConfigurationUtility.createConfigurationSchema();
			configurationSchema.newValidator().validate(source);
			annotateConfigurationTransformer = ConfigurationUtility.createAnnotateConfigurationTransformer();
			
			if(onlyAnnotate) {
				annotateConfigurationTransformer.transform(source, result);
			}
			else {
				Transformer toHeaderFileTransformer;
				Schema annotatedConfigurationSchema;
				DOMResult docResult;
				Source docSource;
				Document doc;
				
				doc = ConfigurationUtility.DOCUMENT_BUILDER_FACTORY.newDocumentBuilder().newDocument();
				docResult = new DOMResult(doc);
				annotateConfigurationTransformer.transform(source, docResult);
				docSource = new DOMSource(doc);
				annotatedConfigurationSchema = ConfigurationUtility.createAnnotatedConfigurationSchema();
				annotatedConfigurationSchema.newValidator().validate(docSource);
				toHeaderFileTransformer = ConfigurationUtility.createConfigurationToHeaderFileTransformer();
				toHeaderFileTransformer.transform(docSource, result);
			}

			System.exit(0);
		}
		catch(FileNotFoundException e) {
			showMessage(MessageHelper.get(Configuration.class, "configurationDefinitionNotFound", language, configurationURL), language, true);
		}
		catch(Exception e) {
			showMessage(MessageHelper.get(Configuration.class, "generalError", language, e.getLocalizedMessage()), language, true);
		}
	}

	private static void showMessage(String message, Locale language, boolean error) {
		if(CONSOLE_AVAILABLE) {
			if(error)
				System.err.print(message);
			else
				System.out.print(message);
		}
		else {
			JFrame frame;
			String title;
			int messageType;
			String[] options;
			
			
			if(error) {
				messageType = JOptionPane.ERROR_MESSAGE;
				title = MessageHelper.get(Configuration.class, "errorMessageTitle", language);
			}
			else {
				messageType = JOptionPane.INFORMATION_MESSAGE;				
				title = MessageHelper.get(Configuration.class, "infoMessageTitle", language);
			}
			
			options = new String[]{"OK"};
			frame = new JFrame();
			JOptionPane.showOptionDialog(frame, message, title, JOptionPane.OK_OPTION , messageType, null, options, options[0]);
		}

		if(error)
			System.exit(1);
		else
			System.exit(0);
	}

	public static void bla(String[] args) {
		if(args.length != 3 || !(args[0].equals("annotate") || args[0].equals("2header-file"))) {
			System.err.print("args: (annotate|2header-file) <configuration file> <output file>");
			System.exit(1);
		}
		
		try {
		}
		catch(Throwable t) {
			t.printStackTrace();
			System.exit(2);
		}
	}

}
