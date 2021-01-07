/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/common/geometry/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( InfiniteLine );
    FORWARD_DECLARATION_DIMENSION_CLASS( Point );
    FORWARD_DECLARATION_DIMENSION_CLASS( Segment );
    FORWARD_DECLARATION_DIMENSION_CLASS( Triangle );
    ALIAS_2D_AND_3D( InfiniteLine );
    ALIAS_2D_AND_3D( Point );
    ALIAS_2D_AND_3D( Segment );
    ALIAS_3D( Triangle );
    class Plane;
    enum struct Position;
} // namespace geode

namespace geode
{
    using SegmentSegmentIntersection = std::pair< Position, Position >;
    using SegmentTriangleIntersection = std::pair< Position, Position >;

    /*!
     * Detect if there is an intersection between two 2D segments
     * @return the position of the intersection on the two segments.
     * Returns outside-outside if there is no intersection or parallel-pallel
     * if all points are colinear
     */
    SegmentSegmentIntersection geode_common_geometry_api
        segment_segment_intersection_detection(
            const Segment2D& segment0, const Segment2D& segment1 );

    /*!
     * Detect the configuration between two 2D colinear segments
     * @return the position of the intersection on the two segments.
     * Returns parallel-pallel if there is segment overlap
     */
    SegmentSegmentIntersection geode_common_geometry_api
        colinear_segment_segment_intersection_detection(
            const Segment2D& segment0, const Segment2D& segment1 );

    /*!
     * Detect if there is an intersection between a line and a segment
     * @return the position of the intersection on the segment.
     * Returns outside if there is no intersection or all points are
     * colinear
     */
    Position geode_common_geometry_api segment_line_intersection_detection(
        const Segment2D& segment, const InfiniteLine2D& line );

    /*!
     * Detect if there is an intersection between a line and a triangle
     * @return the position of the intersection on the triangle.
     * Returns outside if there is no intersection or the line is
     * coplanar with the triangle
     */
    Position geode_common_geometry_api line_triangle_intersection_detection(
        const InfiniteLine3D& line, const Triangle3D& triangle );

    /*!
     * Detect if there is an intersection between a segment and a triangle
     * @return the position of the intersection on the segment and on the
     * triangle. Returns outside if there is no intersection or the segment is
     * coplanar with the triangle
     */
    SegmentTriangleIntersection geode_common_geometry_api
        segment_triangle_intersection_detection(
            const Segment3D& segment, const Triangle3D& triangle );

    /*!
     * Detect if there is an intersection between a segment and a plane
     * @return the position of the intersection on the segment.
     * Returns outside if there is no intersection or the segment is
     * coplanar with the plane
     */
    Position geode_common_geometry_api segment_plane_intersection_detection(
        const Segment3D& segment, const Plane& plane );
} // namespace geode
