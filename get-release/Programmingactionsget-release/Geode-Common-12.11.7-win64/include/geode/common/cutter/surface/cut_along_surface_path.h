/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/common/modifier/surface/triangulated_surface_epsilon_modifier.h>

#include <geode/common/cutter/surface/common.h>
#include <geode/common/cutter/surface/surface_path_finder.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( TriangulatedSurface );
    ALIAS_2D_AND_3D( TriangulatedSurface );
    ALIAS_2D_AND_3D( TriangulatedSurfaceBuilder );
} // namespace geode

namespace geode
{
    template < index_t dimension >
    struct GeometricSurfacePath
    {
        GeometricSurfacePath( SurfacePath path_in, Point< dimension > point )
            : path( std::move( path_in ) ), location( std::move( point ) )
        {
        }

        SurfacePath path;
        Point< dimension > location;
    };
    ALIAS_2D_AND_3D( GeometricSurfacePath );

    struct SideSurfaceCutPathInfo
    {
        SideSurfaceCutPathInfo( std::size_t size )
        {
            if( size > 2 )
            {
                along_path.reserve( 2 * ( size - 2 ) );
                across_path.reserve( 2 * ( size - 2 ) );
                remaining.reserve( 2 * ( size - 2 ) );
                triangles.reserve( 4 * ( size - 2 ) );
            }
        }
        std::vector< PolygonEdgeMapping > along_path;
        std::vector< PolygonEdgeMapping > across_path;
        std::vector< PolygonEdgeMapping > remaining;
        std::vector< TriangleMapping > triangles;
    };

    struct SurfaceCutPathInfo
    {
        SurfaceCutPathInfo( std::size_t size ) : left( size ), right( size )
        {
            vertices.reserve( size );
        }
        std::vector< index_t > vertices;
        SideSurfaceCutPathInfo left;
        SideSurfaceCutPathInfo right;
    };

    std::vector< GeometricSurfacePath2D > geode_common_cutter_surface_api
        determine_cuts( const TriangulatedSurface2D& surface,
            index_t begin,
            index_t end,
            absl::Span< const geode::SurfacePath > path_splits );

    std::vector< GeometricSurfacePath3D > geode_common_cutter_surface_api
        determine_cuts( const TriangulatedSurface3D& surface,
            index_t begin,
            index_t end,
            const Plane& plane,
            absl::Span< const geode::SurfacePath > path_splits );

    template < index_t dimension >
    SurfaceCutPathInfo cut_along_path(
        const TriangulatedSurface< dimension >& surface,
        TriangulatedSurfaceBuilder< dimension >& builder,
        index_t begin,
        index_t end,
        absl::Span< const GeometricSurfacePath< dimension > > path_splits );

    SurfaceCutPathInfo geode_common_cutter_surface_api cut_along_path(
        const TriangulatedSurface2D& surface,
        TriangulatedSurfaceBuilder2D& builder,
        index_t begin,
        index_t end );
} // namespace geode
