package mccf.model;

class BytesUtility {
	static int loadIntBytes(final byte[] bytes, final int[] byteDividers, final int byteNo, final int bitNo, final int bitWidth) {
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
			
			bit = ConfigurationModel.getBit(bytes, currentByte, currentBitInByte);
			
			if(bit)
				value += 1<<currentBit;
		}
		
		return value;
	}

	static void saveBytes(final int value, final byte[] bytes, final int[] byteDividers, final int byteNo, final int bitNo, final int bitWidth) {
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
