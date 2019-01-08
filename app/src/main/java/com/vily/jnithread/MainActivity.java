package com.vily.jnithread;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private ThreadDemo threadDemo;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        threadDemo = new ThreadDemo();

        // 当 C++层调用该方法的时候  会通过这里将结果监听反馈回去
        threadDemo.setOnErrerListener(new ThreadDemo.OnErrerListener() {
            @Override
            public void onError(int code, String msg) {
                Log.d("ywl5320", "code = " + code + " msg = " + msg);
            }
        });
    }

    public void normal(View view) {
        threadDemo.normalThread();
    }

    public void mutexThread(View view) {
        threadDemo.mutexThread();
    }


    // C++调用Java方法   该方法 里面集成来监听
    public void calbackThread(View view) {

        threadDemo.callbackFromC();

    }
}
