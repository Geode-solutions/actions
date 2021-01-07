/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <absl/types/optional.h>

#include <geode/basic/pimpl.h>

#include <geode/geometry/basic_objects.h>

#include <geode/common/helpers/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( SurfaceMesh );
    ALIAS_3D( SurfaceMesh );
} // namespace geode

namespace geode
{
    class geode_common_helpers_api RayTracing3D
    {
    public:
        struct PolygonDistance
        {
            PolygonDistance( index_t polygon_in, double distance_in )
                : polygon( polygon_in ), distance( distance_in )
            {
            }

            bool operator<( const PolygonDistance& other ) const
            {
                return distance < other.distance;
            }

            index_t polygon;
            double distance;
        };

    public:
        RayTracing3D( const SurfaceMesh3D& mesh, const Ray3D& ray );
        RayTracing3D( RayTracing3D&& other );
        ~RayTracing3D();

        absl::optional< PolygonDistance > closest_polygon() const;

        void operator()( index_t polygon_id );

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };

} // namespace geode
