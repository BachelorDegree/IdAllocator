#pragma once

#include "AsyncRpcHandler.hpp"
#include "Proto/idallocator.grpc.pb.h"

class AllocateIdHandler final : public AsyncRpcHandler
{
public:
    AllocateIdHandler(::idallocator::IdAllocatorService::AsyncService *service, grpc::ServerCompletionQueue *cq):
        AsyncRpcHandler(cq), service(service), responder(&ctx)
    {
        this->Proceed();
    }
    void Proceed(void) override;
    void SetInterfaceName(void) override;

private:
    ::idallocator::IdAllocatorService::AsyncService*                     service;
    ::idallocator::AllocateIdReq                                    request;
    ::idallocator::AllocateIdResp                                   response;
    grpc::ServerAsyncResponseWriter<::idallocator::AllocateIdResp>  responder;
};
