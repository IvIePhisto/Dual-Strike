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
	
	public static void writeI8HEXFile(final File outputFile, final byte[] data, byte maximumByteCount) throws IOException {
		PrintStream writer;
				
		if(maximumByteCount <= 0)
			maximumByteCount = DEFAULT_MAXIMUM_BYTE_COUNT;
		
		writer = new PrintStream(outputFile, ENCODING);
		
		try {
			for(int address = 0; address < data.length;) {
				byte byteCount;
				
				byteCount = (byte)(data.length - address);

				if(byteCount > maximumByteCount)
					byteCount = maximumByteCount;

				printI8HEXLine(writer, byteCount, address, IntelRecordType.DATA, data);
				
				if(writer.checkError())
					throw new Error(String.format("An error occured while writing to file \"%s\".", outputFile.getAbsolutePath()));
				
				address += byteCount;
			}

			printI8HEXLine(writer, (byte)0, 0, IntelRecordType.EOF, data);
		}
		finally {
			writer.close();
		}
	}
	
	private static void printI8HEXLine(PrintStream writer, byte byteCount, int address, IntelRecordType recordType, byte[] data) {
		StringBuilder dataString;
		byte checksum;
		
		dataString = new StringBuilder();
		checksum = byteCount;
		checksum += (byte)(address & 0xFF);
		checksum += (byte)((address >> 8) & 0xFF);
		checksum += recordType.getValue();
		
		for(int i = 0; i < byteCount; i++) {
			byte value;
			
			value = data[address + i];
			checksum = (byte)(checksum + value);
			dataString.append(String.format("%02X", value));
		}
		
		checksum = (byte)(-checksum);
			
		writer.printf(":%02X%04X%02X%s%02X\n", byteCount, address, recordType.getValue(), dataString, checksum);
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
					throw new Error(String.format("Invalid characters (neither whitespace nor HEX value) found in line %d of file \"%s\".", lineNo, file.getAbsolutePath()));
				
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
	
	public static void main(String[] args) {
		try {
			File input;
			File output;
			byte[] bytes;
			
			input = new File(args[0]);
			output = new File(args[1]);
			bytes = HexFilesUtility.readSimpleHexFile(input);
			HexFilesUtility.writeI8HEXFile(output, bytes, (byte)-1);
		}
		catch(Throwable t) {
			t.printStackTrace();
		}
	}
}
