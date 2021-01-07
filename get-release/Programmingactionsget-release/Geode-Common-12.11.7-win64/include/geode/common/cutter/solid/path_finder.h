/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/geometry/basic_objects.h>

#include <geode/mesh/core/solid_mesh.h>

#include <geode/common/cutter/solid/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( TetrahedralSolid );
    ALIAS_3D( TetrahedralSolid );
    struct PolyhedronFacet;
} // namespace geode

namespace geode
{
    struct geode_common_cutter_solid_api SolidPath
    {
        SolidPath( const TetrahedralSolid3D& solid,
            PolyhedronFacet facet_in,
            Position position_in );

        SolidPath( index_t vertex_in );

        SolidPath() = default;

        bool is_vertex() const;

        bool is_facet_vertex() const;

        bool is_edge() const;

        bool is_facet() const;

        bool are_same_paths(
            const TetrahedralSolid3D& solid, const SolidPath& other ) const;

        geode::Position get_vertex_position(
            geode::local_index_t vertex ) const;

        geode::Position get_edge_position( geode::local_index_t edge ) const;

        geode::local_index_t get_edge_index() const;

        geode::local_index_t get_vertex_index() const;

        geode::local_index_t get_edge_vertex_id(
            geode::local_index_t vertex ) const;

        static constexpr std::array< geode::Position, 3 > vertex_position{
            { geode::Position::vertex0, geode::Position::vertex1,
                geode::Position::vertex2 }
        };

        static constexpr std::array< geode::Position, 3 > edge_position{
            { geode::Position::edge0, geode::Position::edge1,
                geode::Position::edge2 }
        };

        Position position{ Position::outside };
        PolyhedronFacet facet;
        index_t vertex_id{ NO_ID };
    };

    /*!
     * The objective of this class is to find paths inside TetrahedralSolid3D.
     * As an example, this class is able to find a rectilinear path between two
     * vertices. This class may be derived for custom path findings.
     */
    class geode_common_cutter_solid_api SolidPathFinder
    {
    public:
        /*!
         * Find a rectilinear path between two vertices.
         * @param[in] begin Index of the cut starting vertex
         * @param[in] end Index of the cut ending vertex.
         */
        SolidPathFinder(
            const TetrahedralSolid3D& solid, index_t begin, index_t end );

        std::vector< SolidPath > find_path() const;

    protected:
        bool edge_exist() const;

        bool stop_propagation( const SolidPath& path ) const;

        PolyhedronVertex adjacent_opposite_vertex(
            const PolyhedronFacet& facet ) const;

        PolyhedronFacet opposite_facet( const PolyhedronVertex& vertex ) const;

        PolyhedronVertex opposite_vertex( const PolyhedronFacet& facet ) const;

        Triangle3D facet_triangle( const PolyhedronFacet& facet ) const;

        Segment3D edge_segment( index_t edge_id ) const;

        absl::optional< SolidPath > find_path_intersection_with_facet(
            const PolyhedronFacet& facet ) const;

        SolidPath find_first_path() const;

        SolidPath find_next_path( index_t current ) const;

        PolyhedronFacet find_next_facet(
            const PolyhedronFacet& facet_from ) const;

        SolidPath next_intersection( const SolidPath& previous_path,
            const SolidPath& current_path ) const;

        const TetrahedralSolid3D& solid() const
        {
            return solid_;
        }

        index_t begin() const
        {
            return begin_;
        }

        index_t end() const
        {
            return end_;
        }

        const Segment3D& begin_to_end() const
        {
            return begin2end_;
        }

    private:
        const TetrahedralSolid3D& solid_;
        const index_t begin_;
        const index_t end_;
        const Segment3D begin2end_;
    };
} // namespace geode
