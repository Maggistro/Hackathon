package commands.outgoing;

public abstract class AbstractOutGoingCommand implements IOutGoingCommand {

	@Override
	public abstract byte[] getHeader();

	@Override
	public abstract byte[][] getData() ;

}
