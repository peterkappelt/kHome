/**
 * 
 */
package net.kappelt.kHome.generator;

import java.sql.Timestamp;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import net.kappelt.kHome.device.ConfigRegister;
import net.kappelt.kHome.device.DataRegister;
import net.kappelt.kHome.device.Device;
import net.kappelt.kHome.device.StatusRegister;

/**
 * @author Peter Kappelt
 *
 */
public class Generator {

	/**
	 * @param template
	 * @param device
	 * @return
	 */
	public static String replaceMagicInTemplate(String template, Device device){
		String temp = template;
		
		int i = 0;
		
		//dataregister replacement
		Pattern pattern = Pattern.compile("\\{\\$BLOCK_DATAREGISTER_START\\}(.*?)\\{\\$BLOCK_DATAREGISTER_STOP\\}", Pattern.DOTALL);
		String[] dataSplit = pattern.split(temp);
		Matcher dataMatcher = pattern.matcher(temp);
		
		i = 0;
		temp = dataSplit[0];
		
		while(dataMatcher.find()){
			i++;
			for(DataRegister reg : device.getDataRegisters()){
				temp += replaceMagicInSingleBlock(dataMatcher.group(1), reg);
			}
			temp += dataSplit[i];
		}
		
		//configregister replacement
		pattern = Pattern.compile("\\{\\$BLOCK_CONFIGREGISTER_START\\}(.*?)\\{\\$BLOCK_CONFIGREGISTER_STOP\\}", Pattern.DOTALL);
		String[] configSplit = pattern.split(temp);
		Matcher configMatcher = pattern.matcher(temp);
		
		i = 0;
		temp = configSplit[0];
		
		while(configMatcher.find()){
			i++;
			for(ConfigRegister reg : device.getConfigRegisters()){
				temp += replaceMagicInSingleBlock(configMatcher.group(1), reg);
			}
			temp += configSplit[i];
		}
		
		//statusregister replacement
		pattern = Pattern.compile("\\{\\$BLOCK_STATUSREGISTER_START\\}(.*?)\\{\\$BLOCK_STATUSREGISTER_STOP\\}", Pattern.DOTALL);
		String[] statusSplit = pattern.split(temp);
		Matcher statusMatcher = pattern.matcher(temp);
		
		i = 0;
		temp = statusSplit[0];
		
		while(statusMatcher.find()){
			i++;
			for(StatusRegister reg : device.getStatusRegisters()){
				temp += replaceMagicInSingleBlock(statusMatcher.group(1), reg);
			}
			temp += statusSplit[i];
		}
		
		//generation date/ time
		Timestamp timestamp = new Timestamp(System.currentTimeMillis());
		temp = temp.replaceAll("\\{\\$GEN_TIME\\}", timestamp.toString());
		
		//meta replacement
		temp = temp.replaceAll("\\{\\$META_AUTHOR\\}", device.getMetaAuthor());
		temp = temp.replaceAll("\\{\\$META_COMMENT\\}", device.getMetaComment());
		temp = temp.replaceAll("\\{\\$META_DEVICE_ID_DEC\\}", Integer.toString(device.getMetaDeviceID()));
		temp = temp.replaceAll("\\{\\$META_DEVICE_ID_HEX\\}", Integer.toString(device.getMetaDeviceID(), 16).toUpperCase());
		temp = temp.replaceAll("\\{\\$META_DEVICE_VERSION\\}", device.getMetaDeviceVersion());
		
		//file name
		temp = temp.replaceAll("\\{\\$FILE_NAME\\}", device.getFileNameOpened());
		
		return temp;
	}
	
	private static String replaceMagicInSingleBlock(String singleBlock, DataRegister register){
		return replaceMagicInSingleBlock(singleBlock, register.getAddress(), register.getLengthByte(), register.getIsReadOnly(), register.getInitialValue(), register.getName(), register.getDescription());
	}
	
	private static String replaceMagicInSingleBlock(String singleBlock, ConfigRegister register){
		return replaceMagicInSingleBlock(singleBlock, register.getAddress(), register.getLengthByte(), register.getIsReadOnly(), register.getInitialValue(), register.getName(), register.getDescription());
	}
	
	private static String replaceMagicInSingleBlock(String singleBlock, StatusRegister register){
		return replaceMagicInSingleBlock(singleBlock, register.getAddress(), register.getLengthByte(), register.getIsReadOnly(), register.getInitialValue(), register.getName(), register.getDescription());
	}
	
	private static String replaceMagicInSingleBlock(String singleBlock, int address, int lengthByte, Boolean readOnly, int initialValue, String name, String description){
		String temp = singleBlock;
		
		temp = temp.replaceAll("\\{\\$ADDRESS_DEC\\}", Integer.toString(address));
		temp = temp.replaceAll("\\{\\$ADDRESS_HEX\\}", Integer.toString(address, 16).toUpperCase());
		temp = temp.replaceAll("\\{\\$LENGTH_BYTE\\}", Integer.toString(lengthByte));
		temp = temp.replaceAll("\\{\\$INITIAL_VALUE\\}", Integer.toString(initialValue));
		temp = temp.replaceAll("\\{\\$READ_ONLY\\}", Boolean.toString(readOnly));
		temp = temp.replaceAll("\\{\\$NAME\\}", name);
		temp = temp.replaceAll("\\{\\$DESCRIPTION\\}", description);
		
		return temp;
	}
}
