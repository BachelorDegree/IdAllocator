#pragma once

#include "AsyncRpcHandler.hpp"
#include "Proto/idallocator.grpc.pb.h"

class BatchAllocateIdHandler final : public AsyncRpcHandler
{
public:
    BatchAllocateIdHandler(::idallocator::IdAllocatorService::AsyncService *service, grpc::ServerCompletionQueue *cq):
        AsyncRpcHandler(cq), service(service), responder(&ctx)
    {
        this->Proceed();
    }
    void Proceed(void) override;
    void SetInterfaceName(void) override;

private:
    ::idallocator::IdAllocatorService::AsyncService*                     service;
    ::idallocator::BatchAllocateIdReq                                    request;
    ::idallocator::BatchAllocateIdResp                                   response;
    grpc::ServerAsyncResponseWriter<::idallocator::BatchAllocateIdResp>  responder;
};
