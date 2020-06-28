#pragma once
#include <memory>
#include <string>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include "idallocator.grpc.pb.h"
#include "idallocator.pb.h"
class IdAllocatorServiceClient
{
private:
  std::shared_ptr<grpc::Channel> m_pChannel;
  std::string m_strServiceName;
public:
  IdAllocatorServiceClient();
  // User specified IpPort or CanonicalName
  IdAllocatorServiceClient(const std::string &strIpPortOrCanonicalName);
  std::shared_ptr<grpc::Channel> GetChannel();
    int AllocateId(const ::idallocator::AllocateIdReq & oReq, ::idallocator::AllocateIdResp & oResp);
    int BatchAllocateId(const ::idallocator::BatchAllocateIdReq & oReq, ::idallocator::BatchAllocateIdResp & oResp);
};
