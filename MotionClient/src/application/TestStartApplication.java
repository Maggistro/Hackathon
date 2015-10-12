package application;

import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import com.kuka.roboticsAPI.controllerModel.Controller;
import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.motionModel.CartesianPTP;

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
		do {
			Frame currentFlangePosition = _robot
					.getCurrentCartesianPosition(_robot.getFlange());
			Frame frame = new Frame(0.0, 0.0, 1.0);

			double x = frame.getX() != 0.0 ? currentFlangePosition.getX() + frame.getX() : currentFlangePosition.getX();
			double z = frame.getY() != 0.0 ? currentFlangePosition.getY() + frame.getY() : currentFlangePosition.getY();
			double y = frame.getZ() != 0.0 ? currentFlangePosition.getZ() + frame.getZ() : currentFlangePosition.getZ();

			Frame moveToFrame = new Frame(x, y, z);

			_robot.moveAsync(new CartesianPTP(moveToFrame));
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
