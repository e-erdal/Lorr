/// This file will include supports, wrappers between libraries

#include "Common.hh"

/// FMT support for EASTL
template<>
struct fmt::formatter<eastl::string> : formatter<std::string>
{
    template<typename FormatContext>
    eastl::string format(const eastl::string &str, FormatContext &ctx)
    {
        return format_to(ctx.out(), "{}", str.c_str());
    }

    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }
};

/// EA ALLOCATOR
void *operator new[](size_t size, const char *name, int flags, unsigned debugFlags, const char *file, int line)
{
    return new uint8_t[size];
}

void *operator new[](size_t size, size_t alignment, size_t offset, const char *name, int flags, unsigned debugFlags, const char *file, int line)
{
    size_t adjustedAlignment = (alignment > EA_PLATFORM_PTR_SIZE) ? alignment : EA_PLATFORM_PTR_SIZE;

    void *p = new char[size + adjustedAlignment + EA_PLATFORM_PTR_SIZE];
    void *pPlusPointerSize = (void *)((uintptr_t)p + EA_PLATFORM_PTR_SIZE);
    void *pAligned = (void *)(((uintptr_t)pPlusPointerSize + adjustedAlignment - 1) & ~(adjustedAlignment - 1));

    void **pStoredPtr = (void **)pAligned - 1;
    EASTL_ASSERT(pStoredPtr >= p);
    *(pStoredPtr) = p;

    EASTL_ASSERT(((size_t)pAligned & ~(alignment - 1)) == (size_t)pAligned);

    return pAligned;
}