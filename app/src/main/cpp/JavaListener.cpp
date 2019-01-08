//
// Created by yangw on 2018-2-25.
//

#include "JavaListener.h"


void JavaListener::onError(int type, int code, const char *msg) {

    if(type == 0)    // 子线程   JniEnv是线程相关的 所以子线程不能使用创建线程的JniEnv,JVM 是进程相关的，所以可以使用他来创建JniEnv
    {
        JNIEnv *env;
        jvm->AttachCurrentThread(&env, 0);
        jstring jmsg = env->NewStringUTF(msg);
        env->CallVoidMethod(jobj, jmid, code, jmsg);
        env->DeleteLocalRef(jmsg);

        jvm->DetachCurrentThread();


    }
    else if(type == 1)    // 主线程   可以直接拿到 jenv   然后调用java层的方法
    {
        jstring jmsg = jenv->NewStringUTF(msg);
        jenv->CallVoidMethod(jobj, jmid, code, jmsg);
        jenv->DeleteLocalRef(jmsg);
    }
}

JavaListener::JavaListener(JavaVM *vm, _JNIEnv *env, jobject obj) {

    jvm = vm;
    jenv = env; // 通过这个获取 jenv的方法
    jobj = obj;

    jclass clz = env->GetObjectClass(jobj);
    if(!clz)
    {
        return;
    }
    jmid = env->GetMethodID(clz, "onError", "(ILjava/lang/String;)V");  // 获取到 Java 层到onError
    if(!jmid)
        return;


}
