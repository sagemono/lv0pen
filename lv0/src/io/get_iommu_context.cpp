#include "iommu.h"

iommu_context::iommu_context() : ioc_base(0), iost(0), iost_size(0), iopt(0), iopt_size(0)
{
}

iommu_context *get_iommu_context()
{
    static iommu_context context;
    return &context;
}
