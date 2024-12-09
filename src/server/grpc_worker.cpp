#include "grpc_worker.h"

gRpcWorker::gRpcWorker(QList<QNetworkAddressEntry> addresses, quint16 port)
    : m_apiImpl(*this)
    , m_addresses(addresses)
    , m_port(port)
{
    m_thread = std::make_unique<QThread>();
    moveToThread(m_thread.get());

    connect(m_thread.get(), &QThread::started, this, &gRpcWorker::proceed);
}

gRpcWorker::~gRpcWorker()
{
    QMetaObject::invokeMethod(this, "cleanup");
    m_thread->wait();
}

void gRpcWorker::start()
{
    m_thread->start();
}

void gRpcWorker::cleanup()
{
    m_server->Shutdown();
    m_thread->quit();
}

void gRpcWorker::proceed()
{
    grpc::ServerBuilder builder{};

    for (const auto& networkAddressEntry : qAsConst(m_addresses))
    {
        auto address = QString{ "%0:%1" }.arg(networkAddressEntry.ip().toString()).arg(m_port);
        builder.AddListeningPort(address.toStdString(), grpc::InsecureServerCredentials());
    }

    builder.RegisterService(&m_apiImpl);
    m_server = builder.BuildAndStart();
    emit started();
}