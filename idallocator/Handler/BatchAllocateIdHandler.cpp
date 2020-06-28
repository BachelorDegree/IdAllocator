#include "BatchAllocateIdHandler.hpp"
#include "IdAllocatorServiceImpl.hpp"
void BatchAllocateIdHandler::SetInterfaceName(void)
{
    interfaceName = "IdAllocatorService.BatchAllocateId";
}

void BatchAllocateIdHandler::Proceed(void)
{
    switch (status)
    {
    case Status::CREATE:
        this->SetStatusProcess();
        service->RequestBatchAllocateId(&ctx, &request, &responder, cq, cq, this);
        break;
    case Status::PROCESS:
    {
        // Firstly, spawn a new handler for next incoming RPC call
        new BatchAllocateIdHandler(service, cq);
        this->BeforeProcess();
        // Implement your logic here
        int iRet = IdAllocatorServiceImpl::GetInstance()->BatchAllocateId(request, response);
        this->SetReturnCode(iRet);
        this->SetStatusFinish();
        responder.Finish(response, grpc::Status::OK, this);
        break;
    }
    case Status::FINISH:
        delete this;
        break;
    default:
        // throw exception
        ;
    }
}

