#include "dylib_export.h"
#include "Proto/idallocator.grpc.pb.h"
#include "IdAllocatorServiceImpl.hpp"
#include "Handler/AllocateIdHandler.hpp"
#include "Handler/BatchAllocateIdHandler.hpp"

::idallocator::IdAllocatorService::AsyncService service;

const char * EXPORT_Description(void)
{
    return "idallocator";
}

void EXPORT_DylibInit(const char * conf_file)
{
    IdAllocatorServiceImpl::BeforeServerStart(conf_file);
}

grpc::Service * EXPORT_GetGrpcServiceInstance(void)
{
    return &service;
}
void EXPORT_OnCoroutineWorkerStart(void)
{
    IdAllocatorServiceImpl::SetInstance(new IdAllocatorServiceImpl);
    IdAllocatorServiceImpl::GetInstance()->BeforeWorkerStart();
} 
void EXPORT_OnWorkerThreadStart(grpc::ServerCompletionQueue *cq)
{
  // Bind handlers

    new AllocateIdHandler(&service, cq);
    new BatchAllocateIdHandler(&service, cq);
}
