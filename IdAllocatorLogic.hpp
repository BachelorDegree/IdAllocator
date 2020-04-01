#pragma once
#include <string>
#include <cstdint>
#include <vector>
class IdAllocatorLogic{
  public:
    static int Allocate(const std::string & strNamespace, const uint64_t cnt, std::vector<uint64_t> & vecIds);
  private:
    static int _Allocate(const std::string & strNamespace, const uint64_t cnt, uint64_t & ret_cnt, std::vector<uint64_t> & vecIds);
    static int _AllocateFromStable(const std::string & strNamespace, const uint64_t cnt, uint64_t & start);
};