package net.kappelt.kHome.device;

import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Shell;
import org.w3c.dom.*;

import java.io.File;
import java.nio.file.Paths;
import java.util.ArrayList;

import javax.xml.parsers.*;
import javax.xml.transform.Result;
import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

public class Device {
	Boolean fileIsLoaded = false;					//is true, if a file was opened and is in memory
	String fileNameOpened = "";						//the name of the file, which is opened
	Boolean fileContainsUnsavedChanges = false;		//is true, if something was modified in the internal data representation
	
	ArrayList<DataRegister> dataRegisters = new ArrayList<DataRegister>();
	ArrayList<ConfigRegister> configRegisters = new ArrayList<ConfigRegister>();
	ArrayList<StatusRegister> statusRegisters = new ArrayList<StatusRegister>();
	
	Shell s;
	
	String configVersion = "1.0";
	
	String metaAuthor = "";
	String metaDeviceVersion = "";
	String metaComment = "";
	int metaDeviceID = 0;
	
	//Shell is required for File-Open Dialogs
	public Device(Shell s){
		this.s = s;
	}
	
	public void loadFile() throws Exception{
		FileDialog fd = new FileDialog(s, SWT.OPEN);
		
		String[] filterNames = {
				"kHome Device Files (*.khd)",
				"All Files (*.*)"
		};
		fd.setFilterNames(filterNames);
		
		String[] filterExtensions = {
				"*.khd",
				"*.*"
		};
		fd.setFilterExtensions(filterExtensions);
		
		String fileName = fd.open();

		if(fileName == null){
			System.out.println("[Device] loadFile: user aborted open file dialog");
			return;
		}
		
		loadFile(fileName);
	}

