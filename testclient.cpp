#include <iostream>
#include <thread>
#include<vector>
#include <atomic>
#include "IdAllocatorServiceClient/IdAllocatorServiceClient.hpp"
#include <coredeps/SatelliteClient.hpp>
#include <unistd.h>
std::atomic<uint64_t> success;
std::atomic<uint64_t> failed;
void run()
{
  IdAllocatorServiceClient oClient;
  while (true)
  {
    idallocator::AllocateIdReq oReq;
    idallocator::AllocateIdResp oResp;
    oReq.set_id_namespace("test");
    int iRet = oClient.AllocateId(oReq, oResp);
    //std::cout << IdAllocatorServiceClient().AllocateId(oReq, oResp) << oResp.ShortDebugString() << std::endl;
    if(iRet != 0){
      std::cout<<iRet<<std::endl;
      failed++;
    }else{
      success++;
    }
    
  }
}
int main()
{
    SatelliteClient::GetInstance().SetServer("10.0.0.102:5553");
  std::vector<std::thread> vec;
  
  for(int i=0;i<200;i++){
    auto a = std::thread(run);
    vec.push_back(std::move(a));
  }
  uint64_t before_success = 0;
  uint64_t before_failed = 0;
  while(true){
    before_success = success;
    before_failed = failed;
    sleep(1);
    std::cout<<success-before_success <<" " << failed - before_failed <<std::endl;
  }
  for(auto &a : vec){
    a.join();
  }
}