#pragma once

#include <QtCore/QThread>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkInterface>

#include <memory>

#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>

#include <src/proto/generated/api.grpc.pb.h>


class gRpcWorker : public QObject {
    Q_OBJECT
private:
    class MaintainingApiImpl final : public MaintainingApi::Service {
    public:
        MaintainingApiImpl(gRpcWorker& worker) : m_worker(worker) {}

        grpc::Status Ping(grpc::ServerContext* context, const PingRequest* /*request*/, PingResponse* /*response*/) override {
            emit m_worker.request(QString::fromStdString(context->peer()));
            return grpc::Status::OK;
        }

    private:
        gRpcWorker& m_worker;
    };
public:
    gRpcWorker(QList<QNetworkAddressEntry> addresses, quint16 port);
    ~gRpcWorker();

    void start();

Q_SIGNALS:
    void started();
    void request(const QString&);

private Q_SLOTS:
    void cleanup();
    void proceed();


private:
    MaintainingApiImpl m_apiImpl;
    std::unique_ptr<QThread> m_thread;
    std::unique_ptr<grpc::Server> m_server;
    QList<QNetworkAddressEntry> m_addresses;
    quint16 m_port;
};