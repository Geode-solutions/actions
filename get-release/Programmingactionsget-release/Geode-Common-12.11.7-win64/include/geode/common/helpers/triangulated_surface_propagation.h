/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/common/helpers/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( TriangulatedSurface );
    class PropagationSettings;
} // namespace geode

namespace geode
{
    template < index_t dimension >
    void perform_propagation( const TriangulatedSurface< dimension >& surface,
        PropagationSettings& settings );
} // namespace geode
