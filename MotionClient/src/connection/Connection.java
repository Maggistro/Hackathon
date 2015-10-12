package connection;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

public class Connection {

	private Socket socket = null;

	public Connection(String ipAddress, int port) {
		super();
		try {
			socket = new Socket(ipAddress, port);

			System.out.println("Connection status with \"" + ipAddress + ":"
					+ port + "\" is " + socket.isConnected() + ".");
		} catch (UnknownHostException e) {
			System.out.println("Host isn't knowen. " + ipAddress + ":" + port);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public byte[] readByte() throws IOException {
		InputStream inputStream = socket.getInputStream();
		DataInputStream dataInputStream = new DataInputStream(inputStream);

		int len = 64;
		byte[] data = new byte[len];
		dataInputStream.read(data);

		return data;
	}

	public void closeSockedStreams() {
		try {
			socket.getOutputStream().flush();
		} catch (IOException e) {
			// Do nothing Output already cloesd
		} finally {
			try {
				socket.getOutputStream().close();
			} catch (IOException e) {
				// Do nothing Output already cloesd
			}
		}

		try {
			socket.getInputStream().close();
		} catch (IOException e) {
			// Do nothing Output already cloesd
		}
	}

	public void sendBytes(byte[] header, byte[][] data) throws IOException {
		if (header == null) {
			throw new IllegalArgumentException("CommandName must not be null.");
		}
		if (header.length != 63) {// Header must be 64 byte
			throw new IllegalArgumentException("CommandName must be 64 byte.");
		}

		OutputStream outByteStream = socket.getOutputStream();
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
}
