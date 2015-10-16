package connectionTest;

import java.util.concurrent.LinkedBlockingDeque;

import org.junit.Test;

import connection.ConnectionClient;

public class ServerTest {

	@Test
	public void test() {
		
		LinkedBlockingDeque<byte[]> _recivedDataQueue = new LinkedBlockingDeque<byte[]>();
		ConnectionClient client = new ConnectionClient(_recivedDataQueue,
				27015, "127.0.0.1");
		
		client.run();
	}

}
