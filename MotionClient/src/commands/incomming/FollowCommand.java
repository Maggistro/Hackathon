package commands.incomming;

import java.util.concurrent.ConcurrentLinkedQueue;

import robotData.RobotData;

import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.motionModel.CartesianPTP;
import com.kuka.roboticsAPI.motionModel.IMotionContainer;

public class FollowCommand implements IInCommingCommand{
	private LBR _robot = null;
	private Frame _frame = null;
	private RobotData _robotData = null;
	
	private IMotionContainer _motion = null;
	private ConcurrentLinkedQueue<IInCommingCommand> _activCommands = null;
	
	/**
	 * Default constructor. 
	 * 
	 * @param robot the robot which should be moved.
	 * @param frame the new frame coordinates.
	 */
	public FollowCommand(LBR robot, Frame frame,ConcurrentLinkedQueue<IInCommingCommand> activCommands, RobotData robotData) {
		super();
		_robot = robot;
		_frame = frame;
		_activCommands = activCommands;
		_robotData = robotData;
	}

	@Override
	public void execute() {
		if (_robot != null && _frame != null) {
			_motion = _robot.moveAsync(new CartesianPTP(getNextTarget()).setJointVelocityRel(_robotData.getVelocity()));
		}
	}

	@Override
	public void abourt() {
		if (_motion != null && !_motion.isFinished()) {
			_motion.cancel();
		}
		_activCommands.remove(this);
	}
	
	/**
	 * Creates the new Frame for the motion. The default range to move is defined in the DEFAULT_MOTION_DELTA constants which is: {@value DEFAULT_MOTION_DELTA}.
	 * @return
	 * 	the ne frame with the offset.
	 */
	private Frame getNextTarget() {
		Frame currentFlangePosition = _robot.getCurrentCartesianPosition(_robot.getFlange());
		
		double x = _frame.getX() != 0.0? currentFlangePosition.getX() + _frame.getX() : currentFlangePosition.getX();
		double z = _frame.getY() != 0.0? currentFlangePosition.getY() + _frame.getY() : currentFlangePosition.getY();
		double y = _frame.getZ() != 0.0? currentFlangePosition.getZ() + _frame.getZ() : currentFlangePosition.getZ();		
		
		return new Frame(x,y,z);
	}
}
