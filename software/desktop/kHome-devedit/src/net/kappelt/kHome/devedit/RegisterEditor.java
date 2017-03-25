package net.kappelt.kHome.devedit;

import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Table;

import java.util.ArrayList;

import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.TableItem;

import net.kappelt.kHome.device.ConfigRegister;
import net.kappelt.kHome.device.DataRegister;
import net.kappelt.kHome.device.Device;
import net.kappelt.kHome.device.Register;
import net.kappelt.kHome.device.Register.registerTypes;
import net.kappelt.kHome.device.StatusRegister;

import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.events.MouseAdapter;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.widgets.Combo;

public class RegisterEditor extends Composite {
	private Shell parentShell;
	
	private Table tableRegisters;

	Device device;
	ArrayList<? extends Register> deviceRegisters;
	
	private Text textAddress;
	private Text textInitialValue;
	private Text textName;
	private Text textDescription;
	private Label lblSelectedIndex;
	private Button btnReadOnly;
	private Combo comboLength;
	
	private registerTypes registerType;
	
	private int settingOriginalAddress = 0;		//contains the original address of the selected item, independent from the user interface
	private Button btnClearSelection;
	private Button btnSave;
	private Button btnDelete;
	
	/**
	 * Create the composite.
	 * @param parent
	 * @param style
	 */
	public RegisterEditor(Composite parent, int style, Device device, Shell parentShell, registerTypes registerType) {
		super(parent, style);
		setLayout(new GridLayout(2, false));
		
		this.parentShell = parentShell;
		
		tableRegisters = new Table(this, SWT.BORDER | SWT.FULL_SELECTION);

		tableRegisters.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));
		tableRegisters.setHeaderVisible(true);
		tableRegisters.setLinesVisible(true);
		
		TableColumn tblclmnAddress = new TableColumn(tableRegisters, SWT.NONE);
		tblclmnAddress.setMoveable(true);
		tblclmnAddress.setWidth(100);
		tblclmnAddress.setText("Address");
		
		TableColumn tblclmnLength = new TableColumn(tableRegisters, SWT.NONE);
		tblclmnLength.setMoveable(true);
		tblclmnLength.setWidth(100);
		tblclmnLength.setText("Length");
		
		TableColumn tblclmnInitialValue = new TableColumn(tableRegisters, SWT.NONE);
		tblclmnInitialValue.setMoveable(true);
		tblclmnInitialValue.setWidth(100);
		tblclmnInitialValue.setText("Initial Value");
		
		TableColumn tblclmnReadOnly = new TableColumn(tableRegisters, SWT.NONE);
		tblclmnReadOnly.setMoveable(true);
		tblclmnReadOnly.setWidth(100);
		tblclmnReadOnly.setText("Read Only?");
		
		TableColumn tblclmnName = new TableColumn(tableRegisters, SWT.NONE);
		tblclmnName.setMoveable(true);
		tblclmnName.setWidth(100);
		tblclmnName.setText("Name");
		
		TableColumn tblclmnDescription = new TableColumn(tableRegisters, SWT.NONE);
		tblclmnDescription.setMoveable(true);
		tblclmnDescription.setWidth(100);
		tblclmnDescription.setText("Description");
		
		Group grpRegisterSettings = new Group(this, SWT.NONE);
		GridData gd_grpRegisterSettings = new GridData(SWT.FILL, SWT.FILL, false, false, 1, 1);
		gd_grpRegisterSettings.widthHint = 250;
		grpRegisterSettings.setLayoutData(gd_grpRegisterSettings);
		grpRegisterSettings.setText("Settings");
		
		textAddress = new Text(grpRegisterSettings, SWT.BORDER);
		textAddress.setBounds(86, 20, 160, 21);
		
		Label lblAddress = new Label(grpRegisterSettings, SWT.NONE);
		lblAddress.setBounds(10, 23, 55, 15);
		lblAddress.setText("Address");
		
		Label lblLength = new Label(grpRegisterSettings, SWT.NONE);
		lblLength.setBounds(10, 50, 55, 15);
		lblLength.setText("Length");
		
		Label lblInitial = new Label(grpRegisterSettings, SWT.NONE);
		lblInitial.setBounds(10, 71, 34, 30);
		lblInitial.setText("Initial\r\nValue");
		
		Label lblName = new Label(grpRegisterSettings, SWT.NONE);
		lblName.setBounds(10, 104, 55, 15);
		lblName.setText("Name");
		
		Label lblDescription = new Label(grpRegisterSettings, SWT.NONE);
		lblDescription.setBounds(10, 156, 81, 15);
		lblDescription.setText("Description");
		
		textInitialValue = new Text(grpRegisterSettings, SWT.BORDER);
		textInitialValue.setBounds(71, 74, 175, 21);
		
		btnReadOnly = new Button(grpRegisterSettings, SWT.CHECK);
		btnReadOnly.setBounds(10, 134, 93, 16);
		btnReadOnly.setText("Read Only");
		
		textName = new Text(grpRegisterSettings, SWT.BORDER);
		textName.setBounds(71, 101, 175, 21);
		
		textDescription = new Text(grpRegisterSettings, SWT.BORDER | SWT.V_SCROLL | SWT.MULTI);
		textDescription.setBounds(10, 177, 236, 171);
		
		Label lblSelectedIndexCaption = new Label(grpRegisterSettings, SWT.NONE);
		lblSelectedIndexCaption.setBounds(10, 354, 81, 15);
		lblSelectedIndexCaption.setText("Selected Index:");
		
		lblSelectedIndex = new Label(grpRegisterSettings, SWT.NONE);
		lblSelectedIndex.setBounds(97, 354, 55, 15);
		lblSelectedIndex.setText("UNDEF");
		
		comboLength = new Combo(grpRegisterSettings, SWT.READ_ONLY);
		comboLength.setItems(new String[] {"1", "2", "4"});
		comboLength.setBounds(71, 45, 175, 23);
		comboLength.select(0);
		comboLength.setText("1");
		
		btnSave = new Button(grpRegisterSettings, SWT.NONE);
		btnSave.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseUp(MouseEvent e) {
				settingsSaveButtonClicked();
			}
		});
		btnSave.setBounds(10, 420, 75, 25);
		btnSave.setText("Save");
		btnSave.setVisible(false);
		
		btnClearSelection = new Button(grpRegisterSettings, SWT.NONE);
		btnClearSelection.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseUp(MouseEvent e) {
				tableDeselectItems();
			}
		});
		btnClearSelection.setToolTipText("deselect the current row");
		btnClearSelection.setBounds(10, 389, 25, 25);
		btnClearSelection.setText("X");
		btnClearSelection.setVisible(false);
		
		Button btnAddNew = new Button(grpRegisterSettings, SWT.NONE);
		btnAddNew.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseUp(MouseEvent e) {
				settingsAddButtonClicked();
			}
		});
		btnAddNew.setBounds(91, 420, 75, 25);
		btnAddNew.setText("Add New");
		
		btnDelete = new Button(grpRegisterSettings, SWT.NONE);
		btnDelete.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseUp(MouseEvent e) {
				settingsDeleteButtonClicked();
			}
		});
		btnDelete.setBounds(171, 420, 75, 25);
		btnDelete.setText("Delete");
		btnDelete.setVisible(false);
		
		Label lblx = new Label(grpRegisterSettings, SWT.NONE);
		lblx.setBounds(71, 23, 15, 15);
		lblx.setText("0x");

		tableRegisters.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				tableNewItemSelected();
			}
		});
		
		this.device = device;
		this.registerType = registerType;
		
		if(registerType == registerTypes.registerTypeData){
			this.deviceRegisters = device.getDataRegisters();
		}else if(registerType == registerTypes.registerTypeConfig){
			this.deviceRegisters = device.getConfigRegisters();
		}else if(registerType == registerTypes.registerTypeStatus){
			this.deviceRegisters = device.getStatusRegisters();
		}else{
			System.out.print("[RegisterEditor] Init: register type is undefined");
		}
		
		updateViewFromBackend();
		
	}

	public void updateViewFromBackend(){
		tableRegisters.removeAll();
		
		for(Register register : deviceRegisters){
			TableItem newRow = new TableItem(tableRegisters, SWT.NONE, 0);
			
			newRow.setText(0, "0x" + Integer.toString(register.getAddress(), 16));
			newRow.setText(1, Integer.toString(register.getLengthByte()));
			newRow.setText(2, Integer.toString(register.getInitialValue()));
			newRow.setText(3, Boolean.toString(register.getIsReadOnly()));
			newRow.setText(4, register.getName());
			newRow.setText(5, register.getDescription());
		}
		
		//disable the buttons from the according register type
		if(registerType == registerTypes.registerTypeStatus){
			btnReadOnly.setEnabled(false);
			btnReadOnly.setSelection(true);
			comboLength.setEnabled(false);
		}else if(registerType == registerTypes.registerTypeConfig){
			btnReadOnly.setEnabled(true);
			comboLength.setEnabled(false);
		}else if(registerType == registerTypes.registerTypeData){
			btnReadOnly.setEnabled(true);
			comboLength.setEnabled(true);
		}else{
			System.out.println("[RegisterEditor] Error: register type in updateViewFromBackend is undefined");
		}
	}
	
	void tableNewItemSelected(){
		lblSelectedIndex.setText(Integer.toString(tableRegisters.getSelectionIndex()));
		TableItem[] selection = tableRegisters.getSelection();
		
		settingOriginalAddress = Integer.parseUnsignedInt(selection[0].getText(0).substring(2), 16);
		
		textAddress.setText(selection[0].getText(0).substring(2));
		comboLength.setText(selection[0].getText(1));
		textInitialValue.setText(selection[0].getText(2));
		btnReadOnly.setSelection(Boolean.parseBoolean(selection[0].getText(3)));
		textName.setText(selection[0].getText(4));
		textDescription.setText(selection[0].getText(5).replaceAll("(<br/>|<br>|<br />)", "\n"));
		
		btnClearSelection.setVisible(true);
		btnSave.setVisible(true);
		btnDelete.setVisible(true);
	}
	
	void tableDeselectItems(){
		tableRegisters.deselectAll();
		
		textAddress.setText("");
		comboLength.setText("");
		textInitialValue.setText("");
		btnReadOnly.setSelection(false);
		textName.setText("");
		textDescription.setText("");
		
		btnClearSelection.setVisible(false);
		btnSave.setVisible(false);
		btnDelete.setVisible(false);
	}
	
	void settingsSaveButtonClicked(){
		//parse the inputs and check for error
		int address = 0;
		int lengthByte = 1;
		int initialValue = 0;
		Boolean isReadOnly = false;
		String name = "";
		String description = "";
		
		try{
			address = Integer.parseUnsignedInt(textAddress.getText(), 16);
		}catch(Exception e){
			MessageBox errorMsg = new MessageBox(parentShell, SWT.ICON_ERROR | SWT.OK);
			errorMsg.setMessage("Invalid data in address field (" + textAddress.getText() + " is not a valid hex string)\n" + e.toString());
			e.printStackTrace();
			errorMsg.open();
	
			return;
		}
		
		try{
			lengthByte = Integer.parseUnsignedInt(comboLength.getText());
		}catch(Exception e){
			MessageBox errorMsg = new MessageBox(parentShell, SWT.ICON_ERROR | SWT.OK);
			errorMsg.setMessage("Invalid data in length ComboBox (" + comboLength.getText() + " is not a valid number string)\n" + e.toString());
			e.printStackTrace();
			errorMsg.open();
	
			return;
		}
		if((lengthByte != 1) && (lengthByte != 2) && (lengthByte != 4)){
			MessageBox errorMsg = new MessageBox(parentShell, SWT.ICON_ERROR | SWT.OK);
			errorMsg.setMessage("Invalid data in length ComboBox (" + comboLength.getText() + " may only be 1, 2 ord 4 bytes)");
			errorMsg.open();
			
			return;
		}
		
		try{
			initialValue = Integer.parseInt(textInitialValue.getText());
		}catch(Exception e){
			MessageBox errorMsg = new MessageBox(parentShell, SWT.ICON_ERROR | SWT.OK);
			errorMsg.setMessage("Invalid data in initial value field (" + textInitialValue.getText() + " is not a valid number string)\n" + e.toString());
			e.printStackTrace();
			errorMsg.open();
	
			return;
		}
		
		isReadOnly = btnReadOnly.getSelection();
		
		name = textName.getText();
		
		description = textDescription.getText().replaceAll("(\r\n|\n\r|\r|\n)", "<br/>");
		
		//search for the current address in the list of defined registers
		for(Register register : deviceRegisters){
			if(register.getAddress() == settingOriginalAddress){
				register.setAddress(address);
				register.setLengthByte(lengthByte);
				register.setInitialValue(initialValue);
				register.setIsReadOnly(isReadOnly);
				register.setName(name);
				register.setDescription(description);
				
				break;
			}
		}
		
		//finally: update the table view
		updateViewFromBackend();
		
		//mark the changes
		device.fileMarkChanges();
		Devedit.updateWindowTitle();
		
		//deselect the items
		tableDeselectItems();
	}
	
	void settingsAddButtonClicked(){
		//parse the inputs and check for error
		int address = 0;
		int lengthByte = 1;
		int initialValue = 0;
		Boolean isReadOnly = false;
		String name = "";
		String description = "";
		
		try{
			address = Integer.parseUnsignedInt(textAddress.getText(), 16);
		}catch(Exception e){
			MessageBox errorMsg = new MessageBox(parentShell, SWT.ICON_ERROR | SWT.OK);
			errorMsg.setMessage("Invalid data in address field (" + textAddress.getText() + " is not a valid hex string)\n" + e.toString());
			e.printStackTrace();
			errorMsg.open();
	
			return;
		}
		
		try{
			lengthByte = Integer.parseUnsignedInt(comboLength.getText());
		}catch(Exception e){
			MessageBox errorMsg = new MessageBox(parentShell, SWT.ICON_ERROR | SWT.OK);
			errorMsg.setMessage("Invalid data in length ComboBox (" + comboLength.getText() + " is not a valid number string)\n" + e.toString());
			e.printStackTrace();
			errorMsg.open();
	
			return;
		}
		if((lengthByte != 1) && (lengthByte != 2) && (lengthByte != 4)){
			MessageBox errorMsg = new MessageBox(parentShell, SWT.ICON_ERROR | SWT.OK);
			errorMsg.setMessage("Invalid data in length ComboBox (" + comboLength.getText() + " may only be 1, 2 ord 4 bytes)");
			errorMsg.open();
			
			return;
		}
		
		try{
			initialValue = Integer.parseInt(textInitialValue.getText());
		}catch(Exception e){
			MessageBox errorMsg = new MessageBox(parentShell, SWT.ICON_ERROR | SWT.OK);
			errorMsg.setMessage("Invalid data in initial value field (" + textInitialValue.getText() + " is not a valid number string)\n" + e.toString());
			e.printStackTrace();
			errorMsg.open();
	
			return;
		}
		
		isReadOnly = btnReadOnly.getSelection();
		
		name = textName.getText();
		
		description = textDescription.getText().replaceAll("(\r\n|\n\r|\r|\n)", "<br/>");
		
		if(registerType == registerTypes.registerTypeData){
			DataRegister temp = new DataRegister(
				address,
				lengthByte,
				initialValue,
				isReadOnly,
				name,
				description
			);
			device.getDataRegisters().add(temp);
		}else if(registerType == registerTypes.registerTypeConfig){
			ConfigRegister temp = new ConfigRegister(
				address,
				initialValue,
				isReadOnly,
				name,
				description
			);
			device.getConfigRegisters().add(temp);
		}else if(registerType == registerTypes.registerTypeStatus){
			StatusRegister temp = new StatusRegister(
				address,
				initialValue,
				name,
				description
			);
			device.getStatusRegisters().add(temp);
		}
		updateViewFromBackend();
		
		//mark the changes
		device.fileMarkChanges();
		Devedit.updateWindowTitle();
		
		//deselect the items
		tableDeselectItems();
	}
	
	void settingsDeleteButtonClicked(){
		//search for the current address and delete it
		
		//store the register temporary
		Register registerToDelete = null;
		Boolean foundRegister = false;
		
		for(Register register : deviceRegisters){
			if(register.getAddress() == settingOriginalAddress){
				foundRegister = true;
				registerToDelete = register;
			}
		}
		
		if(foundRegister){
			deviceRegisters.remove(registerToDelete);
		}
		
		//finally: update the table view
		updateViewFromBackend();
		
		//mark the changes
		device.fileMarkChanges();
		Devedit.updateWindowTitle();
		
		//deselect the items
		tableDeselectItems();
	}
	
	@Override
	protected void checkSubclass() {
		// Disable the check that prevents subclassing of SWT components
	}
}
