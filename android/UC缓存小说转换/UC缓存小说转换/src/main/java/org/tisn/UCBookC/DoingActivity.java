package org.tisn.UCBookC;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.os.Environment;
import android.view.KeyEvent;

import org.tisn.UCBookC.umd.Umd;
import org.tisn.UCBookC.umd.UmdHeader;

import java.io.BufferedOutputStream;
import java.io.FileOutputStream;

/**
 * Created by Administrator on 2015/2/23.
 */
public class DoingActivity extends Activity {

    private static String TAG = "DoingActivity";

    private Thread mThread = new Thread() {
        @Override
        public void run() {
            TLog.i(TAG, "开始线程");
            doing();
            TLog.i(TAG, "结束线程");
            setResult(321);
            finish();
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.doing);
        mThread.start();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if(keyCode == KeyEvent.KEYCODE_BACK){
            return false;
        }
        return super.onKeyDown(keyCode, event);
    }

    private void doing(){
        String sPath = Environment.getExternalStorageDirectory()
                .getAbsolutePath() + "/books/";
        DatabaseContext dc = new DatabaseContext(this);
        BooksDB bd = new BooksDB(dc);
        Intent intent = getIntent();
        String[] bis = intent.getStringArrayExtra("BookIDs");
        for(String bookID: bis){
            try {
                Cursor cs = bd.getBookInfo(bookID);
                Umd umd = new Umd();
                UCNovel ucn = new UCNovel(Environment
                        .getExternalStorageDirectory().getAbsolutePath()
                        + "/UCDownloads/novels/"
                        + bookID
                        + "/"
                        + bookID
                        + ".ucnovel");
                UmdHeader uh = umd.getHeader();
                cs.moveToFirst();
                uh.setTitle(cs.getString(1));
                uh.setAuthor(cs.getString(2));
                String tName = cs.getString(3);
                Cursor cp = bd.getChapters(tName);
                String contentID = "";
                while (cp.moveToNext()) {
                    contentID = cp.getString(9);
                    String content = cp.getString(2) + "\r\n"
                            + ucn.GetContent(contentID) + "\r\n";
                    umd.getChapters().addChapter(cp.getString(2), content);
                }
                FileOutputStream fos = new FileOutputStream(sPath
                        + uh.getTitle() + ".umd");
                try {
                    BufferedOutputStream bos = new BufferedOutputStream(fos);
                    umd.buildUmd(bos);
                    bos.flush();
                } finally {
                    fos.close();
                }
                ucn.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
