package mccf;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class HexFilesUtility {
	private final static String ENCODING = "ascii";
	public final static byte DEFAULT_MAXIMUM_BYTE_COUNT = 16;
	
	private static enum IntelRecordType {
		DATA,
		EOF;
		
		byte getValue() {
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
	
	public static void writeI8HexFile(final File outputFile, final byte[] data, byte maximumByteCount) throws IOException {
		PrintStream writer;
				
		if(maximumByteCount <= 0)
			maximumByteCount = DEFAULT_MAXIMUM_BYTE_COUNT;
		
		writer = new PrintStream(outputFile, ENCODING);
		
		try {
			for(int address = 0; address < data.length;) {
				int byteCount;
				
				byteCount = (data.length - address);

				if(byteCount > maximumByteCount)
					byteCount = maximumByteCount;

				printI8HexLine(writer, (byte)byteCount, address, IntelRecordType.DATA, data);
				
				if(writer.checkError())
					throw new IOException(String.format("An error occured while writing to file \"%s\".", outputFile.getAbsolutePath()));
				
				address += byteCount;
			}

			printI8HexLine(writer, (byte)0, 0, IntelRecordType.EOF, data);
		}
		finally {
			writer.close();
		}
	}
	
	private static void printI8HexLine(PrintStream writer, byte byteCount, int address, IntelRecordType recordType, byte[] data) {
		StringBuilder dataString;
		byte checksum;
		
		dataString = new StringBuilder();
		checksum = byteCount;
		checksum += (byte)(address & 0xFF);
		checksum += (byte)((address >> 8) & 0xFF);
		checksum += recordType.getValue();
		
		for(int i = 0; i < byteCount; i++) {
			byte value;
			String valueString;
			
			value = data[address + i];
			checksum = (byte)(checksum + value);
			valueString = String.format("%02X", value);
			dataString.append(valueString);
		}
		
		checksum = (byte)(-checksum);
			
		writer.printf(":%02X%04X%02X%s%02X\n", byteCount, address, recordType.getValue(), dataString, checksum);
	}

	public static byte[] readI8HexFile(final File file) throws IOException {
		InputStream inputStream;

		inputStream = new FileInputStream(file);
		
		try {
			BufferedReader reader;
			Pattern pattern;
			byte[] bytes;
			int lineNo;
			boolean eofReached;
	
			reader = new BufferedReader(new InputStreamReader(inputStream, ENCODING));
			lineNo = 0;
			pattern = Pattern.compile("\\:" +
					"([0123456789aAbBcCdDeEfF][0123456789aAbBcCdDeEfF])" +
					"([0123456789aAbBcCdDeEfF][0123456789aAbBcCdDeEfF][0123456789aAbBcCdDeEfF][0123456789aAbBcCdDeEfF])" +
					"([0123456789aAbBcCdDeEfF][0123456789aAbBcCdDeEfF])" +
					"(([0123456789aAbBcCdDeEfF][0123456789aAbBcCdDeEfF])+)");
			bytes = new byte[0];
			
			while(true) {
				String line;
				String dataWithChecksum;
				Matcher matcher;
				int byteCount;
				int address;
				byte recordType;
				byte checksum;
				int checkValue;
				
				line = reader.readLine();
				lineNo++;
				
				if(line == null)
					break;

				matcher = pattern.matcher(line);
				
				if(!matcher.matches())
					throw new IOException(String.format("Syntax error found in line %d of file \"%s\".", lineNo, file.getAbsolutePath()));
				
				byteCount = Integer.parseInt(matcher.group(1), 16);
				address = Integer.parseInt(matcher.group(2), 16);
				recordType = (byte)Integer.parseInt(matcher.group(3), 16);
				dataWithChecksum = matcher.group(4);
				checksum = (byte)Integer.parseInt(dataWithChecksum.substring(dataWithChecksum.length() - 2, dataWithChecksum.length()), 16);
				
				checkValue = byteCount;
				checkValue += (byte)(address & 0xFF);
				checkValue += (byte)((address >> 8) & 0xFF);
				checkValue += recordType;
				checkValue += checksum;

				if(recordType == 0) { // data record
					if(bytes.length < address + byteCount)
						bytes = Arrays.copyOf(bytes, address + byteCount);
	
					for(int i = 0; i*2 < dataWithChecksum.length() - 2; i += 1) {
						byte b;
						
						b = (byte)Integer.parseInt(dataWithChecksum.substring(i*2, i*2 + 2), 16);
						bytes[address + i] = b;
						checkValue += b;
					}
				}
				else if(recordType == 1) { // EOF record
					if(reader.readLine() != null)
						throw new IOException(String.format("EOF record in line %d of file \"%s\" which is not the end of the file.", lineNo, file.getAbsolutePath()));						
				}
				else {
					throw new IOException(String.format("Unsupported record type in line %d of file \"%s\".", lineNo, file.getAbsolutePath()));
				}
				
				checkValue = checkValue & 0xFF;
				
				if(checkValue != 0)
					throw new IOException(String.format("Checksum error in line %d of file \"%s\".", lineNo, file.getAbsolutePath()));
			}
			
			return bytes;
		}
		finally {
			inputStream.close();
		}
	}
	
	
	public static byte[] readSimpleHexFile(final File file) throws IOException {
		InputStream inputStream;

		inputStream = new FileInputStream(file);
		
		try {
			BufferedReader reader;
			List<Byte> bytes;
			byte[] bytesArray;
			int i;
			int lineNo;
	
			reader = new BufferedReader(new InputStreamReader(inputStream, ENCODING));
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
					throw new IOException(String.format("Invalid characters (neither whitespace nor HEX value) found in line %d of file \"%s\".", lineNo, file.getAbsolutePath()));
				
				for(int j = 0; j < line.length(); j += 2) {
					byte b;
					
					b = (byte)Integer.parseInt(line.substring(j, j + 2), 16);
					bytes.add(b);
				}
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
	
	public static void writeSimpleHexFile(final File outputFile, final byte[] data) throws IOException {
		PrintStream writer;
		
		writer = new PrintStream(outputFile, ENCODING);
		
		try {
			for(int address = 0; address < data.length; address++) {
				writer.printf("%02X ", data[address]);
				
				if((address + 1) % 8 == 0)
					writer.print("\n");
				
				if(writer.checkError())
					throw new IOException(String.format("An error occured while writing to file \"%s\".", outputFile.getAbsolutePath()));
			}
		}
		finally {
			writer.close();
		}
	}
	
	public static void main(String[] args) {
		try {
			File input;
			File output;
			byte[] bytes;
			
			if(args.length < 3 || args.length > 4 || !args[0].matches("\\-plain2intel|\\-intel2plain")) {
				System.err.println("args: (-plain2intel | -intel2plain) <input file> <output file> [size]");
				System.exit(1);
			}
			
			input = new File(args[1]);
			output = new File(args[2]);
			
			if(args[0].equals("-plain2intel")) {
				bytes = HexFilesUtility.readSimpleHexFile(input);
				HexFilesUtility.writeI8HexFile(output, bytes, (byte)-1);
			}
			else {
				bytes = HexFilesUtility.readI8HexFile(input);
				HexFilesUtility.writeSimpleHexFile(output, bytes);
			}
		}
		catch(Throwable t) {
			t.printStackTrace();
		}
	}
}
