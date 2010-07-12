package mccf.device;

import mccf.MessageHelper;

public enum ReturnCode {
	FLASH_AND_EEPROM_PROGRAMMING_AVAILABLE,
	EEPROM_PROGRAMMING_AVAILABLE,
	FLASH_PROGRAMMING_AVAILABLE,
	COMPLETED_SUCCESSFULLY,
	WRONG_ARGUMENTS,
	OPEN_DEVICE_ERROR,
	OPEN_FLASH_FILE_ERROR,
	EMPTY_FLASH_FILE,
	FLASH_PROGRAMMING_ERROR,
	OPEN_EEPROM_FILE_ERROR,
	EMPTY_EEPROM_FILE,
	EEPROM_PROGRAMMING_ERROR,
	EEPROM_DUMPING_ERROR,
	WRITING_EEPROM_DUMP_FILE_ERROR;
	
	static ReturnCode newInstance(final int returnCode) throws RuntimeException {
		switch(returnCode) {
		case -3:
			return FLASH_AND_EEPROM_PROGRAMMING_AVAILABLE;
		case -2:
			return EEPROM_PROGRAMMING_AVAILABLE;
		case -1:
			return FLASH_PROGRAMMING_AVAILABLE;
		case 0:
			return COMPLETED_SUCCESSFULLY;
		case 1:
			return WRONG_ARGUMENTS;
		case 2:
			return OPEN_DEVICE_ERROR;
		case 3:
			return OPEN_FLASH_FILE_ERROR;
		case 4:
			return EMPTY_FLASH_FILE;
		case 5:
			return FLASH_PROGRAMMING_ERROR;
		case 6:
			return OPEN_EEPROM_FILE_ERROR;
		case 7:
			return EMPTY_EEPROM_FILE;
		case 8:
			return EEPROM_PROGRAMMING_ERROR;
		case 9:
			return EEPROM_DUMPING_ERROR;
		case 10:
			return WRITING_EEPROM_DUMP_FILE_ERROR;
		default:
			throw new RuntimeException("Unsuspected return code");
		}
	}
	
	public boolean isError() {
		switch(this) {
		case COMPLETED_SUCCESSFULLY:
		case FLASH_AND_EEPROM_PROGRAMMING_AVAILABLE:
		case EEPROM_PROGRAMMING_AVAILABLE:
		case FLASH_PROGRAMMING_AVAILABLE:
			return false;
		default:
			return true;
		}
	}
	
	public String getTitle() {
		return MessageHelper.get(this, this.name() + "_Title");
	}

	public String getMessage() {
		return MessageHelper.get(this, this.name() + "_Message");
	}
}
