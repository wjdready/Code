#include <QtWidgets/QApplication>
#include <QtCore/QDebug>
#include "MyClass.h"
#include <QElapsedTimer>
#include <QThread>
#include "Worker.h"

void MyClass::start(void)
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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MyClass m;
    m.start();

    return app.exec();
}
