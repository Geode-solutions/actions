/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/geometry/basic_objects.h>

#include <geode/mesh/core/polygonal_surface.h>

#include <geode/common/cutter/surface/common.h>
#include <geode/common/cutter/surface/surface_segment_finder.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( TriangulatedSurface );
    ALIAS_2D( TriangulatedSurface );
} // namespace geode

namespace geode
{
    /*!
     * The objective of this class is to find paths inside TrianguledSurface2D.
     * As an example, this class is able to find a rectilinear path between two
     * vertices. This class may be derived for custom path findings.
     */
    class geode_common_cutter_surface_api SurfaceSegmentFinder2D
        : public SurfaceSegmentFinder< 2 >
    {
    public:
        SurfaceSegmentFinder2D(
            const TriangulatedSurface2D& surface, index_t begin, index_t end );

        /*!
         * Find a rectilinear path between two vertices.
         * @param[in] begin Index of the cut starting vertex
         * @param[in] end Index of the cut ending vertex
         * @return an ordered list of path locations between begin and end.
         */
        std::vector< SurfacePath > find_path() const;

    protected:
        absl::optional< SurfacePath > find_first_path() const;
    };
} // namespace geode
