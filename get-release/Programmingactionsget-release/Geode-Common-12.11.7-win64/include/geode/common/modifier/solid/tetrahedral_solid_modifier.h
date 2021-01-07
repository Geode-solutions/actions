/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <array>
#include <memory>
#include <vector>

#include <geode/basic/pimpl.h>

#include <geode/common/modifier/common/information.h>
#include <geode/common/modifier/solid/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( Point );
    FORWARD_DECLARATION_DIMENSION_CLASS( TetrahedralSolid );
    FORWARD_DECLARATION_DIMENSION_CLASS( TetrahedralSolidBuilder );
    ALIAS_3D( Point );
    ALIAS_3D( TetrahedralSolid );
    ALIAS_3D( TetrahedralSolidBuilder );
    struct PolyhedronFacet;
    struct PolyhedronFacetEdge;
} // namespace geode

namespace geode
{
    using SplitTetrahedron = SplitInfo;
    using SplitFacet = SplitInfo;
    using TetrahedronMapping = Mapping< index_t >;
    using FacetMapping = Mapping< index_t >;
    using EdgeMapping = Mapping< index_t >;
    using VertexMultiMapping = MultiMapping< index_t >;

    struct SidedSolidSplitEdgeInfo
    {
        absl::InlinedVector< TetrahedronMapping, 2 > tetrahedra;
        absl::InlinedVector< FacetMapping, 2 > facets;
        absl::InlinedVector< EdgeMapping, 2 > edges;
    };

    struct AlongSolidSplitEdgeInfo
    {
        absl::InlinedVector< FacetMapping, 2 > facets;
        absl::InlinedVector< EdgeMapping, 2 > edges;
    };

    struct SolidSplitEdgeInfo
    {
        SolidSplitEdgeInfo() = default;
        SolidSplitEdgeInfo( index_t nb_tetrahedra );

        index_t vertex{ NO_ID };
        SidedSolidSplitEdgeInfo left;
        SidedSolidSplitEdgeInfo right;
        AlongSolidSplitEdgeInfo along;
    };

    struct SolidSplitInfo
    {
        SolidSplitInfo() = default;
        SolidSplitInfo( SolidSplitEdgeInfo&& split_edge_info );

        index_t vertex{ NO_ID };
        absl::InlinedVector< TetrahedronMapping, 4 > tetrahedra;
        absl::InlinedVector< FacetMapping, 4 > facets;
        absl::InlinedVector< EdgeMapping, 4 > edges;
    };

    class geode_common_modifier_solid_api TetrahedralSolidModifier
    {
    public:
        TetrahedralSolidModifier( const TetrahedralSolid3D& solid,
            TetrahedralSolidBuilder3D& builder );
        ~TetrahedralSolidModifier();

        bool is_tetrahedron_active( index_t tetrahedron_id ) const;

        bool is_facet_active( index_t facet_id ) const;

        /*!
         * Check if a tetrahedron split will keep the mesh valid, meaning no
         * tetrahedra volume flip its sign. The split is not applied.
         * @param[in] tetrahedron_id Index of the tetrahedron to split.
         * @param[in] point Coordinates of the split point.
         * @return true if split will keep mesh valid.
         */
        bool is_split_tetrahedron_valid(
            index_t tetrahedron_id, const Point3D& point ) const;

        /*!
         * Split a tetrahedron into four new tetrahedra.
         * @param[in] tetrahedron_id Index of the tetrahedron to split.
         * @param[in] point Coordinates of the split point.
         * @warn Given tetrahedron is set as inactive but not deleted.
         * @return new vertex index and mappings between old and new mesh.
         */
        SolidSplitInfo split_tetrahedron(
            index_t tetrahedron_id, const Point3D& point );

        /*!
         * Check if a facet split will keep the mesh valid, meaning no
         * tetrahedra volume flip its sign. The split is not applied.
         * @param[in] facet Index of the PolyhedronFacet to split.
         * @param[in] point Coordinates of the split point.
         * @return true if split will keep mesh valid.
         */
        bool is_split_facet_valid(
            const PolyhedronFacet& facet, const Point3D& point ) const;

        /*!
         * Check if a facet split will keep the mesh valid, meaning no
         * tetrahedra volume flip its sign. The split is not applied.
         * @param[in] facet_id Unique index of the facet to split.
         * @param[in] point Coordinates of the split point.
         * @return true if split will keep mesh valid.
         */
        bool is_split_facet_valid(
            index_t facet_id, const Point3D& point ) const;

        /*!
         * Split a facet into three new ones. The tetrahedra incident to this
         * facet are also split into three new tetrahedra.
         * @param[in] facet_id Unique index of the facet to split.
         * @param[in] point Coordinates of the split point.
         * @warn Split tetrahedra are set as inactive but not deleted.
         * @return new vertex index and mappings between old and new mesh.
         */
        SolidSplitInfo split_facet( index_t facet_id, const Point3D& point );

