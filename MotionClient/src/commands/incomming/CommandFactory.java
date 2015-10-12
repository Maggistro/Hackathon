package commands.incomming;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.concurrent.ConcurrentLinkedQueue;

import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.Frame;

import commands.ICommand;
import commands.outgoing.InfoCommand;
import commands.outgoing.StatusCommand;

import connection.Connection;

import robotData.RobotData;

public class CommandFactory {

	public static ICommand generateCommand(Connection connection, LBR robot,
			RobotData robotData,
			ConcurrentLinkedQueue<IInCommingCommand> activCommands)
			throws IOException {

		byte[] header = connection.readByte();
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
			xCoordinate = ByteBuffer.wrap(connection.readByte()).getInt();
			yCoordinate = ByteBuffer.wrap(connection.readByte()).getInt();
			zCoordinate = ByteBuffer.wrap(connection.readByte()).getInt();

			frame = new Frame(xCoordinate, yCoordinate, zCoordinate);

			result = new StartCommand(robot, frame, activCommands, robotData);
			break;
		case STOP:
			for (IInCommingCommand toStopCommand : activCommands) {
				toStopCommand.abourt();
			}
			break;

		case RESET:
			result = new ResetCommand(robot, activCommands, robotData);
			break;
		case FOLLOW:

			xCoordinate = ByteBuffer.wrap(connection.readByte()).getInt();
			yCoordinate = ByteBuffer.wrap(connection.readByte()).getInt();
			zCoordinate = ByteBuffer.wrap(connection.readByte()).getInt();

			frame = new Frame(xCoordinate, yCoordinate, zCoordinate);

			result = new FollowCommand(robot, frame, activCommands, robotData);
			break;

		case SPEED_UP:
			result = new SpeedUpCommand(robotData, activCommands);
			break;

		case SPEED_DOWN:
			result = new SpeedDownCommand(robotData, activCommands);
			break;

		case INFO:
			result = new InfoCommand(robot);
			break;

		case STATUS:
			result = new StatusCommand(robot);
			break;

		default:
			System.out.println("Unkown Command: " + command.name());
			break;
		}

		
		if (result instanceof IInCommingCommand) {
			//Adding incomming command to aktive commands.
			activCommands.add((IInCommingCommand) result);
			
			// Command size is limited to 50.
			int offset = activCommands.size() - 50;
			if (offset > 0){
				IInCommingCommand toRemoveCommand = activCommands.remove();
				toRemoveCommand.abourt();
			}
		}

		return result;
	}

}
