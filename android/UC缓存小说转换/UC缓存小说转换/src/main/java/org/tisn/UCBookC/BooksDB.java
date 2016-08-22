package org.tisn.UCBookC;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class BooksDB extends SQLiteOpenHelper {

	private static String DATABASE_NAME = "com.UCMobile_catalog";

	public BooksDB(Context context) {
		super(context, DATABASE_NAME, null, 2);
	}

	public Cursor getAll() {
		SQLiteDatabase db = this.getReadableDatabase();
		return db.query("CATALOG_TABLE", null, null, null, null, null, null);
	}

	public Cursor getBookInfo(String bookID) {
		SQLiteDatabase db = this.getReadableDatabase();
		String sql = "SELECT * FROM CATALOG_TABLE WHERE book_id=?";
		return db.rawQuery(sql, new String[] { bookID });
	}

	public Cursor getChapters(String tName) {
		SQLiteDatabase db = this.getReadableDatabase();
		return db.query(tName, null, null, null, null, null, null);
	}

	@Override
	public void onCreate(SQLiteDatabase arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onUpgrade(SQLiteDatabase arg0, int arg1, int arg2) {
		// TODO Auto-generated method stub

	}

}
