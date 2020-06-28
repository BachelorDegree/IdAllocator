#include "AllocateIdHandler.hpp"
#include "IdAllocatorServiceImpl.hpp"
void AllocateIdHandler::SetInterfaceName(void)
{
    interfaceName = "IdAllocatorService.AllocateId";
}

void AllocateIdHandler::Proceed(void)
{
    switch (status)
    {
    case Status::CREATE:
        this->SetStatusProcess();
        service->RequestAllocateId(&ctx, &request, &responder, cq, cq, this);
        break;
    case Status::PROCESS:
    {
        // Firstly, spawn a new handler for next incoming RPC call
        new AllocateIdHandler(service, cq);
        this->BeforeProcess();
        // Implement your logic here
        int iRet = IdAllocatorServiceImpl::GetInstance()->AllocateId(request, response);
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

