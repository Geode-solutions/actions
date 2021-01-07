/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <absl/container/flat_hash_set.h>

#include <geode/basic/logger.h>
#include <geode/basic/pimpl.h>

#include <geode/geometry/basic_objects.h>

#include <geode/mesh/core/solid_mesh.h>

#include <geode/common/cutter/solid/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( TetrahedralSolid );
    ALIAS_3D( TetrahedralSolid );
    FORWARD_DECLARATION_DIMENSION_CLASS( TriangulatedSurface );
    ALIAS_3D( TriangulatedSurface );
    struct PolyhedronFacetEdge;
} // namespace geode

namespace geode
{
    struct geode_common_cutter_solid_api SolidPatch
    {
        SolidPatch( const TetrahedralSolid3D& solid,
            index_t edge_in,
            Position position_in );

        SolidPatch( index_t vertex_in );

        SolidPatch() = default;

        bool is_vertex() const;

        bool is_edge_vertex() const;

        bool is_edge() const;

        bool are_same_paths( const SolidPatch& other ) const;

        geode::Position get_vertex_position( geode::index_t vertex ) const;

        geode::index_t get_vertex_index() const;

        void add_patch_front_edge( index_t patch_front_edge_id )
        {
            patch_front_edge.insert( patch_front_edge_id );
        }

        bool is_in_patch_front_edge( index_t patch_front_edge_id ) const
        {
            return patch_front_edge.find( patch_front_edge_id )
                   != patch_front_edge.end();
        }

        static constexpr std::array< geode::Position, 2 > vertex_position{
            { geode::Position::vertex0, geode::Position::vertex1 }
        };

        Position position{ Position::outside };
        index_t edge_id;
        index_t vertex_id{ NO_ID };
        absl::flat_hash_set< index_t > patch_front_edge;
    };

    struct PatchFrontEdge
    {
        PatchFrontEdge(
            index_t id_in, bool orientation_in, index_t patch_edge_in )
            : id( id_in ),
              orientation( orientation_in ),
              patch_edge( patch_edge_in )
        {
        }

        index_t id{ NO_ID };
        bool orientation{ false };
        index_t patch_edge{ NO_ID };
    };

    using OrientedEdge = std::pair< index_t, bool >;
    /*!
     * The objective of this class is to find patchs inside TetrahedralSolid3D.
     * As an example, this class is able to find a triangular patch given
     * boundary edges. This class may be derived for custom patch findings.
     */
    class geode_common_cutter_solid_api SolidPatchFinder
    {
    public:
        static constexpr auto solid_patch_attribute_name = "patch";
        using solid_patch_attribute_type = SolidPatch;
        static constexpr auto edge_hint_attribute_name = "hint";
        using edge_hint_attribute_type = local_index_t;

        /*!
         * Find a triangular patch.
         * @param[in] begin Index of the cut starting vertex
         * @param[in] end Index of the cut ending vertex.
         */
        SolidPatchFinder( const TetrahedralSolid3D& solid,
            absl::Span< const PatchFrontEdge > boundary_edges,
            const Triangle3D& triangle );

        ~SolidPatchFinder();

        TriangulatedSurface3D& find_patch();

        const TetrahedralSolid3D& solid() const;

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };
} // namespace geode
