#include <colib/co_routine_specific.h>
#include "IdAllocatorServiceImpl.hpp"
struct __IdAllocatorServiceImplWrapper{
    IdAllocatorServiceImpl * pImpl;
};
CO_ROUTINE_SPECIFIC(__IdAllocatorServiceImplWrapper, g_coIdAllocatorServiceImplWrapper)
IdAllocatorServiceImpl *IdAllocatorServiceImpl::GetInstance()
{
    return g_coIdAllocatorServiceImplWrapper->pImpl;
}
void IdAllocatorServiceImpl::SetInstance(IdAllocatorServiceImpl *pImpl)
{
    g_coIdAllocatorServiceImplWrapper->pImpl = pImpl;
}
