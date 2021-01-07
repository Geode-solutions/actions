/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/geometry/basic_objects.h>

#include <geode/mesh/core/surface_mesh.h>

#include <geode/common/cutter/surface/common.h>
#include <geode/common/geometry/position.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( TriangulatedSurface );
} // namespace geode

namespace geode
{
    struct SurfacePath
    {
        SurfacePath( PolygonEdge edge_in, Position position_in )
            : edge( std::move( edge_in ) ), position( position_in )
        {
        }

        SurfacePath( index_t vertex_in ) : vertex_id( vertex_in ) {}

        SurfacePath() = default;

        bool is_vertex() const
        {
            return vertex_id != NO_ID;
        }

        PolygonEdge edge;
        Position position{ Position::outside };
        index_t vertex_id{ NO_ID };
        bool opposite_side{ true };
    };

    /*!
     * The objective of this class is to find paths inside TrianguledSurface<
     * dimension >. This class may be derived for custom path findings.
     */
    template < index_t dimension >
    class SurfacePathFinder
    {
        OPENGEODE_DISABLE_COPY_AND_MOVE( SurfacePathFinder );

    public:
        ~SurfacePathFinder();

    protected:
        SurfacePathFinder< dimension >(
            const TriangulatedSurface< dimension >& surface );

        PolygonVertex adjacent_opposite_vertex( const PolygonEdge& edge ) const;

        Segment< dimension > edge_segment( const PolygonEdge& edge ) const;

        const TriangulatedSurface< dimension >& surface() const;

        template < index_t T = dimension >
        typename std::enable_if< T == 2, SurfacePath >::type next_intersection(
            const PolygonEdge& adjacent_edge, const InfiniteLine2D& line ) const
        {
            const auto opposite_vertex =
                surface().previous_polygon_vertex( adjacent_edge );
            const auto& opposite_point =
                surface().point( surface().polygon_vertex( opposite_vertex ) );
            const auto side = point_side_to_line( opposite_point, line );
            if( side == Side::zero )
            {
                return { opposite_vertex, Position::vertex0 };
            }
            const auto edge_to_split =
                side == Side::positive
                    ? surface().next_polygon_edge( adjacent_edge )
                    : surface().previous_polygon_edge( adjacent_edge );
            return { edge_to_split, Position::inside };
        }

    private:
        const TriangulatedSurface< dimension >& surface_;
    };
} // namespace geode
