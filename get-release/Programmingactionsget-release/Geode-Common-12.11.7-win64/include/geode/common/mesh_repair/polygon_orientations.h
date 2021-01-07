/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/common/mesh_repair/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( SurfaceMesh );
} // namespace geode

namespace geode
{
    template < index_t dimension >
    void repair_polygon_bad_orientations( SurfaceMesh< dimension >& mesh );
} // namespace geode
