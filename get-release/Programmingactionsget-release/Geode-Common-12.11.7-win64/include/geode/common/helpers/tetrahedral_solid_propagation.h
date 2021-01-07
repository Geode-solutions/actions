/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/common/helpers/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( TetrahedralSolid );
    ALIAS_3D( TetrahedralSolid );
    class PropagationSettings;
} // namespace geode

namespace geode
{
    void geode_common_helpers_api perform_propagation(
        const TetrahedralSolid3D& solid, PropagationSettings& settings );
} // namespace geode
