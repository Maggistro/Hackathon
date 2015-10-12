package robotData;

public class RobotData {

	/** The maximal Velocity value. */
	private static final double MAX_VELOCITY = 1;
	/** The step range for increasing or decreasing the velocity. */
	private static final double VELOCITY_STEP = 0.25;
	
	private double _velocity = 0.25;
	

	public void increasingVelocity() {
		if (_velocity < MAX_VELOCITY){
		_velocity = _velocity + VELOCITY_STEP;
		}
	}

	public void decreasingVelocity() {
		if (_velocity > 0) {
			_velocity = _velocity - VELOCITY_STEP;
		}
	}
	
	public double getVelocity(){
		return _velocity;
	}
	
	public void reset(){
		_velocity = 0.25;
	}
}
