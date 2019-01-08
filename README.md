# jniThread         
Java 与 C++ 通过 jni  创建线程，并方法互调    （感谢杨万里老师的视频～）


### 01:最简单的创建线程   normalCallBack  里面是回调
```
pthread_t thread;

void *normalCallBack(void * data)
{
    LOGD("------------create normal thread from C++!");
    pthread_exit(&thread);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_vily_jnithread_ThreadDemo_normalThread(JNIEnv *env, jobject instance) {

    pthread_create(&thread, NULL, normalCallBack, NULL);  // 创建一个线程
    // TODO


}
```
### 02:  C++调用java 层方法 ， 在Java层写一个 onError方法，方法下有一个回调，当c++调用这个方法当时候，回调监听会打印出来
####  001: java 层的方法
```
 public native  void callbackFromC();


    //--------------c++调用的方法-------------//
    public void onError(int code, String msg)
    {
        if(onErrerListener != null)
        {
            onErrerListener.onError(code, msg);
        }
    }

    //---------------监听-----------------------//
    private OnErrerListener onErrerListener;

    public void setOnErrerListener(OnErrerListener onErrerListener) {
        this.onErrerListener = onErrerListener;
    }
    public interface OnErrerListener
    {
        void onError(int code, String msg);
    }
```
####  002: 在 cpp文件下创建  ThreadListener 监听的C++文件  然后在 callbackFromC()下使用
```

#include "JavaListener.h"
JavaVM *jvm;

JavaListener *javaListener;

pthread_t chidlThread;


void *childCallback(void *data)   // 在线程里调用Java 里面的方法。Java层的该方法 注册了回调监听
{
    JavaListener *javaListener1 = (JavaListener *) data;

    javaListener1->onError(0, 101, "---------------哈哈哈哈哈哈");
    pthread_exit(&chidlThread);
}

// C++ 调用java方法     public void onError(int code, String msg)
extern "C"
JNIEXPORT void JNICALL
Java_com_vily_jnithread_ThreadDemo_callbackFromC(JNIEnv *env, jobject instance) {

    // JniEnv是线程相关的    JniEnv是线程相关的   env->NewGlobalRef(instance)：初始化全局的
    // TODO
    javaListener = new JavaListener(jvm, env, env->NewGlobalRef(instance));
    //javaListener->onError(1, 100, "c++ call java meid from main thread!");  // 使用主线程调用Java层方法
    pthread_create(&chidlThread, NULL, childCallback, javaListener);  //  使用子线程调用


}

// 获取 JVM 对象   ，该对象是进程相关的   JniEnv 是线程相关的线程环境
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void* reserved)
{
    JNIEnv *env;
    jvm = vm;
    if(vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK)
    {
        return -1;
    }
    return JNI_VERSION_1_6;
}
```
