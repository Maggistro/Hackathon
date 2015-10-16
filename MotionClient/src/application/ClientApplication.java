package application;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingDeque;

import robotData.RobotData;

import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import com.kuka.roboticsAPI.controllerModel.Controller;
import com.kuka.roboticsAPI.deviceModel.LBR;
import commands.incomming.CommandFactory;
import commands.incomming.IInCommingCommand;
import commands.outgoing.IOutGoingCommand;

import connection.ConnectionClient;
import connection.ConnectionServer;

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
public class ClientApplication extends RoboticsAPIApplication {
	private final static String ipAddress = "127.0.0.0";
	private final static int clientPort = 27015;
	private final static int serverPort = 0000;

	private Controller kuka_Sunrise_Cabinet_1;
	private LBR lbr_iiwa_14_R820_1;

	private LinkedBlockingDeque<byte[]> _recivedDataQueue = new LinkedBlockingDeque<byte[]>();
	private LinkedBlockingDeque<IOutGoingCommand> _outGoingCommandQueue = new LinkedBlockingDeque<IOutGoingCommand>();
	private LinkedBlockingDeque<IInCommingCommand> _activCommandQueue = new LinkedBlockingDeque<IInCommingCommand>();

	private RobotData robotData = new RobotData();

	@Override
	public void initialize() {
		kuka_Sunrise_Cabinet_1 = getController("KUKA_Sunrise_Cabinet_1");
		lbr_iiwa_14_R820_1 = (LBR) getDevice(kuka_Sunrise_Cabinet_1,
				"LBR_iiwa_14_R820_1");
	}

	@Override
	public void run() {
		System.out.println(lbr_iiwa_14_R820_1.getCurrentJointPosition());
		
		do {
			ExecutorService executorService = Executors.newFixedThreadPool(3);

			CommandFactory commandFactory = new CommandFactory(
					_recivedDataQueue, _outGoingCommandQueue,
					_activCommandQueue, lbr_iiwa_14_R820_1, robotData);
			ConnectionClient client = new ConnectionClient(_recivedDataQueue,
					clientPort, ipAddress);
			ConnectionServer server = new ConnectionServer(
					_outGoingCommandQueue, serverPort);

			executorService.execute(client);
			executorService.execute(server);
			executorService.execute(commandFactory);
		} while (true);
	}

	@Override
	public void dispose() {

		for (IInCommingCommand incommingcommand : _activCommandQueue) {
			incommingcommand.abourt();
		}

		super.dispose();
	}

	/**
	 * Auto-generated method stub. Do not modify the contents of this method.
	 */
	public static void main(String[] args) {
		ClientApplication app = new ClientApplication();
		app.runApplication();
	}
}
