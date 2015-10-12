package commands.outgoing;

import commands.ICommand;

public interface IOutGoingCommand extends ICommand{

	/**
	 * This method returns the header informations of the command.
	 * @return the header informations as byte Array.
	 */
	public abstract byte[] getHeader();

	/**
	 * This method returns the data informations of the command.
	 * @return the data informations as byte Array.
	 */
	public abstract byte[][] getData();

}