/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/common/helpers/common.h>

namespace geode
{
    class geode_common_helpers_api PropagationSettings
    {
    public:
        virtual ~PropagationSettings() = default;

        virtual std::vector< index_t > initialize() = 0;

        virtual bool validate( index_t element ) = 0;

        virtual bool stop() = 0;
    };
} // namespace geode
