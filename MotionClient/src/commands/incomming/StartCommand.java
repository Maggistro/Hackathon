package commands.incomming;

import java.util.concurrent.ConcurrentLinkedQueue;

import robotData.RobotData;

import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.motionModel.CartesianPTP;
import com.kuka.roboticsAPI.motionModel.IMotionContainer;

public class StartCommand implements IInCommingCommand{
	
	private static final int DEFAULT_MOTION_DELTA_IN_MM = 500;
	
	private LBR _robot = null;
	private Frame _frame = null;
	private ConcurrentLinkedQueue<IInCommingCommand> _activCommands = null;
	private RobotData _robotData = null;
	
	private IMotionContainer _motion = null;
	private boolean _isRunning = false;
	
	
	/**
	 * Default constructor. 
	 * 
	 * @param robot the robot which should be moved.
	 * @param frame the frame which determines the direction of the motion.
	 */
	public StartCommand(LBR robot, Frame frame,ConcurrentLinkedQueue<IInCommingCommand> activCommands, RobotData robotData) {
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
				_motion = _robot.move(new CartesianPTP(getNextTarget()).setJointVelocityRel(_robotData.getVelocity()));
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
	
	/**
	 * Creates the new Frame for the motion. The default range to move is defined in the DEFAULT_MOTION_DELTA constants which is: {@value DEFAULT_MOTION_DELTA}.
	 * @return
	 * 	the ne frame with the offset.
	 */
	private Frame getNextTarget() {
		Frame currentFlangePosition = _robot.getCurrentCartesianPosition(_robot.getFlange());
		
		double x = _frame.getX() != 0.0? currentFlangePosition.getX() + DEFAULT_MOTION_DELTA_IN_MM : currentFlangePosition.getX();
		double z = _frame.getY() != 0.0? currentFlangePosition.getY() + DEFAULT_MOTION_DELTA_IN_MM : currentFlangePosition.getY();
		double y = _frame.getZ() != 0.0? currentFlangePosition.getZ() + DEFAULT_MOTION_DELTA_IN_MM : currentFlangePosition.getZ();		
		
		return new Frame(x,y,z);
	}
}
