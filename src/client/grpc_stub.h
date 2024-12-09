#pragma once

#include <QtCore/QObject>

#include <grpc++/channel.h>

#include "src/proto/generated/api.grpc.pb.h"

class gRpcStub : public QObject {
    Q_OBJECT
public:
    explicit gRpcStub(std::shared_ptr<grpc::Channel> channel);
    ~gRpcStub();

    void start();

Q_SIGNALS:
    void started();
    void pingResponse();
    void pingFailed();

private Q_SLOTS:
    void cleanup();
    void proceed();

private:
    std::unique_ptr<QThread> m_thread;
    std::unique_ptr<MaintainingApi::Stub> m_stub;
};