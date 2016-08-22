package org.tisn.UCBookC;

import android.util.Log;

/**
 * Created by Administrator on 2015/2/22.
 */
public class TLog {
    private static String TAG = "TLOG";

    private static boolean DEBUG = true;

    public static void i(String msg) {
        i(TAG, msg);
    }

    public static void i(String tag, String msg) {
        if (DEBUG)
            Log.i(tag, msg);
    }
}
