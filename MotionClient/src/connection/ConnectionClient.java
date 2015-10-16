package connection;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.util.concurrent.LinkedBlockingDeque;

public class ConnectionClient implements Runnable{

	private Socket client = null;
	
	private int _port = 0;
	private String _ipAddress = "";
	private LinkedBlockingDeque<byte[]> _blockingQueue;
	
	public ConnectionClient(LinkedBlockingDeque<byte[]> blockingQueue, int port, String ipAddress){
		_port = port;
		_blockingQueue = blockingQueue;
		_ipAddress = ipAddress;
	}
	
	@Override
	public void run() {
		init();
        try {
        	readCommandFromServer();                	
        } 
        finally {
        	closeClientStreams();
        }
	}

	
	private void readCommandFromServer() {
		while (client != null) {								
			try {
				byte[] recevedByte = readByte();
				_blockingQueue.addLast(recevedByte);
			} catch (IOException e) {
				System.out.println("Can't read command from Server.");
				client = null;
				e.printStackTrace();
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

	
	private void init (){
		try {
			client = new Socket(_ipAddress, _port);
		} catch (IOException e) {
			System.out.println("Can't create client socket for connection on port:" + _port);
			e.printStackTrace();
		}
	}
	
	private byte[] readByte() throws IOException {
		InputStream inputStream = client.getInputStream();
		DataInputStream dataInputStream = new DataInputStream(inputStream);

		int len = 64;
		byte[] data = new byte[len];
		dataInputStream.read(data);

		return data;
	}
}
