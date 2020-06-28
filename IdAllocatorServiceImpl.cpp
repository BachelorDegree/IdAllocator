#include <vector>
#include <poll.h>
#include "IdAllocatorServiceImpl.hpp"
#include "IdAllocatorLogic.hpp"
#include "IdAllocatorServiceClient/IdAllocatorServiceClient.hpp"
int IdAllocatorServiceImpl::AllocateId(const ::idallocator::AllocateIdReq &oReq, ::idallocator::AllocateIdResp &oResp)
{
    std::vector<uint64_t> vecRet;
    int iRet = IdAllocatorLogic::Allocate(oReq.id_namespace(), 1, vecRet);
    if (iRet != 0)
    {
        std::cout << iRet << std::endl;
        return iRet;
    }
    oResp.set_id(vecRet[0]);
    return iRet;
}
int IdAllocatorServiceImpl::BatchAllocateId(const ::idallocator::BatchAllocateIdReq &oReq, ::idallocator::BatchAllocateIdResp &oResp)
{
    std::vector<uint64_t> vecRet;
    std::cout << "1" << std::endl;
    int iRet = IdAllocatorLogic::Allocate(oReq.id_namespace(), oReq.count(), vecRet);
    if (iRet != 0)
    {
        return iRet;
    }
    for (auto id : vecRet)
    {
        oResp.add_ids(id);
    }
    return 0;
}
