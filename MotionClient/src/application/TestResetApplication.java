package application;


import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.*;

import com.kuka.roboticsAPI.controllerModel.Controller;
import com.kuka.roboticsAPI.deviceModel.JointPosition;
import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.motionModel.IMotionContainer;

/**
 * Implementation of a robot application.
 * <p>
 * The application provides a {@link RoboticsAPITask#initialize()} and a 
 * {@link RoboticsAPITask#run()} method, which will be called successively in 
 * the application lifecycle. The application will terminate automatically after 
 * the {@link RoboticsAPITask#run()} method has finished or after stopping the 
 * task. The {@link RoboticsAPITask#dispose()} method will be called, even if an 
 * exception is thrown during initialization or run. 
 * <p>
 * <b>It is imperative to call <code>super.dispose()</code> when overriding the 
 * {@link RoboticsAPITask#dispose()} method.</b> 
 * 
 * @see UseRoboticsAPIContext
 * @see #initialize()
 * @see #run()
 * @see #dispose()
 */
public class TestResetApplication extends RoboticsAPIApplication {
	private Controller kuka_Sunrise_Cabinet_1;
	private LBR _robot;

	@Override
	public void initialize() {
		kuka_Sunrise_Cabinet_1 = getController("KUKA_Sunrise_Cabinet_1");
		_robot = (LBR) getDevice(kuka_Sunrise_Cabinet_1, "LBR_iiwa_14_R820_1");
	}

	@Override
	public void run() {
		_robot.move(ptpHome());
		
		IMotionContainer motion = _robot.moveAsync(ptp(1,1,1,1,1,1,1));
		
		for (int counter = 0; counter < 100000; counter++) {
			//simulate sleep
		}
		
		if (motion != null && !motion.isFinished()) {
			motion.cancel();
			System.out.println("Cancel Motion.");
		}else{
			System.out.println("Motion already has finished.");
		}
		
		final JointPosition start = new JointPosition(0,0,0,Math.toRadians(-90), 0,Math.toRadians(90),0);
		_robot.move(ptp(start));
	}

	/**
	 * Auto-generated method stub. Do not modify the contents of this method.
	 */
	public static void main(String[] args) {
		TestResetApplication app = new TestResetApplication();
		app.runApplication();
	}
}
