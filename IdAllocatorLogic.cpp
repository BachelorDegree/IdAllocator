#include <mutex>
#include <cstdio>
#include <iostream>
#include <map>
#include <inttypes.h>
#include <colib/co_routine.h>
#include <colib/sync/co_mutex.h>
#include <colib/co_routine_specific.h>
#include "PaxosKVServiceClient/PaxosKVServiceClient.hpp"
#include "IdAllocatorLogic.hpp"
libco::CoMutex g_oMutex;
struct IdAllocatorCache
{
  std::map<std::string, std::pair<uint64_t, uint64_t>> *pMap;
};
CO_ROUTINE_SPECIFIC(IdAllocatorCache, g_coIdAllocatorCache);
int IdAllocatorLogic::_Allocate(const std::string &strNamespace, const uint64_t cnt, uint64_t &ret_cnt, std::vector<uint64_t> &vecIds)
{
  uint64_t cacheCnt = 10000;
  if (g_coIdAllocatorCache->pMap == nullptr)
  {
    
    g_coIdAllocatorCache->pMap = new std::map<std::string, std::pair<uint64_t, uint64_t>>;
  }
  int iRet = 0;
  auto & oMap = *g_coIdAllocatorCache->pMap;
  if (oMap.count(strNamespace) == 0 || oMap[strNamespace].first >= oMap[strNamespace].second)
  {
    uint64_t newStart = 0;
    cacheCnt = cacheCnt + cnt * 2;
    iRet = _AllocateFromStable(strNamespace, cacheCnt, newStart);
    if (iRet != 0)
    {
      return iRet;
    }
    oMap[strNamespace] = {newStart + cnt, newStart + cacheCnt};
    ret_cnt = cnt;
    for (uint64_t i = 0; i < ret_cnt; i++)
    {
      vecIds.push_back(i + newStart);
    }
    return 0;
  }
  auto &entry = oMap[strNamespace];
  for (ret_cnt = 0; ret_cnt < cnt && entry.first < entry.second; ret_cnt++, entry.first++)
  {
    vecIds.push_back(entry.first);
  }
  return 0;
}
int IdAllocatorLogic::Allocate(const std::string &strNamespace, const uint64_t cnt, std::vector<uint64_t> &vecIds)
{
  uint64_t cur_cnt = cnt;
  while (cur_cnt > 0)
  {
    uint64_t ret_cnt = 0;
    int iRet = _Allocate(strNamespace, cur_cnt, ret_cnt, vecIds);
    if (iRet != 0)
    {
      return iRet;
    }
    cur_cnt -=ret_cnt;
  }
  return 0;
}
int IdAllocatorLogic::_AllocateFromStable(const std::string &strNamespace, const uint64_t cnt, uint64_t &start)
{
  std::cout<<"reallocator"<<std::endl;
  std::lock_guard<libco::CoMutex> oGuard(g_oMutex);
  PaxosKVServiceClient oClient;
  uint64_t now = 0;
  do
  {
    paxoskv::GetReq oReq;
    paxoskv::GetResp oResp;
    oReq.set_key("idallocator_namespace_" + strNamespace);
    int iRet = oClient.Get(oReq, oResp);
    if (iRet == 404)
    {
      now = 0;
      break;
    }
    if (iRet != 0)
    {
      std::cerr << "allocate id failed" << std::endl;
      return iRet;
    }
    iRet = sscanf(oResp.value().c_str(), "%" PRIu64, &now);
    if (iRet != 1)
    {
      return 500;
    }
  } while (false);
  start = now;
  now = now + cnt;
  do
  {
    paxoskv::SetReq oReq;
    paxoskv::SetResp oResp;
    oReq.set_key("idallocator_namespace_" + strNamespace);
    oReq.set_value(std::to_string(now));
    int iRet = oClient.Set(oReq, oResp);
    if (iRet != 0)
    {
      std::cerr << "allocate id failed" << std::endl;
      return iRet;
    }
  } while (false);
  return 0;
}