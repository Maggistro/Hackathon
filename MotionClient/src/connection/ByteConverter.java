package connection;

import java.nio.ByteBuffer;

public class ByteConverter {

	public static byte[] convertIntTo64Byte(int numberToConvert) {
		return ByteBuffer.allocate(64).putInt(numberToConvert).array();
	}

	public static byte[] convertStringTo64Byte(String stringToConvert) {
		byte[] stringAsByteArray = stringToConvert.getBytes();
		byte[] data = new byte[64];

		if (stringAsByteArray.length <= 64) {
			for (int index = 0; index < stringAsByteArray.length; index++) {
				byte lookedByte = stringAsByteArray[index];
				data[index] = lookedByte;
			}
		} else {
			throw new RuntimeException("Can't convert String to 64 byte Array."
					+ stringToConvert);
		}

		return data;
	}
}
