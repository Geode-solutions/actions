/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/geometry/basic_objects.h>

#include <geode/mesh/core/surface_mesh.h>

#include <geode/common/cutter/surface/surface_path_finder.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( TriangulatedSurface );
} // namespace geode

namespace geode
{
    /*!
     * This class is able to find a rectilinear path
     * between two vertices. This class may be derived for custom path findings.
     */
    template < index_t dimension >
    class SurfaceSegmentFinder : public SurfacePathFinder< dimension >
    {
    public:
        SurfaceSegmentFinder( const TriangulatedSurface< dimension >& surface,
            index_t begin,
            index_t end );

        ~SurfaceSegmentFinder();

    protected:
        bool edge_exist() const;

        bool stop_propagation( const SurfacePath& path ) const;

        index_t begin() const
        {
            return begin_;
        }

        index_t end() const
        {
            return end_;
        }

        const Segment< dimension >& segment() const
        {
            return segment_;
        }

    private:
        index_t begin_;
        index_t end_;
        Segment< dimension > segment_;
    };
} // namespace geode
