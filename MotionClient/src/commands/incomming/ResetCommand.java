package commands.incomming;

import static com.kuka.roboticsAPI.motionModel.BasicMotions.ptpHome;

import java.util.concurrent.ConcurrentLinkedQueue;

import robotData.RobotData;

import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.motionModel.IMotionContainer;

public class ResetCommand implements IInCommingCommand {

	private LBR _robot = null;
	private ConcurrentLinkedQueue<IInCommingCommand> _activCommands = null;
	private RobotData _robotData = null;
	
	private IMotionContainer _motion = null;
	
	/**
	 * Default constructor. 
	 * 
	 * @param robot the robot which should be moved.
	 */
	public ResetCommand(LBR robot,ConcurrentLinkedQueue<IInCommingCommand> activCommands, RobotData robotData) {
		super();
		_robot = robot;
		_activCommands = activCommands;
		_robotData = robotData;
	}

	@Override
	public void execute() {
		if (_robot != null) {
			_motion = _robot.moveAsync(ptpHome());
		}
		
		_robotData.reset();
		
		for (IInCommingCommand command : _activCommands) {
			command.abourt();
		}
	}

	@Override
	public void abourt() {
		if (_motion != null && !_motion.isFinished()) {
			_motion.cancel();
		}
	}

}
