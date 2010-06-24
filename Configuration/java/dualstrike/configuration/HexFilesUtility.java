package dualstrike.configuration;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.util.LinkedList;
import java.util.List;

public class HexFilesUtility {
	public final static String DEFAULT_ENCODING = "ascii";
	public final static int DEFAULT_MAXIMUM_BYTE_COUNT = 32;
	
	private static enum IntelRecordType {
		DATA,
		EOF;
		
		int getValue() {
			switch(this) {
			case DATA:
				return 0;
			case EOF:
				return 1;
			default:
				throw new RuntimeException();
			}
		}
	}
	
	public static void writeI8HEXFile(final File outputFile, String encoding, final byte[] data, int maximumByteCount) throws IOException {
		PrintStream writer;
		
		if(encoding == null)
			encoding = DEFAULT_ENCODING;
		
		if(maximumByteCount <= 0)
			maximumByteCount = DEFAULT_MAXIMUM_BYTE_COUNT;
		
		writer = new PrintStream(outputFile, encoding);
		
		try {
			for(int address = 0; address < data.length;) {
				int byteCount;
				
				byteCount = data.length - address;
	
				if(byteCount > maximumByteCount)
					byteCount = maximumByteCount;
	
				printI8HEXLine(writer, byteCount, address, IntelRecordType.DATA, data);
				
				if(writer.checkError())
					throw new Error(String.format("An error occured while writing to file \"%s\".", outputFile.getAbsolutePath()));
			}

			printI8HEXLine(writer, 0, 0, IntelRecordType.EOF, data);
		}
		finally {
			writer.close();
		}
	}
	
	private static void printI8HEXLine(PrintStream writer, int byteCount, int address, IntelRecordType recordType, byte[] data) {
		StringBuilder dataString;
		int checksum;
		
		dataString = new StringBuilder();
		checksum = byteCount + address + recordType.getValue();
		
		for(int i = 0; i < byteCount; i++) {
			byte value;
			
			value = data[address + i];
			checksum += value;
			dataString.append(String.format("%x", value));
		}
		
		checksum = -checksum;
			
		writer.printf(":%x%04x%x%s%x\n", byteCount, address, recordType.getValue(), dataString, checksum);
	}
	
	public static byte[] readSimpleHexFile(final File file, String encoding) throws IOException {
		InputStream inputStream;
		
		if(encoding == null)
			encoding = DEFAULT_ENCODING;

		inputStream = new FileInputStream(file);
		
		try {
			BufferedReader reader;
			List<Byte> bytes;
			byte[] bytesArray;
			int i;
			int lineNo;
	
			reader = new BufferedReader(new InputStreamReader(inputStream, encoding));
			bytes = new LinkedList<Byte>();
			lineNo = 0;
			
			while(true) {
				String line;
				
				line = reader.readLine();
				lineNo++;
				
				if(line == null)
					break;
				
				line = line.replaceAll("\\s+", "");
				
				if(!line.matches("[0123456789aAbBcCdDeEfF]*"))
					throw new Error(String.format("Invalid characters (neither whitespace nor HEX value) found in line %d of file \"%s\".", lineNo, file.getAbsolutePath()));
				
				for(int j = 0; j < line.length(); j += 2)
					bytes.add(Byte.parseByte(line.substring(j, j + 2), 16));
			}
			
			bytesArray = new byte[bytes.size()];
			i = 0;
			
			for(byte b: bytes)
				bytesArray[i++] = b;
			
			return bytesArray;
		}
		finally {
			inputStream.close();
		}
	}
	
	public static void main(String[] args) {
		try {
			File input;
			File output;
			byte[] bytes;
			
			input = new File(args[0]);
			output = new File(args[1]);
			bytes = HexFilesUtility.readSimpleHexFile(input, null);
			HexFilesUtility.writeI8HEXFile(output, null, bytes, -1);
		}
		catch(Throwable t) {
			t.printStackTrace();
		}
	}
}
