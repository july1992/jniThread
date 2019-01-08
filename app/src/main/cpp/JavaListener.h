//
// Created by yangw on 2018-2-25.
//

#include "jni.h"

#ifndef JNITHREAD_JAVALISTENER_H
#define JNITHREAD_JAVALISTENER_H


class JavaListener {

public:
    JavaVM *jvm;    // 保存 虚拟机
    _JNIEnv *jenv;  // 线程环境
    jobject jobj;   // 对象  全局队
    jmethodID jmid; // 得到Java 层里面  onError
public:
    JavaListener(JavaVM *vm, _JNIEnv *env, jobject obj);
    ~JavaListener();   // 回收对象队时候  会走这里

    /**
     * 1:主线程
     * 0：子线程
     * @param type
     * @param code
     * @param msg
     */
    void onError(int type, int code, const char *msg);


};


#endif //JNITHREAD_JAVALISTENER_H
