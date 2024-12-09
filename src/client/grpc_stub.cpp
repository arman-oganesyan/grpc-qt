#include "grpc_stub.h"

#include <QtCore/QThread>
#include <QtWidgets/QApplication>

gRpcStub::gRpcStub(std::shared_ptr<grpc::Channel> channel)
    : m_stub(MaintainingApi::NewStub(channel))
{
    m_thread = std::make_unique<QThread>();
    moveToThread(m_thread.get());

    connect(m_thread.get(), &QThread::started, this, &gRpcStub::proceed);
}

gRpcStub::~gRpcStub() {
    QMetaObject::invokeMethod(this, "cleanup");
    m_thread->wait();
}

void gRpcStub::start() {
    m_thread->start();
}

void gRpcStub::cleanup()
{
    m_thread->requestInterruption();
    m_thread->quit();
}

void gRpcStub::proceed()
{
    QElapsedTimer timer;

    while (!m_thread->isInterruptionRequested()) {
        QApplication::processEvents();

        if (timer.isValid() && timer.elapsed() < 5000)
            continue;

        PingRequest request;
        request.set_clientip("What should be here?");

        PingResponse response;

        grpc::ClientContext context;

        if (auto status = m_stub->Ping(&context, request, &response); status.ok()) {
            emit pingResponse();
        }
        else {
            emit pingFailed();
        }

        timer.start();
    }
}
