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
    FORWARD_DECLARATION_DIMENSION_CLASS( TriangulatedSurface );
    FORWARD_DECLARATION_DIMENSION_CLASS( Triangle );
    ALIAS_3D( TetrahedralSolid );
    ALIAS_3D( TetrahedralSolidBuilder );
    ALIAS_3D( TriangulatedSurface );
    ALIAS_3D( Triangle );
    struct PatchFrontEdge;
    using VertexMultiMapping = MultiMapping< index_t >;
    using EdgeMapping = Mapping< index_t >;
    using OrientedEdgeMapping = std::pair< EdgeMapping, bool >;
    using EdgeMultiMapping = MultiMapping< index_t >;
    using FacetMultiMapping = MultiMapping< index_t >;
    using TetrahedronMapping = Mapping< index_t >;
} // namespace geode

namespace geode
{
    struct SolidCutPatchInfo
    {
        std::vector< VertexMultiMapping > vertices;
        std::vector< FacetMultiMapping > facets_along;
        std::vector< FacetMultiMapping > facets_across;
        std::vector< EdgeMultiMapping > edges;
        std::vector< TetrahedronMapping > tetrahedra;
    };

    /*!
     * Perform a TetrahedralSolid3D planar cut along a triangle given the patch
     * cut information.
     * @param[in] solid TetrahedralSolid to cut
     * @param[in] builder Builder of the Solid
     * @param[in] triangle The triangle corresponding to the patch to insert in
     * solid
     * @param[in] patch Patch cut information
     * @warn Cut tetrahedra are set as inactive but not deleted.
     */
    SolidCutPatchInfo geode_common_cutter_solid_api cut_along_patch(
        const TetrahedralSolid3D& solid,
        TetrahedralSolidBuilder3D& builder,
        const Triangle3D& triangle,
        TriangulatedSurface3D& patch );

    /*!
     * Perform a TetrahedralSolid3D planar cut along a triangle given its
     * boundary edges. These boundary edges are oriented solid edges.
     * @param[in] solid TetrahedralSolid to cut
     * @param[in] builder Builder of the Solid
     * @param[in] boundary_edges List of oriented Patch boundary as solid edges
     * @param[in] triangle The triangle corresponding to the patch to insert in
     * solid
     * @warn Cut tetrahedra are set as inactive but not deleted.
     */
    SolidCutPatchInfo geode_common_cutter_solid_api cut_along_patch(
        const TetrahedralSolid3D& solid,
        TetrahedralSolidBuilder3D& builder,
        absl::Span< const PatchFrontEdge > boundary_edges,
        const Triangle3D& triangle );
} // namespace geode