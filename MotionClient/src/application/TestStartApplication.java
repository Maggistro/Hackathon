package application;

import static com.kuka.roboticsAPI.motionModel.BasicMotions.linRel;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.ptp;

import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import com.kuka.roboticsAPI.controllerModel.Controller;
import com.kuka.roboticsAPI.deviceModel.JointPosition;
import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.motionModel.CartesianPTP;
import com.kuka.roboticsAPI.motionModel.RelativeLIN;

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
public class TestStartApplication extends RoboticsAPIApplication {
	private Controller kuka_Sunrise_Cabinet_1;
	private LBR _robot;

	@Override
	public void initialize() {
		kuka_Sunrise_Cabinet_1 = getController("KUKA_Sunrise_Cabinet_1");
		_robot = (LBR) getDevice(kuka_Sunrise_Cabinet_1, "LBR_iiwa_14_R820_1");
	}

	@Override
	public void run() {
		int maxCommands = 5;
		int commandsCount = 0;

		final JointPosition start = new JointPosition(0,0,0,Math.toRadians(-90), 0,Math.toRadians(90),0);
		_robot.move(ptp(start));
		//_robot.move(ptp(getApplicationData().getFrame("/Home")));
		
		do {		
			Frame frameOffset = new Frame(-100.0, 0.0, 0.0);
			
			RelativeLIN motion = linRel(frameOffset.getX(), frameOffset.getY(), frameOffset.getZ());
			_robot.move(motion);
			
			commandsCount++;
		} while (commandsCount <= maxCommands);
	}

	/**
	 * Auto-generated method stub. Do not modify the contents of this method.
	 */
	public static void main(String[] args) {
		TestStartApplication app = new TestStartApplication();
		app.runApplication();
	}
}
