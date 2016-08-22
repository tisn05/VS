package org.tisn.UCBookC;

import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.util.ArrayList;

import org.tisn.UCBookC.umd.Umd;
import org.tisn.UCBookC.umd.UmdHeader;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

public class MainActivity extends Activity implements OnClickListener {
    private static String TAG = "MainActivity";

    private ListView mListView;
    private Button btStart;

    private Thread mThread = new Thread() {
        @Override
        public void run() {
            TLog.i(TAG, "开始线程");
            transform();
            TLog.i(TAG, "结束线程");
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mListView = (ListView) findViewById(R.id.listView1);
        MyAdapter mMyAdapter = new MyAdapter(this);
        mListView.setAdapter(mMyAdapter);

        btStart = (Button) findViewById(R.id.button_start);
        btStart.setOnClickListener(this);
    }

    private void transform() {
        String sPath = Environment.getExternalStorageDirectory()
                .getAbsolutePath() + "/books/";
        DatabaseContext dc = new DatabaseContext(this);
        BooksDB bd = new BooksDB(dc);
        MyAdapter ma = (MyAdapter) mListView.getAdapter();
        Integer[] ss = ma.getSelecteds();
        for (Integer i : ss) {
            try {
                String bookID = ma.getBookID(i);
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

    @Override
    public void onClick(View v) {
        String sPath = Environment.getExternalStorageDirectory()
                .getAbsolutePath() + "/books/";
        switch (v.getId()) {
            case R.id.button_start:
                ArrayList<String> bis = new ArrayList<String>();
                MyAdapter ma = (MyAdapter) mListView.getAdapter();
                Integer[] ss = ma.getSelecteds();
                if(ss.length == 0){
                    Toast.makeText(this,"请至少选择一项", Toast.LENGTH_SHORT).show();
                }
                for (Integer i : ss) {
                    bis.add(ma.getBookID(i));
                }
                Intent intent = new Intent(this, DoingActivity.class);
                intent.putExtra("BookIDs", bis.toArray(new String[0]));
                startActivityForResult(intent, 123);
                break;
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

            Toast.makeText(this, "处理完成！", Toast.LENGTH_LONG).show();
    }
}
