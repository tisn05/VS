package org.tisn.UCBookC;

import java.io.File;

import android.content.Context;
import android.content.ContextWrapper;
import android.database.DatabaseErrorHandler;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.os.Environment;

public class DatabaseContext extends ContextWrapper {

	public DatabaseContext(Context base) {
		super(base);
	}

	@Override
	public File getDatabasePath(String name) {
		boolean sdExist = Environment.MEDIA_MOUNTED.equals(Environment
				.getExternalStorageState());
		if (!sdExist) {
			return null;
		} else {
			String dbDir = Environment.getExternalStorageDirectory()
					.getAbsolutePath();
			dbDir += "/UCDownloads/novels/";
			String dbPath = dbDir + name;
			File dirFile = new File(dbDir);
			boolean b = dirFile.exists();
			if (!b) {
				return null;
			} else {
				File dbFile = new File(dbPath);
				if (!dbFile.exists()) {
					return null;
				} else
					return dbFile;
			}
		}
	}

	@Override
	public SQLiteDatabase openOrCreateDatabase(String name, int mode,
			CursorFactory factory) {
		SQLiteDatabase result = SQLiteDatabase.openOrCreateDatabase(
				getDatabasePath(name), null);
		return result;
	}

	@Override
	public SQLiteDatabase openOrCreateDatabase(String name, int mode,
			CursorFactory factory, DatabaseErrorHandler errorHandler) {
		SQLiteDatabase result = SQLiteDatabase.openOrCreateDatabase(
				getDatabasePath(name), null);
		return result;
	}
}
