package commands.incomming;

import commands.ICommand;

public interface IInCommingCommand extends ICommand{

	/** This method contains the Command which was passed from the byte stream. */
	public void execute();
	
	/** This method stops the current Command if possible. */
	public void abourt();
}
