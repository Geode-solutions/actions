/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/common/modifier/surface/common.h>
#include <geode/common/modifier/surface/triangulated_surface_modifier.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( Point );
    FORWARD_DECLARATION_DIMENSION_CLASS( TriangulatedSurface );
    FORWARD_DECLARATION_DIMENSION_CLASS( TriangulatedSurfaceBuilder );
    struct PolygonEdge;
    enum struct Position;
} // namespace geode

namespace geode
{
    template < index_t dimension >
    class TriangulatedSurfaceEpsilonModifier
        : public TriangulatedSurfaceModifier< dimension >
    {
    public:
        TriangulatedSurfaceEpsilonModifier(
            const TriangulatedSurface< dimension >& surface,
            TriangulatedSurfaceBuilder< dimension >& builder );
        ~TriangulatedSurfaceEpsilonModifier();

        /*!
         * Insert a point in a triangle either by spliting the triangle into
         * three new ones, or by snapping the point on an edge (two new
         * triangles) or a vertex (no new triangles).
         * @param[in] triangle_id Index of the triangle to split.
         * @param[in] point Coordinates of the split point.
         * @warn Split triangle is set as inactive but not deleted.
         * @return in the following order: the position of the split vertex, the
         * information regarding the modifications.
         */
        std::tuple< Position, SplitTriangleInfo > insert_point_in_triangle(
            index_t triangle_id, const Point< dimension >& point );

        /*!
         * Insert a point on an edge either by spliting the edge into
         * two new ones, or by snapping the point on a vertex (no new edges).
         * @param[in] edge Index of the PolygonEdge to split.
         * @param[in] point Coordinates of the split point.
         * @warn Potential split triangles are set as inactive but not deleted.
         * @return in the following order: the position of the split vertex, the
         * information regarding the modifications.
         */
        std::tuple< Position, SplitPolygonEdgeInfo > insert_point_on_edge(
            const PolygonEdge& edge, const Point< dimension >& point );

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };
    ALIAS_2D_AND_3D( TriangulatedSurfaceEpsilonModifier );

} // namespace geode
