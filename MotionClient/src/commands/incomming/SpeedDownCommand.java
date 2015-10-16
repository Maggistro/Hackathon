package commands.incomming;

import java.util.concurrent.LinkedBlockingDeque;

import robotData.RobotData;

public class SpeedDownCommand extends AbstractInComingCommand {

	/** The data for the robot. */
	private RobotData _data = null;
	private LinkedBlockingDeque<IInCommingCommand> _activCommands = null;

	/**
	 * Default constructor.
	 * 
	 * @param data
	 *            The data of the robot which contains the velocity data.
	 */
	public SpeedDownCommand(RobotData data,
			LinkedBlockingDeque<IInCommingCommand> activCommands) {
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
