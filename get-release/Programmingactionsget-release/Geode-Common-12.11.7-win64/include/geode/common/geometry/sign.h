/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/common/geometry/common.h>

namespace geode
{
    class Tetra;
    enum struct Side;
    using Sign = Side;
} // namespace geode

namespace geode
{
    /*!
     * Return the sign of tetrahedron volume.
     */
    Sign geode_common_geometry_api tetrahedron_volume_sign(
        const Tetra& tetra );
} // namespace geode
