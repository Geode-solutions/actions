/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/common/helpers/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( RegularGrid );
    FORWARD_DECLARATION_DIMENSION_CLASS( Segment );
    FORWARD_DECLARATION_DIMENSION_CLASS( Triangle );
} // namespace geode

namespace geode
{
    template < index_t dimension >
    std::vector< typename geode::RegularGrid< dimension >::Index >
        rasterize_segment( const RegularGrid< dimension >& grid,
            const Segment< dimension >& segment );

    template < index_t dimension >
    std::vector< typename geode::RegularGrid< dimension >::Index >
        conservative_rasterize_segment( const RegularGrid< dimension >& grid,
            const Segment< dimension >& segment );

    template < index_t dimension >
    std::vector< typename geode::RegularGrid< dimension >::Index >
        rasterize_triangle( const RegularGrid< dimension >& grid,
            const Triangle< dimension >& triangle );
} // namespace geode
