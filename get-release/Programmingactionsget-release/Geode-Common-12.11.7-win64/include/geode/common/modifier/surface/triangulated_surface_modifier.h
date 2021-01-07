/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/basic/pimpl.h>

#include <geode/mesh/core/surface_mesh.h>

#include <geode/common/modifier/common/information.h>
#include <geode/common/modifier/surface/common.h>

namespace geode
{
    struct PolygonEdge;
    FORWARD_DECLARATION_DIMENSION_CLASS( Point );
    FORWARD_DECLARATION_DIMENSION_CLASS( TriangulatedSurface );
    FORWARD_DECLARATION_DIMENSION_CLASS( TriangulatedSurfaceBuilder );
} // namespace geode

namespace geode
{
    using SplitTriangle = SplitInfo;
    using TriangleMapping = Mapping< index_t >;
    using EdgeMapping = Mapping< index_t >;
    using EdgeMultiMapping = MultiMapping< index_t >;
    using PolygonEdgeMapping = Mapping< PolygonEdge >;
    using PolygonEdgeMultiMapping = MultiMapping< PolygonEdge >;

    struct PolygonEdgeInfo
    {
        absl::InlinedVector< PolygonEdge, 2 > added;
        absl::InlinedVector< PolygonEdgeMapping, 2 > modified;
        absl::InlinedVector< PolygonEdgeMapping, 2 > remaining;
        absl::InlinedVector< TriangleMapping, 2 > triangles;
    };

    struct SplitPolygonEdgeInfo
    {
        index_t vertex;
        PolygonEdgeInfo left;
        PolygonEdgeInfo right;
    };

    struct SplitTriangleInfo
    {
        index_t vertex;
        absl::InlinedVector< PolygonEdge, 6 > added;
        absl::InlinedVector< PolygonEdgeMapping, 2 > modified;
        absl::InlinedVector< PolygonEdgeMapping, 3 > remaining;
        absl::InlinedVector< TriangleMapping, 3 > triangles;
    };

    template < index_t dimension >
    class TriangulatedSurfaceModifier
    {
    public:
        TriangulatedSurfaceModifier(
            const TriangulatedSurface< dimension >& surface,
            TriangulatedSurfaceBuilder< dimension >& builder );
        ~TriangulatedSurfaceModifier();

        bool is_triangle_active( index_t triangle_id ) const;

        bool is_edge_active( index_t edge_id ) const;

        bool is_edge_active( const PolygonEdge& edge ) const;

        /*!
         * Split a triangle into three new triangles.
         * @param[in] triangle_id Index of the triangle to split.
         * @param[in] point Coordinates of the split point.
         * @warn Given triangle is set as inactive but not deleted.
         */
        SplitTriangleInfo split_triangle(
            index_t triangle_id, const Point< dimension >& point );

        /*!
         * Split an edge into two new edges. The triangles incident to this edge
         * are also split into two new triangles.
         * @param[in] edge Index of the PolygonEdge to split.
         * @param[in] point Coordinates of the split point.
         * @warn Split triangles are set as inactive but not deleted.
         */
        SplitPolygonEdgeInfo split_edge(
            const PolygonEdge& edge, const Point< dimension >& point );

        /*!
         * Collapse an edge. The triangles incident to this edge
         * are set as inactive but not deleted.
         * @param[in] edge Index of the PolygonEdge to collapse.
         * @param[in] point Coordinates of the collapse point.
         * @return in the following order: the index of the collapse vertex, the
         * multi mapping (old <-> new) of the edited polygon edges.
         */
        std::tuple< index_t, absl::InlinedVector< PolygonEdgeMultiMapping, 4 > >
            collapse_edge(
                const PolygonEdge& edge, const Point< dimension >& point );

        /*!
         * Collapse an edge. The triangles incident to this edge
         * are set as inactive but not deleted.
         * @param[in] edge_id Unique index of the edge to collapse.
         * @param[in] point Coordinates of the collapse point.
         * @return in the following order: the index of the collapse vertex, the
         * mapping (old <-> new) of the edited edges.
         */
        std::tuple< index_t, std::vector< EdgeMultiMapping > > collapse_edge(
            index_t edge_id, const Point< dimension >& point );

        /*!
         * Clean up the mesh removing inactive triangles
         * @return old2new mappings for triangles
         */
        std::vector< index_t > clean_triangles();

        /*!
         * Clean up the mesh removing isolated vertices
         * @return old2new mappings for vertices
         * @warning This method should be called after clean_triangles()
         */
        std::vector< index_t > clean_vertices();

        /*!
         * Clean up the mesh removing edges
         * @return old2new mappings for edges
         * @warning This method should be called after clean_triangles()
         */
        std::vector< index_t > clean_edges();

        /*!
         * Clean up the mesh removing inactive triangles, isolated vertices and
         * edges.
         * @return old2new mappings for triangles, vertices and edges.
         */
        std::tuple< std::vector< index_t >,
            std::vector< index_t >,
            std::vector< index_t > >
            clean();

    protected:
        const TriangulatedSurface< dimension >& surface() const;

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };

    ALIAS_2D_AND_3D( TriangulatedSurfaceModifier );

} // namespace geode
