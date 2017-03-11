package net.kappelt.kHome.device;

abstract public class Register {
	int address;				//the address of the register
	int lengthByte;				//the length of the register in byte. May be 1, 2, 4 for Data Registers. Is always 1 for Config and Status Registers
	int initialValue;			//the initial value of the register. Is 0 by default
	
	Boolean isReadOnly;			//If one, the register is generated as read-only. Always true for status registers
	
	String name;				//A readable string of the register. May not contain whitespaces
	String description;			//A description of the register. Might be longer and can contain whitespaces, newlines, ...
	
	public enum registerTypes{
		registerTypeUndefined, registerTypeData, registerTypeConfig, registerTypeStatus
	}
	
	public Register(){
		this(0x00, 1, 0, true, "", "");
	}
	
	public Register(int address, int lengthByte, int initialValue, Boolean isReadOnly, String name, String description){
		this.address = address;
		
		this.lengthByte = lengthByte;
		this.initialValue = initialValue;
		
		this.isReadOnly = isReadOnly;
		
		this.name = name;
		this.description = description;
	}
	
	public void setInitialValue(int initialValue){
		this.initialValue = initialValue;
	}
	
	public int getInitialValue(){
		return this.initialValue;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}
	
	public int getLengthByte() {
		return lengthByte;
	}

	/**
	 * @param lengthByte might be 1, 2, 4
	 */
	public void setLengthByte(int lengthByte) {
		if((lengthByte != 1) && (lengthByte != 2) && (lengthByte != 4)){
			this.lengthByte = 1;
		}else{
			this.lengthByte = lengthByte;
		}
	}

	public Boolean getIsReadOnly() {
		return isReadOnly;
	}

	public void setIsReadOnly(Boolean isReadOnly) {
		this.isReadOnly = isReadOnly;
	}

	public int getAddress() {
		return address;
	}

	public void setAddress(int address) {
		this.address = address;
	}
	
	public registerTypes getRegisterType(){
		return registerTypes.registerTypeUndefined;
	}
}
