package commands.incomming;

import static com.kuka.roboticsAPI.motionModel.BasicMotions.linRel;

import java.util.concurrent.LinkedBlockingDeque;

import robotData.RobotData;

import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.motionModel.CartesianPTP;
import com.kuka.roboticsAPI.motionModel.IMotionContainer;
import com.kuka.roboticsAPI.motionModel.RelativeLIN;

public class StartCommand extends AbstractInComingCommand {

	private static final int DEFAULT_MOTION_DELTA_IN_MM = 500;

	private LBR _robot = null;
	private Frame _frame = null;
	private LinkedBlockingDeque<IInCommingCommand> _activCommands = null;
	private RobotData _robotData = null;

	private IMotionContainer _motion = null;
	private boolean _isRunning = false;

	/**
	 * Default constructor.
	 * 
	 * @param robot
	 *            the robot which should be moved.
	 * @param frame
	 *            the frame which determines the direction of the motion.
	 */
	public StartCommand(LBR robot, Frame frame,
			LinkedBlockingDeque<IInCommingCommand> activCommands,
			RobotData robotData) {
		super();
		_robot = robot;
		_frame = frame;
		_activCommands = activCommands;
		_robotData = robotData;
	}

	@Override
	public void execute() {
		if (_robot != null && _frame != null) {

			_isRunning = true;

			do {
				RelativeLIN motion = linRel(_frame.getX(), _frame.getY(), _frame.getZ());
				_robot.move(motion);
			} while (_isRunning);
		}
		_activCommands.remove(this);
	}

	@Override
	public void abourt() {
		if (_motion != null && !_motion.isFinished()) {
			_isRunning = false;
			_motion.cancel();
		}
		_activCommands.remove(this);
	}
}
