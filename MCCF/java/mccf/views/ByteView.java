package mccf.views;

import java.util.Arrays;

import mccf.model.BooleanModel;
import mccf.model.ChoiceModel;
import mccf.model.ConfigurationModel;
import mccf.model.PageModel;
import mccf.model.SettingModel;

public class ByteView {
	public static void loadBytes(final ConfigurationModel configuration, byte[] bytes) throws ByteLoadingException {
		if(bytes.length < configuration.getByteWidth() + 2)
			throw new ByteLoadingException(ByteLoadingException.Type.BYTE_COUNT);
		
		for(int i = 0; i < configuration.getByteWidth() + 2; i++)
			if(bytes[i] == 0xFF)
				throw new ByteLoadingException(ByteLoadingException.Type.UNITITIALIZED_DATA);				
		
		if(bytes[0] != configuration.getDevice())
			throw new ByteLoadingException(ByteLoadingException.Type.DEVICE);

		if(bytes[1] != configuration.getVersion())
			throw new ByteLoadingException(ByteLoadingException.Type.VERSION);
		
		bytes = Arrays.copyOfRange(bytes, 2, bytes.length);

		for(PageModel page: configuration.getPages())
			for(SettingModel settingModel: page.getSettings()) {
				if(settingModel instanceof BooleanModel) {
					BooleanModel setting;
					
					setting = (BooleanModel)settingModel;
					setting.setValue(getBit(bytes, setting.getByteNo(), setting.getBitNo()));
				}
				else if(settingModel instanceof ChoiceModel) {
					ChoiceModel setting;
					
					setting = (ChoiceModel)settingModel;
					setting.setCurrentOption(ByteView.loadIntBytes(bytes, setting.getByteDividers(), setting.getByteNo(), setting.getBitNo(), setting.getBitWidth()));
				}
				else
					throw new UnsupportedOperationException("Unknown setting type");
			}
	}
	
	public static byte[] saveBytes(final ConfigurationModel configuration) {
		byte[] bytes;
		
		bytes = new byte[configuration.getByteWidth() + 2];
		
		for(PageModel page: configuration.getPages())
			for(SettingModel settingModel: page.getSettings()) {
				if(settingModel instanceof BooleanModel) {
					BooleanModel setting;
					
					setting = (BooleanModel)settingModel;

					if(setting.getValue())
						bytes[setting.getByteNo()] |= (1 << setting.getBitNo());
				}
				else if(settingModel instanceof ChoiceModel) {
					ChoiceModel setting;
					
					setting = (ChoiceModel)settingModel;
					ByteView.saveBytes(setting.getCurrentOption(), bytes, setting.getByteDividers(), setting.getByteNo(), setting.getBitNo(), setting.getBitWidth());
				}
				else
					throw new UnsupportedOperationException("Unknown setting type");

			}

		for(int i = 0; i < configuration.getByteWidth(); i++)
			bytes[bytes.length - i - 1] = bytes[bytes.length - i - 3];
		
		bytes[0] = configuration.getDevice();
		bytes[1] = configuration.getVersion();
		
		return bytes;
	}

	private static boolean getBit(final byte[] bytes, final int byteNo, final int bitNo) {
		byte value;
		boolean bit;
		
		value = (byte)(bytes[byteNo] >> bitNo);
		value &= 1;
		bit = value == 1;
		
		return bit;
	}

	private static int loadIntBytes(final byte[] bytes, final int[] byteDividers, final int byteNo, final int bitNo, final int bitWidth) {
		int currentByteDivider;
		int nextByteDividerIndex;
		int currentByte;
		int value;
		
		nextByteDividerIndex = 0;
		
		if(byteDividers.length == 0)
			currentByteDivider = Integer.MAX_VALUE;
		else
			currentByteDivider = byteDividers[nextByteDividerIndex++];
		
		currentByte = byteNo;
		value = 0;
		
		for(int currentBit = 0, currentBitInByte = bitNo; currentBit < bitWidth; currentBit++, currentBitInByte++) {
			boolean bit;
			
			if(currentBit == currentByteDivider) {
				currentByte++;
	
				currentBitInByte = 0;
				
				if(nextByteDividerIndex < byteDividers.length)
					currentByteDivider = byteDividers[nextByteDividerIndex++];
				else
					currentByteDivider = Integer.MAX_VALUE;
			}
			
			bit = getBit(bytes, currentByte, currentBitInByte);
			
			if(bit)
				value += 1<<currentBit;
		}
		
		return value;
	}

	private static void saveBytes(final int value, final byte[] bytes, final int[] byteDividers, final int byteNo, final int bitNo, final int bitWidth) {
		int currentByteDivider;
		int nextByteDividerIndex;
		int currentByte;
		
		nextByteDividerIndex = 0;
		
		if(byteDividers.length == 0)
			currentByteDivider = Integer.MAX_VALUE;
		else
			currentByteDivider = byteDividers[nextByteDividerIndex++];
		
		currentByte = byteNo;
		
		for(int currentBit = 0, currentBitInByte = bitNo; currentBit < bitWidth; currentBit++, currentBitInByte++) {
			int optionIndex;
						
			if(currentBit == currentByteDivider) {
				currentByte++;
				
				currentBitInByte = 0;
				
				if(nextByteDividerIndex < byteDividers.length)
					currentByteDivider = byteDividers[nextByteDividerIndex++];
				else
					currentByteDivider = Integer.MAX_VALUE;
			}
	
			optionIndex = bitWidth - currentBit - 1;
			
			if(((value >> optionIndex) & 1) == 1)
				bytes[currentByte] |= 1 << currentBitInByte;
		}
	}
}
