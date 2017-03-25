package net.kappelt.kHome.devedit;

import org.eclipse.swt.widgets.Dialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Label;
import org.eclipse.wb.swt.SWTResourceManager;

public class Devedit_About extends Dialog {

	protected Object result;
	protected Shell shell;

	/**
	 * Create the dialog.
	 * @param parent
	 * @param style
	 */
	public Devedit_About(Shell parent, int style) {
		super(parent, style);
		setText("About Devedit");
	}

	/**
	 * Open the dialog.
	 * @return the result
	 */
	public Object open() {
		createContents();
		shell.open();
		shell.layout();
		Display display = getParent().getDisplay();
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch()) {
				display.sleep();
			}
		}
		return result;
	}

	/**
	 * Create contents of the dialog.
	 */
	private void createContents() {
		shell = new Shell(getParent(), SWT.DIALOG_TRIM);
		shell.setSize(527, 280);
		shell.setText(getText());
		
		Label lblLogo = new Label(shell, SWT.NONE);
		lblLogo.setImage(SWTResourceManager.getImage(Devedit_About.class, "/net/kappelt/kHome/devedit/khome-std-500x150-transparent.png"));
		lblLogo.setBounds(10, 10, 501, 150);
		
		Label lblDescription = new Label(shell, SWT.NONE);
		lblDescription.setAlignment(SWT.CENTER);
		lblDescription.setBounds(10, 166, 501, 75);
		lblDescription.setText("kHome Devedit Tool\r\n\r\nVersion 1.2\r\n\r\n\u00A9 Peter Kappelt 2017");

	}
}
