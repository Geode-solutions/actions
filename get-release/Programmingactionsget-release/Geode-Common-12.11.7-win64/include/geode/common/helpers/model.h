/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <absl/container/fixed_array.h>

#include <geode/basic/uuid.h>

#include <geode/common/helpers/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( Line );
    ALIAS_3D( Line );
    class BRep;
    struct MeshComponentVertex;
} // namespace geode

namespace geode
{
    struct SidedSurface
    {
        static constexpr index_t POSITIVE{ 0 };
        static constexpr index_t NEGATIVE{ 1 };

        SidedSurface() = default;
        SidedSurface( uuid id_in, index_t side_in )
            : id{ std::move( id_in ) }, side{ side_in }
        {
        }

        bool operator==( const SidedSurface& other ) const
        {
            return id == other.id && side == other.side;
        }

        uuid id;
        index_t side{ 0 };
    };

    struct geode_common_helpers_api SortedSurfaces
    {
        SortedSurfaces( index_t nb_surfaces );

        const SidedSurface& next( const SidedSurface& surface ) const;

        absl::FixedArray< SidedSurface > surfaces;
    };

    SortedSurfaces geode_common_helpers_api surface_radial_sort(
        const BRep& brep, const Line3D& line );

    std::vector< std::pair< MeshComponentVertex, MeshComponentVertex > >
        geode_common_helpers_api common_vertices(
            const std::array< std::vector< MeshComponentVertex >, 2 >&
                vertices );
} // namespace geode
