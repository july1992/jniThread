#include <jni.h>
#include <string>
#include "pthread.h"
#include "AndroidLog.h"


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


#include "queue"
#include "unistd.h"

pthread_t produc;      // 线程   生产者
pthread_t custom;      // 线程   消费者
pthread_mutex_t mutex; // 锁对象
pthread_cond_t cond;   // 线程条件对象
bool exit= false;   // 是否退出

std::queue<int> queue; // 队列


void *producCallback(void *data)    // 生产者线程回调
{

    while (!exit)
    {
        pthread_mutex_lock(&mutex);  // 给线程加锁  防止线程在入队时 同时在出队

        queue.push(1);
        LOGD("--------------生产者生产一个产品，通知消费者消费， 产品数量为 %d", queue.size());
        pthread_cond_signal(&cond);   // 发出一个生产信号
        pthread_mutex_unlock(&mutex); // 解锁
        sleep(5);
    }


    pthread_exit(&produc);    // 退出线程
}

void *customCallback(void *data)    // 消费者线程回调
{
    while (!exit)
    {
        pthread_mutex_lock(&mutex);

        if(queue.size() > 0)
        {
            queue.pop();
            LOGD("-------------消费者消费产品，产品数量还剩余 %d ", queue.size());
        } else{
            LOGD("-------------没有产品可以消费， 等待中...");
            pthread_cond_wait(&cond, &mutex);   // 等待时 自动解锁
        }
        pthread_mutex_unlock(&mutex);
        usleep(500 * 1000);
    }
    pthread_exit(&custom);     // 退出线程
}

extern "C"
JNIEXPORT void JNICALL
Java_com_vily_jnithread_ThreadDemo_mutexThread(JNIEnv *env, jobject instance) {

    for(int i = 0; i < 10; i++)
    {
        queue.push(1);
    }
    // TODO
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&produc, NULL, producCallback, NULL);
    pthread_create(&custom, NULL, customCallback, NULL);


}


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