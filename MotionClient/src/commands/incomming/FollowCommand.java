package commands.incomming;

import static com.kuka.roboticsAPI.motionModel.BasicMotions.linRel;

import java.util.concurrent.LinkedBlockingDeque;

import robotData.RobotData;

import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.motionModel.CartesianPTP;
import com.kuka.roboticsAPI.motionModel.IMotionContainer;
import com.kuka.roboticsAPI.motionModel.RelativeLIN;

public class FollowCommand extends AbstractInComingCommand {
	private LBR _robot = null;
	private Frame _frame = null;
	private RobotData _robotData = null;

	private IMotionContainer _motion = null;
	private LinkedBlockingDeque<IInCommingCommand> _activCommands = null;

	/**
	 * Default constructor.
	 * 
	 * @param robot
	 *            the robot which should be moved.
	 * @param frame
	 *            the new frame coordinates.
	 */
	public FollowCommand(LBR robot, Frame frame,
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
			
			RelativeLIN motion = linRel(_frame.getX(), _frame.getY(), _frame.getZ());
			_robot.move(motion);
		}
	}

	@Override
	public void abourt() {
		if (_motion != null && !_motion.isFinished()) {
			_motion.cancel();
		}
		_activCommands.remove(this);
	}
}
