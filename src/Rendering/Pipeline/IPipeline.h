
#pragma once

#include <cstdint>
#include <memory>

namespace mono
{
    class IPipeline
    {
    public:

        virtual ~IPipeline() = default;
        virtual uint32_t Id() const = 0;
        virtual void Apply() = 0;
    };

    using IPipelinePtr = std::unique_ptr<IPipeline>;
}
