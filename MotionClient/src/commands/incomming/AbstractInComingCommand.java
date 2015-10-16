package commands.incomming;

public abstract class AbstractInComingCommand implements IInCommingCommand {

	@Override
	public abstract void execute() ;

	@Override
	public abstract void abourt() ;

}
