package commands.incomming;

import java.nio.ByteBuffer;
import java.util.concurrent.LinkedBlockingDeque;

import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.Frame;

import commands.Command;
import commands.ICommand;
import commands.outgoing.IOutGoingCommand;
import commands.outgoing.InfoCommand;
import commands.outgoing.StatusCommand;

import robotData.RobotData;

public class CommandFactory implements Runnable {

	private LinkedBlockingDeque<byte[]> _recivedDataQueue;
	private LinkedBlockingDeque<IOutGoingCommand> _outGoingCommandQueue;
	private LinkedBlockingDeque<IInCommingCommand> _activCommandQueue;

	private LBR _robot = null;
	private RobotData _robotData = null;

	public CommandFactory(LinkedBlockingDeque<byte[]> recivedDataQueue,
			LinkedBlockingDeque<IOutGoingCommand> outGoingCommandQueue,
			LinkedBlockingDeque<IInCommingCommand> activCommandQueue,
			LBR robot, RobotData robotData) {

		_recivedDataQueue = recivedDataQueue;
		_outGoingCommandQueue = outGoingCommandQueue;
		_activCommandQueue = activCommandQueue;

		_robot = robot;
		_robotData = robotData;
	}

	@Override
	public void run() {
		byte[] header = _recivedDataQueue.getFirst();
		String commandName = new String(header);

		Command command = Command.getCommandFromName(commandName);
		ICommand result = null;

		int xCoordinate = 0;
		int yCoordinate = 0;
		int zCoordinate = 0;
		Frame frame = new Frame(xCoordinate, yCoordinate, zCoordinate);

		frame = new Frame(xCoordinate, yCoordinate, zCoordinate);

		switch (command) {
		case START:
			xCoordinate = ByteBuffer.wrap(_recivedDataQueue.getFirst())
					.getInt();
			yCoordinate = ByteBuffer.wrap(_recivedDataQueue.getFirst())
					.getInt();
			zCoordinate = ByteBuffer.wrap(_recivedDataQueue.getFirst())
					.getInt();

			frame = new Frame(xCoordinate, yCoordinate, zCoordinate);

			result = new StartCommand(_robot, frame, _activCommandQueue,
					_robotData);
			break;
		case STOP:
			for (IInCommingCommand toStopCommand : _activCommandQueue) {
				toStopCommand.abourt();
			}
			break;

		case RESET:
			result = new ResetCommand(_robot, _activCommandQueue, _robotData);
			break;
		case FOLLOW:

			xCoordinate = ByteBuffer.wrap(_recivedDataQueue.getFirst())
					.getInt();
			yCoordinate = ByteBuffer.wrap(_recivedDataQueue.getFirst())
					.getInt();
			zCoordinate = ByteBuffer.wrap(_recivedDataQueue.getFirst())
					.getInt();

			frame = new Frame(xCoordinate, yCoordinate, zCoordinate);

			result = new FollowCommand(_robot, frame, _activCommandQueue,
					_robotData);
			break;

		case SPEED_UP:
			result = new SpeedUpCommand(_robotData, _activCommandQueue);
			break;

		case SPEED_DOWN:
			result = new SpeedDownCommand(_robotData, _activCommandQueue);
			break;

		case INFO:
			result = new InfoCommand(_robot);
			break;

		case STATUS:
			result = new StatusCommand(_robot);
			break;

		default:
			System.out.println("Unkown Command: " + command.name());
			break;
		}

		if (result instanceof IInCommingCommand) {
			// Adding incomming command to aktive commands.
			_activCommandQueue.add((IInCommingCommand) result);

			// Command size is limited to 50.
			int offset = _activCommandQueue.size() - 50;
			if (offset > 0) {
				IInCommingCommand toRemoveCommand = _activCommandQueue
						.removeLast();
				toRemoveCommand.abourt();
			}
		} else if (result instanceof IOutGoingCommand) {
			_outGoingCommandQueue.addLast((IOutGoingCommand) result);
		}
	}

}
