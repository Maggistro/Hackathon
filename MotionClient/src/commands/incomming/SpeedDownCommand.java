package commands.incomming;

import java.util.concurrent.ConcurrentLinkedQueue;

import robotData.RobotData;

public class SpeedDownCommand implements IInCommingCommand{

	/** The data for the robot. */
	private RobotData _data = null;
	private ConcurrentLinkedQueue<IInCommingCommand> _activCommands = null;
	
	/**
	 * Default constructor.
	 * @param data The data of the robot which contains the velocity data.
	 */
	public SpeedDownCommand(RobotData data,ConcurrentLinkedQueue<IInCommingCommand> activCommands) {
		super();
		_data = data;
	}
	
	@Override
	public void execute() {
		_data.decreasingVelocity();
		_activCommands.remove(this);
	}

	@Override
	public void abourt() {
		_data.increasingVelocity();
		_activCommands.remove(this);
	}
}
