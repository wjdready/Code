#include "Worker.h"


Worker::Worker()
{
}


Worker::~Worker()
{
}

void Worker::process() 
{
    // allocate resources using new here
    qDebug("Hello World!");
    emit finished();
}