	public void loadFile(String filePath) throws Exception{
		closeFile();
		
		//the user may abort the closing of the file due to unsaved changes. Skip the routine then
		if(fileIsLoaded){
			System.out.println("[Device] loadFile Info: Skipped opening due to user abortion!");
			return;
		}
		
		File temp = new File(filePath);
		if(!temp.canRead()){
			MessageBox tempDialog = new MessageBox(s, SWT.ICON_ERROR | SWT.OK);
	    	tempDialog.setText("The desired file isn't readable!");
	    	tempDialog.open();
	    	System.out.println("[Device] loadFile Error: can't read from file \"" + filePath + "\"");
	    	
		}
		
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		
		Document doc = builder.parse(filePath);
		doc.getDocumentElement().normalize();
		
		//process the data registers
		NodeList dataRegistersNodeList = doc.getElementsByTagName("dataRegister");
		for(int i = 0; i < dataRegistersNodeList.getLength(); i++){
			try{
				Element currentNode = (Element) dataRegistersNodeList.item(i);
				DataRegister tempDataRegister = new DataRegister();
				
				if(currentNode.getElementsByTagName("address").getLength() > 0){
					tempDataRegister.setAddress(Integer.parseUnsignedInt(currentNode.getElementsByTagName("address").item(0).getTextContent(), 16));
				}else{
					tempDataRegister.setAddress(0);
				}
				
				if(currentNode.getElementsByTagName("lengthByte").getLength() > 0){
					tempDataRegister.setLengthByte(Integer.parseUnsignedInt(currentNode.getElementsByTagName("lengthByte").item(0).getTextContent(), 16));
				}else{
					tempDataRegister.setLengthByte(1);
				}
				
				if(currentNode.getElementsByTagName("initialValue").getLength() > 0){
					tempDataRegister.setInitialValue(Integer.parseInt(currentNode.getElementsByTagName("initialValue").item(0).getTextContent()));
				}else{
					tempDataRegister.setInitialValue(0);
				}
				
				if(currentNode.getElementsByTagName("readOnly").getLength() > 0){
					tempDataRegister.setIsReadOnly(Boolean.parseBoolean(currentNode.getElementsByTagName("readOnly").item(0).getTextContent()));
				}else{
					tempDataRegister.setIsReadOnly(false);
				}
				
				if(currentNode.getElementsByTagName("name").getLength() > 0){
					tempDataRegister.setName(currentNode.getElementsByTagName("name").item(0).getTextContent().replaceAll("\\s+",""));
				}else{
					tempDataRegister.setName("Undefined");
				}
				
				if(currentNode.getElementsByTagName("description").getLength() > 0){
					tempDataRegister.setDescription(currentNode.getElementsByTagName("description").item(0).getTextContent());
				}else{
					tempDataRegister.setDescription("");
				}
				
				dataRegisters.add(tempDataRegister);
			}catch (Exception e) {
				e.printStackTrace();
			}
		}
		
		//process the config registers
		NodeList configRegistersNodeList = doc.getElementsByTagName("configRegister");
		for(int i = 0; i < configRegistersNodeList.getLength(); i++){
			try{
				Element currentNode = (Element) configRegistersNodeList.item(i);
				ConfigRegister tempConfigRegister = new ConfigRegister();
				
				if(currentNode.getElementsByTagName("address").getLength() > 0){
					tempConfigRegister.setAddress(Integer.parseUnsignedInt(currentNode.getElementsByTagName("address").item(0).getTextContent(), 16));
				}else{
					tempConfigRegister.setAddress(0);
				}
				
				if(currentNode.getElementsByTagName("initialValue").getLength() > 0){
					tempConfigRegister.setInitialValue(Integer.parseInt(currentNode.getElementsByTagName("initialValue").item(0).getTextContent()));
				}else{
					tempConfigRegister.setInitialValue(0);
				}
				
				if(currentNode.getElementsByTagName("readOnly").getLength() > 0){
					tempConfigRegister.setIsReadOnly(Boolean.parseBoolean(currentNode.getElementsByTagName("readOnly").item(0).getTextContent()));
				}else{
					tempConfigRegister.setIsReadOnly(false);
				}
			
				if(currentNode.getElementsByTagName("name").getLength() > 0){
					tempConfigRegister.setName(currentNode.getElementsByTagName("name").item(0).getTextContent().replaceAll("\\s+",""));
				}else{
					tempConfigRegister.setName("Undefined");
				}
				
				if(currentNode.getElementsByTagName("description").getLength() > 0){
					tempConfigRegister.setDescription(currentNode.getElementsByTagName("description").item(0).getTextContent());
				}else{
					tempConfigRegister.setDescription("");
				}
				
				configRegisters.add(tempConfigRegister);
			}catch (Exception e) {
				e.printStackTrace();
			}
		}

		//process the status registers
		NodeList statusRegistersNodeList = doc.getElementsByTagName("statusRegister");
		for(int i = 0; i < statusRegistersNodeList.getLength(); i++){
			try{
				Element currentNode = (Element) statusRegistersNodeList.item(i);
				StatusRegister tempStatusRegister = new StatusRegister();
				
				if(currentNode.getElementsByTagName("address").getLength() > 0){
					tempStatusRegister.setAddress(Integer.parseUnsignedInt(currentNode.getElementsByTagName("address").item(0).getTextContent(), 16));
				}else{
					tempStatusRegister.setAddress(0);
				}
				
				if(currentNode.getElementsByTagName("initialValue").getLength() > 0){
					tempStatusRegister.setInitialValue(Integer.parseInt(currentNode.getElementsByTagName("initialValue").item(0).getTextContent()));
				}else{
					tempStatusRegister.setInitialValue(0);
				}
			
				if(currentNode.getElementsByTagName("name").getLength() > 0){
					tempStatusRegister.setName(currentNode.getElementsByTagName("name").item(0).getTextContent().replaceAll("\\s+",""));
				}else{
					tempStatusRegister.setName("Undefined");
				}
				
				if(currentNode.getElementsByTagName("description").getLength() > 0){
					tempStatusRegister.setDescription(currentNode.getElementsByTagName("description").item(0).getTextContent());
				}else{
					tempStatusRegister.setDescription("");
				}
				
				statusRegisters.add(tempStatusRegister);
			}catch (Exception e) {
				e.printStackTrace();
			}
		}		
		
		try {
			configVersion = doc.getElementsByTagName("version").item(0).getTextContent();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		try {
			NodeList metas = doc.getElementsByTagName("meta");
			if(metas.getLength() > 0){
				Element currentNode = (Element) metas.item(0);
				
				if(currentNode.getElementsByTagName("comment").getLength() > 0){
					setMetaComment(currentNode.getElementsByTagName("comment").item(0).getTextContent());
				}else{
					setMetaComment("");
				}
				if(currentNode.getElementsByTagName("author").getLength() > 0){
					setMetaAuthor(currentNode.getElementsByTagName("author").item(0).getTextContent());
				}else{
					setMetaAuthor("");
				}
				if(currentNode.getElementsByTagName("deviceVersion").getLength() > 0){
					setMetaDeviceVersion(currentNode.getElementsByTagName("deviceVersion").item(0).getTextContent());
				}else{
					setMetaDeviceVersion("");
				}
				if(currentNode.getElementsByTagName("deviceID").getLength() > 0){
					setMetaDeviceID(Integer.parseUnsignedInt(currentNode.getElementsByTagName("deviceID").item(0).getTextContent()));
				}else{
					setMetaDeviceID(0);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		
		fileNameOpened = Paths.get(filePath).getFileName().toString();
		fileIsLoaded = true;
	}
	
	public void saveFile() throws Exception{
		FileDialog fd = new FileDialog(s, SWT.SAVE);
		
		String[] filterNames = {
				"kHome Device Files (*.khd)",
				"All Files (*.*)"
		};
		fd.setFilterNames(filterNames);
		
		String[] filterExtensions = {
				"*.khd",
				"*.*"
		};
		fd.setFilterExtensions(filterExtensions);
		
		String fileName = fd.open();

		//User cancelled. Abort
		if(fileName == null){
			System.out.println("[Device] saveFile: user cancelled file dialog");
			return;
		}
		
		saveFile(fileName);
	}
	
	public void saveFile(String filePath) throws Exception{
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		
		Document doc = builder.newDocument();
		
		//root node
		Element rootElement = doc.createElement("khd");
		doc.appendChild(rootElement);
		
		//data registers
		for(DataRegister dataRegister : dataRegisters){
			Element dataRegisterElement = doc.createElement("dataRegister");
			rootElement.appendChild(dataRegisterElement);
			
			Element dataRegisterAttributeElement;
			
			dataRegisterAttributeElement = doc.createElement("address");
			dataRegisterElement.appendChild(dataRegisterAttributeElement);
			dataRegisterAttributeElement.insertBefore(doc.createTextNode(Integer.toString(dataRegister.getAddress(), 16)), dataRegisterAttributeElement.getLastChild());
			
			dataRegisterAttributeElement = doc.createElement("lengthByte");
			dataRegisterElement.appendChild(dataRegisterAttributeElement);
			dataRegisterAttributeElement.insertBefore(doc.createTextNode(Integer.toString(dataRegister.getLengthByte())), dataRegisterAttributeElement.getLastChild());
			
			dataRegisterAttributeElement = doc.createElement("readOnly");
			dataRegisterElement.appendChild(dataRegisterAttributeElement);
			dataRegisterAttributeElement.insertBefore(doc.createTextNode(dataRegister.getIsReadOnly().toString()), dataRegisterAttributeElement.getLastChild());
			
			dataRegisterAttributeElement = doc.createElement("initialValue");
			dataRegisterElement.appendChild(dataRegisterAttributeElement);
			dataRegisterAttributeElement.insertBefore(doc.createTextNode(Integer.toString(dataRegister.getInitialValue())), dataRegisterAttributeElement.getLastChild());
			
			dataRegisterAttributeElement = doc.createElement("name");
			dataRegisterElement.appendChild(dataRegisterAttributeElement);
			dataRegisterAttributeElement.insertBefore(doc.createTextNode(dataRegister.getName()), dataRegisterAttributeElement.getLastChild());
			
			dataRegisterAttributeElement = doc.createElement("description");
			dataRegisterElement.appendChild(dataRegisterAttributeElement);
			dataRegisterAttributeElement.insertBefore(doc.createTextNode(dataRegister.getDescription()), dataRegisterAttributeElement.getLastChild());
		}

		//config registers
		for(ConfigRegister configRegister : configRegisters){
			Element configRegisterElement = doc.createElement("configRegister");
			rootElement.appendChild(configRegisterElement);
			
			Element configRegisterAttributeElement;
			
			configRegisterAttributeElement = doc.createElement("address");
			configRegisterElement.appendChild(configRegisterAttributeElement);
			configRegisterAttributeElement.insertBefore(doc.createTextNode(Integer.toString(configRegister.getAddress(), 16)), configRegisterAttributeElement.getLastChild());
			
			configRegisterAttributeElement = doc.createElement("lengthByte");
			configRegisterElement.appendChild(configRegisterAttributeElement);
			configRegisterAttributeElement.insertBefore(doc.createTextNode(Integer.toString(configRegister.getLengthByte())), configRegisterAttributeElement.getLastChild());
			
			configRegisterAttributeElement = doc.createElement("readOnly");
			configRegisterElement.appendChild(configRegisterAttributeElement);
			configRegisterAttributeElement.insertBefore(doc.createTextNode(configRegister.getIsReadOnly().toString()), configRegisterAttributeElement.getLastChild());
			
			configRegisterAttributeElement = doc.createElement("initialValue");
			configRegisterElement.appendChild(configRegisterAttributeElement);
			configRegisterAttributeElement.insertBefore(doc.createTextNode(Integer.toString(configRegister.getInitialValue())), configRegisterAttributeElement.getLastChild());
			
			configRegisterAttributeElement = doc.createElement("name");
			configRegisterElement.appendChild(configRegisterAttributeElement);
			configRegisterAttributeElement.insertBefore(doc.createTextNode(configRegister.getName()), configRegisterAttributeElement.getLastChild());
			
			configRegisterAttributeElement = doc.createElement("description");
			configRegisterElement.appendChild(configRegisterAttributeElement);
			configRegisterAttributeElement.insertBefore(doc.createTextNode(configRegister.getDescription()), configRegisterAttributeElement.getLastChild());
		}
		
		//status registers
		for(StatusRegister statusRegister : statusRegisters){
			Element statusRegisterElement = doc.createElement("statusRegister");
			rootElement.appendChild(statusRegisterElement);
			
			Element statusRegisterAttributeElement;
			
			statusRegisterAttributeElement = doc.createElement("address");
			statusRegisterElement.appendChild(statusRegisterAttributeElement);
			statusRegisterAttributeElement.insertBefore(doc.createTextNode(Integer.toString(statusRegister.getAddress(), 16)), statusRegisterAttributeElement.getLastChild());
			
			statusRegisterAttributeElement = doc.createElement("lengthByte");
			statusRegisterElement.appendChild(statusRegisterAttributeElement);
			statusRegisterAttributeElement.insertBefore(doc.createTextNode(Integer.toString(statusRegister.getLengthByte())), statusRegisterAttributeElement.getLastChild());
			
			statusRegisterAttributeElement = doc.createElement("readOnly");
			statusRegisterElement.appendChild(statusRegisterAttributeElement);
			statusRegisterAttributeElement.insertBefore(doc.createTextNode(statusRegister.getIsReadOnly().toString()), statusRegisterAttributeElement.getLastChild());
			
			statusRegisterAttributeElement = doc.createElement("initialValue");
			statusRegisterElement.appendChild(statusRegisterAttributeElement);
			statusRegisterAttributeElement.insertBefore(doc.createTextNode(Integer.toString(statusRegister.getInitialValue())), statusRegisterAttributeElement.getLastChild());
			
			statusRegisterAttributeElement = doc.createElement("name");
			statusRegisterElement.appendChild(statusRegisterAttributeElement);
			statusRegisterAttributeElement.insertBefore(doc.createTextNode(statusRegister.getName()), statusRegisterAttributeElement.getLastChild());
			
			statusRegisterAttributeElement = doc.createElement("description");
			statusRegisterElement.appendChild(statusRegisterAttributeElement);
			statusRegisterAttributeElement.insertBefore(doc.createTextNode(statusRegister.getDescription()), statusRegisterAttributeElement.getLastChild());
		}
		
		//meta tags
		Element metaElement = doc.createElement("meta");
		rootElement.appendChild(metaElement);
		
		Element metaAttributeElement;
		
		metaAttributeElement = doc.createElement("author");
		metaElement.appendChild(metaAttributeElement);
		metaAttributeElement.insertBefore(doc.createTextNode(getMetaAuthor()), metaAttributeElement.getLastChild());
		
		metaAttributeElement = doc.createElement("comment");
		metaElement.appendChild(metaAttributeElement);
		metaAttributeElement.insertBefore(doc.createTextNode(getMetaComment()), metaAttributeElement.getLastChild());
		
		metaAttributeElement = doc.createElement("deviceVersion");
		metaElement.appendChild(metaAttributeElement);
		metaAttributeElement.insertBefore(doc.createTextNode(getMetaDeviceVersion()), metaAttributeElement.getLastChild());
		
		metaAttributeElement = doc.createElement("deviceID");
		metaElement.appendChild(metaAttributeElement);
		metaAttributeElement.insertBefore(doc.createTextNode(Integer.toString(getMetaDeviceID())), metaAttributeElement.getLastChild());
		
		//khd-File-Version
		Element versionElement = doc.createElement("version");
		rootElement.appendChild(versionElement);
		versionElement.insertBefore(doc.createTextNode(getConfigVersion()), versionElement.getLastChild());
		
		//save the stuff, error handling (check if file is writable)
	    Source source = new DOMSource(doc);

	    File file = new File(filePath);
	    
	    if(!file.canWrite() && file.exists()){
	    	MessageBox temp = new MessageBox(s, SWT.ICON_ERROR | SWT.OK);
	    	temp.setMessage("The desired file isn't writeable!");
	    	temp.open();
	    	System.out.println("[Device] saveFile Error: can't write to file \"" + filePath + "\"");
	    	return;
	    }
	    
	    Result result = new StreamResult(file);

	    Transformer xformer = TransformerFactory.newInstance().newTransformer();
	    xformer.transform(source, result);
	    
	    //mark it as unsaved
	    fileContainsUnsavedChanges = false;
	    fileIsLoaded = true;
	    fileNameOpened = Paths.get(filePath).getFileName().toString();
	}
	
	public void closeFile(){
		if(fileContainsUnsavedChanges){
			MessageBox temp = new MessageBox(s, SWT.ICON_QUESTION | SWT.OK | SWT.CANCEL);
			temp.setMessage("The file, which is currently opened, contains unsaved changes!\n\nThe changes will get overwritten.");
			if(temp.open() == SWT.CANCEL){
				return;
			}
		}
		
		dataRegisters.clear();
		configRegisters.clear();
		statusRegisters.clear();
		
		setMetaAuthor("");
		setMetaComment("");
		setMetaDeviceVersion("");
		setMetaDeviceID(0);
		
		fileNameOpened = "";
		fileIsLoaded = false;
		fileContainsUnsavedChanges = false;
	}
	
	public ArrayList<DataRegister> getDataRegisters(){
		return dataRegisters;
	}
	
	public ArrayList<ConfigRegister> getConfigRegisters(){
		return configRegisters;
	}
	
	public ArrayList<StatusRegister> getStatusRegisters(){
		return statusRegisters;
	}

	public String getConfigVersion() {
		return configVersion;
	}

	public void setConfigVersion(String configVersion) {
		this.configVersion = configVersion;
	}

	public String getMetaAuthor() {
		return metaAuthor;
	}

	public void setMetaAuthor(String metaAuthor) {
		this.metaAuthor = metaAuthor;
	}

	public String getMetaDeviceVersion() {
		return metaDeviceVersion;
	}

	public void setMetaDeviceVersion(String metaDeviceVersion) {
		this.metaDeviceVersion = metaDeviceVersion;
	}

	public String getMetaComment() {
		return metaComment;
	}

	public void setMetaComment(String metaComment) {
		this.metaComment = metaComment;
	}

	public int getMetaDeviceID() {
		return metaDeviceID;
	}

	public void setMetaDeviceID(int metaDeviceID) {
		this.metaDeviceID = metaDeviceID;
	}

	public Boolean getFileIsLoaded() {
		return fileIsLoaded;
	}

	public String getFileNameOpened() {
		return fileNameOpened;
	}
	
	/**
	 * call this, when you change something in the backend. Is for user handling and "Are you sure to close" dialog
	 */
	public void fileMarkChanges(){
		fileContainsUnsavedChanges = true;
	}
	
	public Boolean getFileContainsUnsavedChanges(){
		return fileContainsUnsavedChanges;
	}
}
