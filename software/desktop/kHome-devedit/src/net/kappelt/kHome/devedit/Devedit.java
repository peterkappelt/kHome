package net.kappelt.kHome.devedit;

import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import java.awt.Desktop;
import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;
import java.io.PrintWriter;

import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;

import net.kappelt.kHome.device.Device;
import net.kappelt.kHome.device.Register.registerTypes;
import net.kappelt.kHome.generator.Generator;

import org.eclipse.swt.widgets.TabFolder;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.TabItem;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.events.MouseAdapter;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.wb.swt.SWTResourceManager;

public class Devedit {
	private static Text textMetaAuthor;
	private static Text textMetaDeviceVersion;
	private static Text textMetaDeviceComment;
	private static Text textConfigFileVersion;
	private static Shell shlKhomeDevedit;
	private static Device loadedKhomeDevice;
	private static Text textMetaDeviceID;
	
	/**
	 * Launch the application.
	 * @param args
	 */
	public static void main(String[] args) {		

		Display display = Display.getDefault();
		shlKhomeDevedit = new Shell();
		shlKhomeDevedit.setImage(SWTResourceManager.getImage(Devedit.class, "/net/kappelt/kHome/devedit/kappelt-small-invers-135px.png"));
		shlKhomeDevedit.setSize(878, 560);
		shlKhomeDevedit.setText("kHome Devedit");
		shlKhomeDevedit.setLayout(new FillLayout(SWT.HORIZONTAL));
		
		loadedKhomeDevice = new Device(shlKhomeDevedit);
		
		updateWindowTitle();
		
		Menu menu = new Menu(shlKhomeDevedit, SWT.BAR);
		shlKhomeDevedit.setMenuBar(menu);
		
		MenuItem mntmFile = new MenuItem(menu, SWT.CASCADE);
		mntmFile.setText("File");
		
		Menu menu_1 = new Menu(mntmFile);
		mntmFile.setMenu(menu_1);
		
		
		MenuItem mntmSave = new MenuItem(menu_1, SWT.NONE);
		mntmSave.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				try {
					loadedKhomeDevice.saveFile();
					
					updateWindowTitle();
				} catch (Exception e2) {
					e2.printStackTrace();
				}
			}
		});
		mntmSave.setText("Save...");
		
		TabFolder tabFolder = new TabFolder(shlKhomeDevedit, SWT.NONE);
		
		TabItem tbtmDataRegisters = new TabItem(tabFolder, SWT.NONE);
		tbtmDataRegisters.setText("Data Registers");
		
		RegisterEditor dataRegisterEditor = new RegisterEditor(tabFolder, SWT.NONE, loadedKhomeDevice, shlKhomeDevedit, registerTypes.registerTypeData);
		tbtmDataRegisters.setControl(dataRegisterEditor);
		
		TabItem tbtmConfigRegisters = new TabItem(tabFolder, SWT.NONE);
		tbtmConfigRegisters.setText("Config Registers");
		
		RegisterEditor configRegisterEditor = new RegisterEditor(tabFolder, SWT.NONE, loadedKhomeDevice, shlKhomeDevedit, registerTypes.registerTypeConfig);
		tbtmConfigRegisters.setControl(configRegisterEditor);
		
		TabItem tbtmStatusRegisters = new TabItem(tabFolder, SWT.NONE);
		tbtmStatusRegisters.setText("Status Registers");
		
		RegisterEditor statusRegisterEditor = new RegisterEditor(tabFolder, SWT.NONE, loadedKhomeDevice, shlKhomeDevedit, registerTypes.registerTypeStatus);
		tbtmStatusRegisters.setControl(statusRegisterEditor);
		
		TabItem tbtmMeta = new TabItem(tabFolder, SWT.NONE);
		tbtmMeta.setText("Meta");
		
		Composite composite = new Composite(tabFolder, SWT.NONE);
		tbtmMeta.setControl(composite);
		composite.setLayout(new GridLayout(3, false));
		
		Label lblAuthor = new Label(composite, SWT.NONE);
		lblAuthor.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblAuthor.setText("Author");
		new Label(composite, SWT.NONE);
		
		textMetaAuthor = new Text(composite, SWT.BORDER);
		textMetaAuthor.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		
		Label lblDeviceVersion = new Label(composite, SWT.NONE);
		lblDeviceVersion.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblDeviceVersion.setText("Device Version");
		new Label(composite, SWT.NONE);
		
		textMetaDeviceVersion = new Text(composite, SWT.BORDER);
		textMetaDeviceVersion.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		
		Label lblDeviceComment = new Label(composite, SWT.NONE);
		lblDeviceComment.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblDeviceComment.setText("Device Comment");
		new Label(composite, SWT.NONE);
		
		textMetaDeviceComment = new Text(composite, SWT.BORDER | SWT.MULTI);
		GridData gd_textMetaDeviceComment = new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1);
		gd_textMetaDeviceComment.heightHint = 150;
		textMetaDeviceComment.setLayoutData(gd_textMetaDeviceComment);
		
		Label lblConfigFileVersion = new Label(composite, SWT.NONE);
		lblConfigFileVersion.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblConfigFileVersion.setText("Config File Version");
		new Label(composite, SWT.NONE);
		
		textConfigFileVersion = new Text(composite, SWT.BORDER);
		textConfigFileVersion.setEditable(false);
		textConfigFileVersion.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		
		Label lblDeviceID = new Label(composite, SWT.NONE);
		lblDeviceID.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblDeviceID.setText("Device ID");
		
		Label lblx = new Label(composite, SWT.NONE);
		lblx.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
		lblx.setText("0x");
		
		textMetaDeviceID = new Text(composite, SWT.BORDER);
		textMetaDeviceID.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(composite, SWT.NONE);
		new Label(composite, SWT.NONE);
		new Label(composite, SWT.NONE);
		
		Button btnSaveMeta = new Button(composite, SWT.NONE);
		btnSaveMeta.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseUp(MouseEvent e) {
				saveMetaTab(loadedKhomeDevice);
			}
		});
		btnSaveMeta.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 1, 1));
		btnSaveMeta.setText("Save Meta");
		new Label(composite, SWT.NONE);
		new Label(composite, SWT.NONE);

		MenuItem mntmOpen = new MenuItem(menu_1, SWT.NONE);
		mntmOpen.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e){
				
				try{
					loadedKhomeDevice.loadFile();
					dataRegisterEditor.updateViewFromBackend();
					dataRegisterEditor.tableDeselectItems();
					configRegisterEditor.updateViewFromBackend();
					configRegisterEditor.tableDeselectItems();
					statusRegisterEditor.updateViewFromBackend();
					statusRegisterEditor.tableDeselectItems();
					updateMetaTab(loadedKhomeDevice);
					
					updateWindowTitle();
				}catch(Exception error){
					error.printStackTrace();
				}
			}
		});
		mntmOpen.setText("Open...");
		
		MenuItem mntmCloseFile = new MenuItem(menu_1, SWT.NONE);
		mntmCloseFile.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				loadedKhomeDevice.closeFile();
				dataRegisterEditor.updateViewFromBackend();
				dataRegisterEditor.tableDeselectItems();
				configRegisterEditor.updateViewFromBackend();
				configRegisterEditor.tableDeselectItems();
				statusRegisterEditor.updateViewFromBackend();
				statusRegisterEditor.tableDeselectItems();
				updateMetaTab(loadedKhomeDevice);
				
				updateWindowTitle();
			}
		});
		mntmCloseFile.setText("Close File");
		
		MenuItem mntmGenerate = new MenuItem(menu, SWT.CASCADE);
		mntmGenerate.setText("Generate");
		
		Menu menu_3 = new Menu(mntmGenerate);
		mntmGenerate.setMenu(menu_3);
		
		MenuItem mntmHtmlReport = new MenuItem(menu_3, SWT.NONE);
		mntmHtmlReport.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				BufferedReader br = new BufferedReader(new InputStreamReader(getClass().getResourceAsStream("/net/kappelt/kHome/devedit/generatorTemplates/HTML/report-template.html")));
			    try {
			        StringBuilder sb = new StringBuilder();
			        String line = br.readLine();

			        while (line != null) {
			            sb.append(line);
			            sb.append("\n");
			            line = br.readLine();
			        }	
			        
			        FileDialog fd = new FileDialog(shlKhomeDevedit, SWT.SAVE);
			        String[] filterNames = {
							"HTML Files (*.html)",
							"All Files (*.*)"
					};
					fd.setFilterNames(filterNames);
					
					String[] filterExtensions = {
							"*.html",
							"*.*"
					};
					fd.setFilterExtensions(filterExtensions);
			        
					String fileName = fd.open();
					
					if(fileName == null){
						System.out.println("[Devedit] HTML Report: user abborted save file dialog");
						return;
					}
					
					String output = Generator.replaceMagicInTemplate(sb.toString(), loadedKhomeDevice);
					
					PrintWriter out = new PrintWriter(fileName);
					out.print(output);
					
					Desktop.getDesktop().open(new File(fileName));
					
					out.close();
			        br.close();
			    }catch (Exception error){
			    	MessageBox temp = new MessageBox(shlKhomeDevedit, SWT.ICON_ERROR | SWT.OK);
			    	temp.setMessage("Error while generating report:\n" + error.toString());
			    	temp.open();
			    	
			    	error.printStackTrace();
			    }
			}
		});
		mntmHtmlReport.setText("HTML Report...");
		
		MenuItem mntmCRegisterApi = new MenuItem(menu_3, SWT.NONE);
		mntmCRegisterApi.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				BufferedReader brSource = new BufferedReader(new InputStreamReader(getClass().getResourceAsStream("/net/kappelt/kHome/devedit/generatorTemplates/C_Registers/khRegister.c")));
				BufferedReader brHeader = new BufferedReader(new InputStreamReader(getClass().getResourceAsStream("/net/kappelt/kHome/devedit/generatorTemplates/C_Registers/khRegister.h")));
			    try {
			        
			        
			        FileDialog fd = new FileDialog(shlKhomeDevedit, SWT.SAVE);
			        String[] filterNames = {
							"C Source Files (*.c)",
							"All Files (*.*)"
					};
					fd.setFilterNames(filterNames);
					
					String[] filterExtensions = {
							"*.c",
							"*.*"
					};
					fd.setFilterExtensions(filterExtensions);
			        
					fd.setFileName("khRegister.c");
					
					String fileName = fd.open();
					
					if(fileName == null){
						System.out.println("[Devedit] C Register API: user abborted save file dialog");
						return;
					}
					
					StringBuilder sbSource = new StringBuilder();
			        String lineSource = brSource.readLine();
			        while (lineSource != null) {
			            sbSource.append(lineSource);
			            sbSource.append("\n");
			            lineSource = brSource.readLine();
			        }
			        String outputSource = Generator.replaceMagicInTemplate(sbSource.toString(), loadedKhomeDevice);
					
			        StringBuilder sbHeader = new StringBuilder();
			        String lineHeader = brHeader.readLine();
			        while (lineHeader != null) {
			            sbHeader.append(lineHeader);
			            sbHeader.append("\n");
			            lineHeader = brHeader.readLine();
			        }
			        String outputHeader = Generator.replaceMagicInTemplate(sbHeader.toString(), loadedKhomeDevice);
			        
					PrintWriter out = new PrintWriter(fileName);
					out.print(outputSource);
					out.close();
					
					out = new PrintWriter(fileName.replaceFirst("[.][^.]+$", "") + ".h");
					out.print(outputHeader);
					out.close();
					
					//Do not open it
					//Desktop.getDesktop().open(new File(fileName));
					
			        brSource.close();
			        brHeader.close();
			    }catch (Exception error){
			    	MessageBox temp = new MessageBox(shlKhomeDevedit, SWT.ICON_ERROR | SWT.OK);
			    	temp.setMessage("Error while generating C Register API:\n" + error.toString());
			    	temp.open();
			    	
			    	error.printStackTrace();
			    }
			}
		});
		mntmCRegisterApi.setText("C Register API...");
		
		MenuItem mntmHelp = new MenuItem(menu, SWT.CASCADE);
		mntmHelp.setText("Help");
		
		Menu menu_2 = new Menu(mntmHelp);
		mntmHelp.setMenu(menu_2);
		
		MenuItem mntmAbout = new MenuItem(menu_2, SWT.NONE);
		mntmAbout.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				Shell newShell = new Shell(Display.getDefault(), SWT.TITLE | SWT.CLOSE | SWT.DIALOG_TRIM);
				
				Devedit_About aboutDialog = new Devedit_About(newShell, SWT.MODELESS);
				aboutDialog.open();
			}
		});
		mntmAbout.setText("About");
		
		updateMetaTab(loadedKhomeDevice);
		
		/**
		 * there was a argument in the command line.
		 * Open the defined file
		 */
		if(args.length > 0){
			try{
				loadedKhomeDevice.loadFile(args[0]);
				dataRegisterEditor.updateViewFromBackend();
				dataRegisterEditor.tableDeselectItems();
				configRegisterEditor.updateViewFromBackend();
				configRegisterEditor.tableDeselectItems();
				statusRegisterEditor.updateViewFromBackend();
				statusRegisterEditor.tableDeselectItems();
				updateMetaTab(loadedKhomeDevice);
				
				updateWindowTitle();
			}catch(Exception error){
				MessageBox temp = new MessageBox(shlKhomeDevedit, SWT.ICON_ERROR | SWT.OK);
				temp.setMessage("Error while opening file specified with programm call: \n" + error.toString());
				error.printStackTrace();
			}
		}
		
		shlKhomeDevedit.open();
		shlKhomeDevedit.layout();
		
		while (!shlKhomeDevedit.isDisposed()) {
			if (!display.readAndDispatch()) {
				display.sleep();
			}
		}
	}
	
	public static void updateWindowTitle(){
		String title = "kHome Devedit";
		if(loadedKhomeDevice.getFileIsLoaded()){
			title += " - " + loadedKhomeDevice.getFileNameOpened();
		}else{
			title += " - No File Opened";
		}
		
		if(loadedKhomeDevice.getFileContainsUnsavedChanges()){
			title += "*";
		}
		
		shlKhomeDevedit.setText(title);
	}
	
	static void updateMetaTab(Device khomeDevice){
		textMetaAuthor.setText(khomeDevice.getMetaAuthor());
		textMetaDeviceVersion.setText(khomeDevice.getMetaDeviceVersion());
		textMetaDeviceComment.setText(khomeDevice.getMetaComment().replaceAll("(<br/>|<br>|<br />)", "\n"));
		textConfigFileVersion.setText(khomeDevice.getConfigVersion());
		textMetaDeviceID.setText(Integer.toString(khomeDevice.getMetaDeviceID(), 16));
	}
	
	static void saveMetaTab(Device kHomeDevice){
		int metaDeviceID;
		
		try{
			metaDeviceID = Integer.parseUnsignedInt(textMetaDeviceID.getText(), 16);
		}catch(Exception e){
			MessageBox errorMsg = new MessageBox(shlKhomeDevedit, SWT.ICON_ERROR | SWT.OK);
			errorMsg.setMessage("Invalid data in length ComboBox (" + textMetaDeviceID.getText() + " is not a valid hex string)\n" + e.toString());
			e.printStackTrace();
			errorMsg.open();
	
			return;
		}
		
		kHomeDevice.setMetaAuthor(textMetaAuthor.getText());
		kHomeDevice.setMetaDeviceVersion(textMetaDeviceVersion.getText());
		kHomeDevice.setMetaComment(textMetaDeviceComment.getText().replaceAll("(\r\n|\n\r|\r|\n)", "<br/>"));
		kHomeDevice.setConfigVersion(textConfigFileVersion.getText());
		kHomeDevice.setMetaDeviceID(metaDeviceID);
		
		kHomeDevice.fileMarkChanges();
		updateWindowTitle();
	}
}
