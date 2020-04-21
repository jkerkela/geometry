// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2014-2017, Oracle and/or its affiliates.

// Contributed and/or modified by Menelaos Karavelas, on behalf of Oracle
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html

#ifndef BOOST_GEOMETRY_ALGORITHMS_DETAIL_DISJOINT_MULTIRANGE_GEOMETRY_HPP
#define BOOST_GEOMETRY_ALGORITHMS_DETAIL_DISJOINT_MULTIRANGE_GEOMETRY_HPP

#include <boost/range.hpp>

#include <boost/geometry/algorithms/detail/check_iterator_range.hpp>
#include <boost/geometry/algorithms/dispatch/disjoint.hpp>


namespace boost { namespace geometry
{


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace disjoint
{

template <typename Geometry, typename Strategy, typename Construction>
class disjoint_geometry_to_query_geometry
{
public:
    disjoint_geometry_to_query_geometry(Geometry const& geometry,
                                        Strategy const& strategy)
        : m_geometry(geometry)
        , m_strategy(strategy)
    {}

    typedef typename point_type<Geometry>::type point_type;

    typedef segment_intersection_points<point_type> intersection_return_type;

    template <typename QueryGeometry>
    inline intersection_return_type
    apply(QueryGeometry const& query_geometry) const
    {
        return Construction::apply(query_geometry, m_geometry, m_strategy);
    }

private:
    Geometry const& m_geometry;
    Strategy const& m_strategy;
};


template <typename Geometry, typename Strategy, typename BinaryPredicate>
class unary_disjoint_geometry_to_query_geometry
{
public:
    unary_disjoint_geometry_to_query_geometry(Geometry const& geometry,
                                              Strategy const& strategy)
        : m_geometry(geometry)
        , m_strategy(strategy)
    {}

    template <typename QueryGeometry>
    inline bool apply(QueryGeometry const& query_geometry) const
    {
        return BinaryPredicate::apply(query_geometry, m_geometry, m_strategy);
    }

private:
    Geometry const& m_geometry;
    Strategy const& m_strategy;
};

template<typename MultiRange, typename ConstantSizeGeometry>
struct multirange_constant_size_geometry_with_info
{

    typedef typename point_type<ConstantSizeGeometry>::type point_type;

    typedef segment_intersection_points<point_type> intersection_return_type;

    template <typename Strategy>
    static inline intersection_return_type
    apply(MultiRange const& multirange,
          ConstantSizeGeometry const& constant_size_geometry,
          Strategy const& strategy)
    {
        typedef disjoint_geometry_to_query_geometry
            <
                ConstantSizeGeometry,
                Strategy,
                dispatch::disjoint_with_info
                    <
                        typename boost::range_value<MultiRange>::type,
                        ConstantSizeGeometry
                    >
            > unary_predicate_type;

        intersection_return_type res = detail::check_iterator_range_with_info
            <
                ConstantSizeGeometry, unary_predicate_type
            >::apply(boost::begin(multirange), boost::end(multirange),
                     unary_predicate_type(constant_size_geometry, strategy));
        return res;
    }

    template <typename Strategy>
    static inline intersection_return_type
    apply(ConstantSizeGeometry const& constant_size_geometry,
          MultiRange const& multirange,
          Strategy const& strategy)
    {
        return apply(multirange, constant_size_geometry, strategy);
    }
};


template<typename MultiRange, typename ConstantSizeGeometry>
struct multirange_constant_size_geometry
{
    typedef typename point_type<ConstantSizeGeometry>::type point_type;
    typedef segment_intersection_points<point_type> intersection_return_type;

    template <typename Strategy>
    static inline bool
    apply(MultiRange const& multirange,
          ConstantSizeGeometry const& constant_size_geometry,
          Strategy const& strategy)
    {
        intersection_return_type res =
                multirange_constant_size_geometry_with_info
                <
                    MultiRange,
                    ConstantSizeGeometry
                >::apply(multirange, constant_size_geometry, strategy);
        return res.count != 0;
    }

    template <typename Strategy>
    static inline bool apply(ConstantSizeGeometry const& constant_size_geometry,
                             MultiRange const& multirange,
                             Strategy const& strategy)
    {
        return apply(multirange, constant_size_geometry, strategy);
    }
};

}} // namespace detail::disjoint
#endif // DOXYGEN_NO_DETAIL


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_ALGORITHMS_DETAIL_DISJOINT_MULTIRANGE_GEOMETRY_HPP
