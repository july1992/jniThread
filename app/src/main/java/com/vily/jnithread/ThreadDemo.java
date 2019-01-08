package com.vily.jnithread;

/**
 * Created by yangw on 2018-2-24.
 */

public class ThreadDemo {

    static
    {
        System.loadLibrary("native-lib");
    }


    public native void normalThread();

    public native void mutexThread();


    private OnErrerListener onErrerListener;

    public void setOnErrerListener(OnErrerListener onErrerListener) {
        this.onErrerListener = onErrerListener;
    }

    public void onError(int code, String msg)
    {
        if(onErrerListener != null)
        {
            onErrerListener.onError(code, msg);
        }
    }

    public interface OnErrerListener
    {
        void onError(int code, String msg);
    }

    public native  void callbackFromC();

}
