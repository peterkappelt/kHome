package net.kappelt.kHome.device;

/**
 * 
 * @author Peter Kappelt
 *
 */

public class StatusRegister extends Register {
	
	public StatusRegister(){
		super();
		super.lengthByte = 1;
		super.isReadOnly = true;
	}
	
	public StatusRegister(int address, int initialValue, String name, String description){
		super(address, 1, initialValue, true, name, description);
	}
	
	
	@Override
	@Deprecated
	public void setIsReadOnly(Boolean isReadOnly){
		return;
	}
	
	@Override
	@Deprecated
	public void setLengthByte(int lengthByte){
		return;
	}
	
	@Override
	public registerTypes getRegisterType(){
		return registerTypes.registerTypeStatus;
	}
}
