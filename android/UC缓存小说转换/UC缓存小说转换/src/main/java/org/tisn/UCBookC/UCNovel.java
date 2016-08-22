package org.tisn.UCBookC;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.charset.Charset;
import java.util.HashMap;

import com.ta.common.Arrays;

public class UCNovel extends RandomAccessFile {

	private java.util.HashMap<String, int[]> mList;

	public UCNovel(String path) throws IOException {
		super(path, "r");
		byte[] tmp = new byte[4];
		this.read(tmp);
		if (!Arrays.equals(tmp, new byte[] { 15, 0, 0, 0 })) {
			throw new IOException();
		}
		tmp = new byte[15];
		this.read(tmp);
		if (!new String(tmp).equals("novel_data_v1.0")) {
			throw new IOException();
		}
		GetList();
	}

	public final void GetList() {
		try {
			mList = new HashMap<String, int[]>();
			this.seek(19);
			this.skipBytes(4);
			int f = -1;
			byte[] l = new byte[4];
			byte[] tmp;
			String name;
			int ll = 0;
			while (true) {
				this.skipBytes(4);
				this.read(l, 0, 4);
				ll = byteToInt2(l);
				tmp = new byte[ll];
				this.read(tmp, 0, ll);
				name = new String(tmp);
				this.read(l, 0, 4);
				int[] pos = new int[2];
				pos[0] = byteToInt2(l);
				if (f == -1) {
					f = pos[0];
				}
				this.read(l, 0, 4);
				pos[1] = byteToInt2(l);
				mList.put(name, pos);
				if (pos[1] == this.length() - 1) {
					break;
				}
			}
		} catch (Exception e) {

		}
	}

	private static int byteToInt2(byte[] b) {

		int mask = 0xff;
		int temp = 0;
		int n = 0;
		for (int i = 3; i >= 0; i--) {
			n <<= 8;
			temp = b[i] & mask;
			n |= temp;
		}
		return n;
	}

	public final String GetContent(String content_key) {
		try {
			int[] pos = mList.get(content_key);
			this.seek(pos[0]);
			int len = pos[1] - pos[0] + 1;
			byte[] tmp = new byte[len];
			this.read(tmp, 0, len);
			String content = new String(tmp, Charset.forName("UTF8"));
			return content;
		} catch (Exception e) {
			return null;
		}
	}

}
