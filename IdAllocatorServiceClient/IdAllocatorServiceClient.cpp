#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <coredeps/SatelliteClient.hpp>
#include <coredeps/ContextHelper.hpp>
#include "IdAllocatorServiceClient.hpp"
IdAllocatorServiceClient::IdAllocatorServiceClient()
{
  m_pChannel = GetChannel();
}
std::shared_ptr<grpc::Channel> IdAllocatorServiceClient::GetChannel()
{
  const std::string strServiceName = "IdAllocatorService";
  std::string strServer = SatelliteClient::GetInstance().GetNode(strServiceName);
  return grpc::CreateChannel(strServer, grpc::InsecureChannelCredentials());
}
int IdAllocatorServiceClient::AllocateId(const ::idallocator::AllocateIdReq & oReq, ::idallocator::AllocateIdResp & oResp)
{
  ::idallocator::IdAllocatorService::Stub oStub{m_pChannel};
  grpc::ClientContext oContext;
  auto oStatus = oStub.AllocateId(&oContext, oReq, &oResp);
  if (oStatus.ok() == false)
  {
    return -1;
  }
  int iRet = ClientContextHelper(oContext).GetReturnCode();
  if(iRet != 0){
    for(auto a :oContext.GetServerTrailingMetadata()){
      std::cout<<a.first << " " << a.second << std::endl;
    }
  }
  return ClientContextHelper(oContext).GetReturnCode();
}
int IdAllocatorServiceClient::BatchAllocateId(const ::idallocator::BatchAllocateIdReq & oReq, ::idallocator::BatchAllocateIdResp & oResp)
{
  ::idallocator::IdAllocatorService::Stub oStub{m_pChannel};
  grpc::ClientContext oContext;
  auto oStatus = oStub.BatchAllocateId(&oContext, oReq, &oResp);
  if (oStatus.ok() == false)
  {
    return -1;
  }
  return ClientContextHelper(oContext).GetReturnCode();
}
