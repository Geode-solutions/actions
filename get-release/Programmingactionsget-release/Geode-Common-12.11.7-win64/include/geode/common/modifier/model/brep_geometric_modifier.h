/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/basic/pimpl.h>

#include <geode/common/modifier/model/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( Point );
    FORWARD_DECLARATION_DIMENSION_CLASS( Surface );

    ALIAS_3D( Point );
    ALIAS_3D( Surface );

    class BRep;

    struct PolygonEdge;
} // namespace geode

namespace geode
{
    class geode_common_modifier_model_api BRepGeometricModifier
    {
    public:
        BRepGeometricModifier( BRep& brep );
        ~BRepGeometricModifier();

        void collapse_edge( const Surface3D& surface,
            const PolygonEdge& edge,
            const Point3D& point );

        void clean_triangles_surfaces();

        void clean_edges_lines();

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };
} // namespace geode