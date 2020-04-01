#pragma once
#include "Proto/idallocator.pb.h"
#include "PaxosKVServiceClient/PaxosKVServiceClient.hpp"

class IdAllocatorServiceImpl
{
public:
    static IdAllocatorServiceImpl *GetInstance();
    static void SetInstance(IdAllocatorServiceImpl *);
    static int BeforeServerStart(const char * czConf) {
        return 0;
    }
    int BeforeWorkerStart() {
        return 0;
    }
    int AllocateId(const ::idallocator::AllocateIdReq & oReq, ::idallocator::AllocateIdResp & oResp);
    int BatchAllocateId(const ::idallocator::BatchAllocateIdReq & oReq, ::idallocator::BatchAllocateIdResp & oResp);
private:
    PaxosKVServiceClient m_oClient;

};
