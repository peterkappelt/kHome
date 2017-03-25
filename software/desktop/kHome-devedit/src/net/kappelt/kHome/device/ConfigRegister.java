/**
 * 
 */
package net.kappelt.kHome.device;

/**
 * @author Peter Kappelt
 *
 */
public class ConfigRegister extends Register {

	public ConfigRegister(){
		super();
		super.lengthByte = 1;
	}
	
	public ConfigRegister(int address, int initialValue, Boolean isReadOnly, String name, String description){
		super(address, 1, initialValue, isReadOnly, name, description);
	}
	
	@Override
	@Deprecated
	public void setLengthByte(int lengthByte){
		return;
	}
	
	@Override
	public registerTypes getRegisterType(){
		return registerTypes.registerTypeConfig;
	}
}
