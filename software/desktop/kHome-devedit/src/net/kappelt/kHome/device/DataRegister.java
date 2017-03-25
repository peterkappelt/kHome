/**
 * 
 */
package net.kappelt.kHome.device;

/**
 * @author Peter Kappelt
 *
 */
public class DataRegister extends Register {
	public DataRegister(){
		super();
	}
	
	public DataRegister(int address, int lengthByte, int initialValue, Boolean isReadOnly, String name, String description){
		super(address, lengthByte, initialValue, isReadOnly, name, description);
	}
	
	@Override
	public registerTypes getRegisterType(){
		return registerTypes.registerTypeData;
	}
}
