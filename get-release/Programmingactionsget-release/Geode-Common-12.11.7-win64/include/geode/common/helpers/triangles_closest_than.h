/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/basic/pimpl.h>

#include <geode/common/helpers/propagation_settings.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( TriangulatedSurface );
} // namespace geode

namespace geode
{
    template < index_t dimension >
    class TrianglesClosestThan : public PropagationSettings
    {
    public:
        TrianglesClosestThan( const TriangulatedSurface< dimension >& surface,
            index_t vertex_from,
            double distance_max );

        ~TrianglesClosestThan();

        std::vector< index_t > initialize() override;

        bool validate( index_t element ) override;

        bool stop() override;

        const std::vector< index_t >& matched_elements() const;

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };
    ALIAS_2D_AND_3D( TrianglesClosestThan );

} // namespace geode
