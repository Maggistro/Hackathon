package connection;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.LinkedBlockingDeque;

import commands.outgoing.IOutGoingCommand;

public class ConnectionServer implements Runnable{

	private ServerSocket server = null;
	private Socket client = null;
	
	private int _port = 0;
	private LinkedBlockingDeque<IOutGoingCommand> _outGoingcommandQueue;
	
	public ConnectionServer(LinkedBlockingDeque<IOutGoingCommand> outGoingcommandQueue, int port){
		_port = port;
		_outGoingcommandQueue = outGoingcommandQueue;
	}
	
	@Override
	public void run() {
		init();
        try {
            while (true) {
            	waitingForClient();
                try {
                   sendCommandsToClient();                	
                } finally {
                	System.out.println("Some error during sending informations form the client.");
                	closeClientStreams();
                }
            }
        } 
        finally {
        	closeClientStreams();
        	closeServerStreams();
        }
	}

	
	private void sendCommandsToClient() {
		while (client != null) {
			if (!_outGoingcommandQueue.isEmpty()) {
				IOutGoingCommand sendCommand = _outGoingcommandQueue.getFirst();
				
				if (sendCommand != null) {
					try {
						sendBytes(sendCommand.getHeader(), sendCommand.getData());
					} catch (IOException e) {
						System.out.println("Can't send command to client. " + new String(sendCommand.getHeader()));
						client = null;
						_outGoingcommandQueue.addLast(sendCommand);
						e.printStackTrace();
					}
				}
				
			}
		}
	}

	private void closeClientStreams() {
		try {
			client.getOutputStream().flush();
		} catch (IOException e) {
			// Do nothing Output already cloesd
		} finally {
			try {
				client.getOutputStream().close();
			} catch (IOException e) {
				// Do nothing Output already cloesd
			}
		}

		try {
			client.getInputStream().close();
		} catch (IOException e) {
			// Do nothing Output already cloesd
		}
	}

	private void sendBytes(byte[] header, byte[][] data) throws IOException {
		if (header == null) {
			throw new IllegalArgumentException("CommandName must not be null.");
		}
		if (header.length != 63) {// Header must be 64 byte
			throw new IllegalArgumentException("CommandName must be 64 byte.");
		}

		OutputStream outByteStream = client.getOutputStream();
		DataOutputStream dataOutByteStream = new DataOutputStream(outByteStream);

		try {
			dataOutByteStream.write(header, 0, 64);

			if (data != null && data.length > 0) {
				for (int index = 0; index < data.length; index++) {
					dataOutByteStream.write(data[index], 0, 64);
				}
			}
		} catch (IOException e) {
			System.out.println("Can't send command.");
			e.printStackTrace();
		}
	}
	
	private void init (){
		try {
			server = new ServerSocket(_port);
		} catch (IOException e) {
			System.out.println("Can't create Server socket for connection on port:" + _port);
			e.printStackTrace();
		}
	}
	
	private void waitingForClient(){
		 try {
			client = server.accept();
		} catch (IOException e) {
			System.out.println("Error during accepting a client for the server socket.");
			e.printStackTrace();
		}
	}
	
	private void closeServerStreams() {
		try {
			server.close();
		} catch (IOException e) {
			System.out.println("Error during closing the Server connection.");
			e.printStackTrace();
		}
	}
}
