/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/basic/pimpl.h>

#include <geode/common/modifier/edged_curve/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( Point );
    FORWARD_DECLARATION_DIMENSION_CLASS( EdgedCurve );
    FORWARD_DECLARATION_DIMENSION_CLASS( EdgedCurveBuilder );
} // namespace geode

namespace geode
{
    template < index_t dimension >
    class EdgedCurveModifier
    {
    public:
        EdgedCurveModifier( const EdgedCurve< dimension >& curve,
            EdgedCurveBuilder< dimension >& builder );
        ~EdgedCurveModifier();

        bool is_edge_active( index_t edge_id ) const;

        /*!
         * Collapse an edge.
         * @param[in] edge_id Index of the edge to collapse.
         * @param[in] point Coordinates of the collapse point.
         * @return the index of the collapse vertex.
         */
        index_t collapse_edge(
            index_t edge_id, const Point< dimension >& point );

        /*!
         * Clean up the mesh removing inactive edges
         * @return old2new mappings for edges
         */
        std::vector< index_t > clean_edges();

        /*!
         * Clean up the mesh removing inactive edges, isolated vertices.
         * @return old2new mappings for edges and vertices.
         */
        std::tuple< std::vector< index_t >, std::vector< index_t > > clean();

    protected:
        const EdgedCurve< dimension >& curve() const;

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };
    ALIAS_2D_AND_3D( EdgedCurveModifier );
} // namespace geode
