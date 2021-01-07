/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/common/cutter/solid/common.h>
#include <geode/common/modifier/common/information.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( TetrahedralSolid );
    FORWARD_DECLARATION_DIMENSION_CLASS( TetrahedralSolidBuilder );
    ALIAS_3D( TetrahedralSolid );
    ALIAS_3D( TetrahedralSolidBuilder );
    struct SolidPath;
    using VertexMultiMapping = MultiMapping< index_t >;
    using EdgeMultiMapping = MultiMapping< index_t >;
    using OrientedEdgeMultiMapping = std::pair< EdgeMultiMapping, bool >;
    using FacetMultiMapping = MultiMapping< index_t >;
    using TetrahedronMapping = Mapping< index_t >;
} // namespace geode

namespace geode
{
    struct SolidCutPathInfo
    {
        std::vector< VertexMultiMapping > vertices;
        std::vector< OrientedEdgeMultiMapping > along_path;
        std::vector< EdgeMultiMapping > across_path;
        std::vector< FacetMultiMapping > facets;
        std::vector< TetrahedronMapping > tetrahedra;
    };

    /*!
     * Perform a TetrahedralSolid3D rectilinear cut between two vertices
     * given where to split the tetrahedra.
     * @param[in] begin Index of the cut starting vertex
     * @param[in] end Index of the cut ending vertex
     * @param[in] path_splits Ordered list of Path splits
     * @warn Cut tetrahedra are set as inactive but not deleted.
     */
    SolidCutPathInfo geode_common_cutter_solid_api cut_along_path(
        const TetrahedralSolid3D& solid,
        TetrahedralSolidBuilder3D& builder,
        index_t begin,
        index_t end,
        absl::Span< const SolidPath > path_splits );

    SolidCutPathInfo geode_common_cutter_solid_api cut_along_path(
        const TetrahedralSolid3D& solid,
        TetrahedralSolidBuilder3D& builder,
        index_t begin,
        index_t end );
} // namespace geode