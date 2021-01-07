/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/basic/pimpl.h>

#include <geode/common/helpers/propagation_settings.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( TetrahedralSolid );
    ALIAS_3D( TetrahedralSolid );
} // namespace geode

namespace geode
{
    class geode_common_helpers_api TetrahedraClosestThan
        : public PropagationSettings
    {
    public:
        TetrahedraClosestThan( const TetrahedralSolid3D& solid,
            index_t vertex_from,
            double distance_max );

        ~TetrahedraClosestThan();

        std::vector< index_t > initialize() override;

        bool validate( index_t element ) override;

        bool stop() override;

        const std::vector< index_t >& matched_elements() const;

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };

} // namespace geode