        /*!
         * Split a facet into three new ones. The tetrahedra incident to this
         * facet are also split into three new tetrahedra.
         * @param[in] facet Index of the PolyhedronFacet to split.
         * @param[in] point Coordinates of the split point.
         * @warn Split tetrahedra are set as inactive but not deleted.
         * @details Edge unique index is found and then the above function is
         * called.
         * @return new vertex index and mappings between old and new mesh.
         */
        SolidSplitInfo split_facet(
            const PolyhedronFacet& facet, const Point3D& point );

        /*!
         * Check if an edge split will keep the mesh valid, meaning no
         * tetrahedra volume flip its sign. The split is not applied.
         * @param[in] edge Index of the PolyhedronFacetEdge to split.
         * @param[in] point Coordinates of the split point.
         * @return true if split will keep mesh valid.
         */
        bool is_split_edge_valid(
            const PolyhedronFacetEdge& edge, const Point3D& point ) const;

        /*!
         * Check if an edge split will keep the mesh valid, meaning no
         * tetrahedra volume flip its sign. The split is not applied.
         * @param[in] edge_id Unique index of the edge to split.
         * @param[in] point Coordinates of the split point.
         * @return true if split will keep mesh valid.
         */
        bool is_split_edge_valid( index_t edge_id, const Point3D& point ) const;

        /*!
         * Split an edge into two new ones. Each tetrahedron incident to this
         * edge is also split into two new tetrahedra.
         * @param[in] edge_id Unique index of the edge to split.
         * @param[in] point Coordinates of the split point.
         * @warn Split tetrahedra are set as inactive but not deleted.
         * @return new vertex index and mappings between old and new mesh.
         */
        SolidSplitEdgeInfo split_edge( index_t edge_id, const Point3D& point );

        /*!
         * Split an edge into two new ones. Each tetrahedron incident to this
         * edge is also split into two new tetrahedra.
         * @param[in] edge Index of the PolyhedronFacetEdge to split.
         * @param[in] point Coordinates of the split point.
         * @warn Split tetrahedra are set as inactive but not deleted.
         * @return new vertex index and mappings between old and new mesh.
         */
        SolidSplitEdgeInfo split_edge(
            const PolyhedronFacetEdge& edge, const Point3D& point );

        /*!
         * Check if an edge collapse will keep the mesh valid, meaning no
         * tetrahedra volume flip its sign. The collapse is not applied.
         * @param[in] edge Index of the PolyhedronFacetEdge to collapse.
         * @param[in] point Coordinates of the collapse point.
         * @return true if collapse will keep mesh valid.
         */
        bool is_collapse_edge_valid(
            const PolyhedronFacetEdge& edge, const Point3D& point ) const;

        /*!
         * Check if an edge collapse will keep the mesh valid, meaning no
         * tetrahedra volume flip its sign. The collapse is not applied.
         * @param[in] edge_id Unique index of the edge to collapse.
         * @param[in] point Coordinates of the collapse point.
         * @return true if collapse will keep mesh valid.
         */
        bool is_collapse_edge_valid( index_t _id, const Point3D& point ) const;

        /*!
         * Collapse an edge. The tetrahedra incident to this edge
         * are set as inactive but not deleted.
         * @param[in] edge Index of the PolyhedronFacetEdge to collapse.
         * @param[in] point Coordinates of the collapse point.
         * @return in the following order: the index of the collapse vertex, the
         * mapping (old <-> new) of the edited facets and the mapping (old <->
         * new) of the edited edges.
         */
        std::tuple< VertexMultiMapping,
            absl::FixedArray< FacetMapping >,
            absl::FixedArray< EdgeMapping > >
            collapse_edge(
                const PolyhedronFacetEdge& edge, const Point3D& point );

        /*!
         * Collapse an edge. The tetrahedra incident to this edge
         * are set as inactive but not deleted.
         * @param[in] edge_id Unique index of the edge to collapse.
         * @param[in] point Coordinates of the collapse point.
         * @return in the following order: the index of the collapse vertex, the
         * mapping (old <-> new) of the edited facets and the mapping (old <->
         * new) of the edited edges.
         */
        std::tuple< VertexMultiMapping,
            absl::FixedArray< FacetMapping >,
            absl::FixedArray< EdgeMapping > >
            collapse_edge( index_t edge_id, const Point3D& point );

        /*!
         * Clean up the mesh removing inactive tetrahedra, isolated vertices,
         * edges and facets.
         */
        void clean();

    protected:
        const TetrahedralSolid3D& solid() const;

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };

} // namespace geode
