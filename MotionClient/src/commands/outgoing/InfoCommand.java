package commands.outgoing;

import com.kuka.roboticsAPI.deviceModel.JointPosition;
import com.kuka.roboticsAPI.deviceModel.LBR;

import connection.ByteConverter;

public class InfoCommand extends AbstractOutGoingCommand{

	private LBR _robot = null;
	
	/**
	 * Default constructor. 
	 * 
	 * @param robot the robot which should be moved.
	 */
	public InfoCommand(LBR robot) {
		_robot = robot;
	}

	@Override
	public byte[] getHeader(){
		return ByteConverter.convertStringTo64Byte("Info");
	}
	
	@Override
	public byte[][] getData(){
		JointPosition currentPosition = _robot.getCurrentJointPosition();
		int axisCount = currentPosition.getAxisCount();
		
		byte[][] data = new byte[axisCount][64];
		
		for (int jointIndex = 0; jointIndex < axisCount; jointIndex++) {
			data[1] = ByteConverter.convertIntTo64Byte((int) currentPosition.get(jointIndex));
		}
		
		return data;
	}
}
