#include "MyClass.h"
#include "Worker.h"
#include <QtCore/QDebug>
#include <QThread>

void MyClass::startTest()
{
    QThread *thread = new QThread;
    Worker *worker = new Worker;

    worker->moveToThread(thread);

    /* 线程 start 后开始处理 */
    connect(thread, QThread::started, worker, Worker::process);

    /* 线程结束后通知 worker 删除 */
    connect(thread, QThread::finished, worker, Worker::deleteLater);

    /* Worker 结束后通知线程退出和删除 */
    connect(worker, Worker::finished, thread, QThread::quit);
    connect(worker, Worker::finished, thread, QThread::deleteLater);

    thread->start();
}


void MyClass::startTest2()
{
    QThread *thread = new QThread;

    moveToThread(thread);

    connect(thread, QThread::started, this, process);

    /* 线程结束后通知 worker 删除 */
    // connect(thread, QThread::finished, this, Worker::deleteLater);

    /* 结束后通知线程退出和删除 */
    connect(this, finished, thread, QThread::quit);
    connect(this, finished, thread, QThread::deleteLater);

    thread->start();
}

void MyClass::process() 
{
    // allocate resources using new here
    qDebug("Hello World!");
    emit finished();
}

int main(void)
{
    MyClass m;
    m.startTest();
    m.startTest2();

    getchar();

    return 0;
}
