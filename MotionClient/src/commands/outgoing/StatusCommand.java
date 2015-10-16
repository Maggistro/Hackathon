package commands.outgoing;

import com.kuka.roboticsAPI.controllerModel.sunrise.ISafetyState;
import com.kuka.roboticsAPI.controllerModel.sunrise.SunriseSafetyState.EmergencyStop;
import com.kuka.roboticsAPI.controllerModel.sunrise.SunriseSafetyState.SafetyStopType;
import com.kuka.roboticsAPI.deviceModel.LBR;

import connection.ByteConverter;

public class StatusCommand extends AbstractOutGoingCommand{

	private LBR _robot = null;
	
	@Override
	public byte[] getHeader(){
		return ByteConverter.convertStringTo64Byte("Status");
	}
	
	/**
	 * Default constructor. 
	 * 
	 * @param robot the robot which should be moved.
	 */
	public StatusCommand(LBR robot) {
		_robot = robot;
	}
	
	@Override
	public byte[][] getData(){
		byte[][] data = new byte[1][64];
		
		//------ safty check ------
		ISafetyState saftyState = _robot.getSafetyState();
		boolean isSafe = true;
		
		isSafe |=  EmergencyStop.INACTIVE.equals(saftyState.getEmergencyStopEx());
		isSafe |=  EmergencyStop.INACTIVE.equals(saftyState.getEmergencyStopInt());
		isSafe |=  SafetyStopType.NOSTOP.equals(saftyState.getSafetyStopSignal());
		 
		//------- is robot mastered -------
		boolean isMastered = _robot.isMastered();
		
		// --------- can robot move --------
		boolean canMove = _robot.isReadyToMove();
		
		int status = 1; //1 = all ok, -1 = some error
		
		if(!isMastered | !canMove | !isSafe ){
			status = -1;
		}
		
		data[0] = ByteConverter.convertIntTo64Byte(status);
		
		return data;
	}
}