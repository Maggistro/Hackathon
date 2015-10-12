package application;


import java.io.IOException;
import java.util.concurrent.ConcurrentLinkedQueue;

import robotData.RobotData;

import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import com.kuka.roboticsAPI.controllerModel.Controller;
import com.kuka.roboticsAPI.deviceModel.LBR;

import commands.ICommand;
import commands.incomming.CommandFactory;
import commands.incomming.IInCommingCommand;
import commands.outgoing.IOutGoingCommand;

import connection.Connection;

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
	private final static String ipAddress = "";
	private final static  int port = 0000;
	
	private Controller kuka_Sunrise_Cabinet_1;
	private LBR lbr_iiwa_14_R820_1;
	private Connection connection = new Connection(ipAddress, port);
	
	private RobotData robotData = new RobotData();
	private ConcurrentLinkedQueue<IInCommingCommand> aktivCommands = new ConcurrentLinkedQueue<IInCommingCommand>();
	

	@Override
	public void initialize() {
		kuka_Sunrise_Cabinet_1 = getController("KUKA_Sunrise_Cabinet_1");
		lbr_iiwa_14_R820_1 = (LBR) getDevice(kuka_Sunrise_Cabinet_1,
				"LBR_iiwa_14_R820_1");
	}

	@Override
	public void run() {
		ICommand command= null;
		
		do {
			try {
				command = CommandFactory.generateCommand(connection, lbr_iiwa_14_R820_1, robotData, aktivCommands);
				
				if (command == null) {
					System.out.println("No Command recived.");
				}else if(command instanceof IInCommingCommand){
					//Execute incomming command
					IInCommingCommand incommingCommand = (IInCommingCommand) command;
					incommingCommand.execute();
				}else if(command instanceof IOutGoingCommand){
					//Sending outgoing command
					IOutGoingCommand outcommand = (IOutGoingCommand) command;
					connection.sendBytes(outcommand.getHeader(), outcommand.getData());
				}				
			} catch (IOException e) {
				System.out.println("Some error occures during reading form the stream.");
				e.printStackTrace();
			}
		} while (true);
	}

	
	
	@Override
	public void dispose() {
		connection.closeSockedStreams();
		
		for (IInCommingCommand incommingcommand : aktivCommands) {
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
