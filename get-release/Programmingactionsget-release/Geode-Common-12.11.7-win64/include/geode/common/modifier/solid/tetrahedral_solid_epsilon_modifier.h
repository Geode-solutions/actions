/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/common/modifier/solid/common.h>
#include <geode/common/modifier/solid/tetrahedral_solid_modifier.h>

namespace geode
{
    struct PolyhedronFacet;
    FORWARD_DECLARATION_DIMENSION_CLASS( Point );
    FORWARD_DECLARATION_DIMENSION_CLASS( TetrahedralSolid );
    FORWARD_DECLARATION_DIMENSION_CLASS( TetrahedralSolidBuilder );
    ALIAS_3D( Point );
    ALIAS_3D( TetrahedralSolid );
    ALIAS_3D( TetrahedralSolidBuilder );
} // namespace geode

namespace geode
{
    class geode_common_modifier_solid_api TetrahedralSolidEpsilonModifier
        : public TetrahedralSolidModifier
    {
    public:
        TetrahedralSolidEpsilonModifier( const TetrahedralSolid3D& solid,
            TetrahedralSolidBuilder3D& builder );
        ~TetrahedralSolidEpsilonModifier();

        /*!
         * Insert a point in a tetrahedron either by spliting the tetrahedron
         * into four new ones, or by snapping the point on a facet (three new
         * tetrahedra for each side), an edge (each incident tetrahedra is split
         * into two new tetrahedra) or a vertex (no new tetrahedron).
         * @param[in] tetrahedron_id Index of the tetrahedron to split.
         * @param[in] point Coordinates of the split point.
         * @warn Split tetrahedron is set as inactive but not deleted.
         * @return new vertex index and mappings between old and new mesh.
         */
        SolidSplitInfo insert_point_in_tetrahedron(
            index_t tetrahedron_id, const Point3D& point );

        /*!
         * Insert a point on a facet either by spliting the facet into
         * three new ones, or by snapping the point on a edge or a vertex.
         * @param[in] facet Index of the PolyhedronFacet to split.
         * @param[in] point Coordinates of the split point.
         * @warn Potential split tetrahedra are set as inactive but not deleted.
         * @return new vertex index and mappings between old and new mesh.
         */
        SolidSplitInfo insert_point_on_facet(
            const PolyhedronFacet& facet, const Point3D& point );

        /*!
         * Insert a point on an edge either by spliting the edge into
         * two new ones, or by snapping the point on a vertex.
         * @param[in] edge Index of the PolyhedronFacetEdge to split.
         * @param[in] point Coordinates of the split point.
         * @warn Potential split tetrahedra are set as inactive but not deleted.
         * @return new vertex index and mappings between old and new mesh.
         */
        SolidSplitEdgeInfo insert_point_on_edge(
            const PolyhedronFacetEdge& edge, const Point3D& point );

        /*!
         * Insert a point on an edge either by spliting the edge into
         * two new ones, or by snapping the point on a vertex.
         * @param[in] edge_id Unique index of the edge to split.
         * @param[in] point Coordinates of the split point.
         * @warn Potential split tetrahedra are set as inactive but not deleted.
         * @return new vertex index and mappings between old and new mesh.
         */
        SolidSplitEdgeInfo insert_point_on_edge(
            index_t edge_id, const Point3D& point );

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };
} // namespace geode
