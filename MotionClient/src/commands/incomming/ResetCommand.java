package commands.incomming;

import static com.kuka.roboticsAPI.motionModel.BasicMotions.ptp;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.ptpHome;

import java.util.concurrent.LinkedBlockingDeque;

import robotData.RobotData;

import com.kuka.roboticsAPI.deviceModel.JointPosition;
import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.motionModel.IMotionContainer;

public class ResetCommand extends AbstractInComingCommand {

	private LBR _robot = null;
	private LinkedBlockingDeque<IInCommingCommand> _activCommands = null;
	private RobotData _robotData = null;

	private IMotionContainer _motion = null;

	/**
	 * Default constructor.
	 * 
	 * @param robot
	 *            the robot which should be moved.
	 */
	public ResetCommand(LBR robot,
			LinkedBlockingDeque<IInCommingCommand> activCommands,
			RobotData robotData) {
		super();
		_robot = robot;
		_activCommands = activCommands;
		_robotData = robotData;
	}

	@Override
	public void execute() {
		for (IInCommingCommand command : _activCommands) {
			command.abourt();
		}
		
		if (_robot != null) {
			final JointPosition start = new JointPosition(0,0,0,Math.toRadians(-90), 0,Math.toRadians(90),0);
			_robot.move(ptp(start));
		}

		_robotData.reset();
	}

	@Override
	public void abourt() {
		if (_motion != null && !_motion.isFinished()) {
			_motion.cancel();
		}
	}

}
