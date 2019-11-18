// Boost.Geometry
// Unit Test

// Copyright (c) 2019, Oracle and/or its affiliates.

// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html

#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE test_closest_points_geographic_linear_areal
#endif

#define BOOST_GEOMETRY_TEST_DEBUG

#include "test_closest_points_common.hpp"

namespace bg = boost::geometry;

//===========================================================================

template <typename Point, typename Strategy>
void test_closest_points_segment_polygon_or_ring(Strategy const& strategy)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "segment/polygon or ring closest_points tests" << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::ring<Point> Ring;
    typedef bg::model::polygon<Point> Polygon;
    typedef test_geometry<Segment, Ring, Segment> tester;

    tester::apply("SEGMENT(2 0,0 2)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategy);

    typedef test_geometry<Segment, Polygon, Segment> tester2;

    tester2::apply("SEGMENT(2 0,0 2)",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategy);

    tester2::apply("SEGMENT(2 0,0 2)",
                   "POLYGON((0 0,1 0,0 1,0 0)(0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4))",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategy);
}

//===========================================================================

template <typename Point, typename Strategy>
void test_closest_points_segment_multi_polygon(Strategy const& strategy)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "segment/multi-polygon or ring closest_points tests" << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::polygon<Point> Polygon;
    typedef bg::model::multi_polygon<Polygon> MultiPolygon;
    typedef test_geometry<Segment, MultiPolygon, Segment> tester;

    tester::apply("SEGMENT(2 0,0 2)",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategy);
}

//===========================================================================

template <typename Point, typename Strategy>
void test_closest_points_segment_box(Strategy const& strategy)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "segment/box closest_points tests" << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::box<Point> Box;
    typedef test_geometry<Segment, Box, Segment> tester;

    tester::apply("SEGMENT(2 0,0 2)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(0.983761 1.0167,10 10)",
                  strategy);

    tester::apply("SEGMENT(5 0,5 15)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(5 15,10 15.055)",
                  strategy);

    tester::apply("SEGMENT(0 5,15 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(9.98098 5.03848,10 10)",
                  strategy);

    tester::apply("SEGMENT(0 5,20 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(10 5.07725,10 10)",
                  strategy);

    tester::apply("SEGMENT(10 5, 20 6)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20 6,20 10)",
                  strategy);

    tester::apply("SEGMENT(0 0, 0 10)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(0 10,10 10.1521)",
                  strategy);

    tester::apply("SEGMENT(0 5, 125 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20.2533 8.02139,20 10)",
                  strategy);

    // Test segments above box
    tester::apply("SEGMENT(0 25, 9 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(9 25,10 20)",
                  strategy);
    tester::apply("SEGMENT(0 25, 10 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(10 25,10 20)",
                  strategy);
    tester::apply("SEGMENT(0 25, 11 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(11 25,11 20)",
                  strategy);
    tester::apply("SEGMENT(0 25, 22 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20.3787 25.1124,20 20)",
                  strategy);

    // Segments left-right of box
    tester::apply("SEGMENT(0 5,9 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(9 5,10 10)",
                  strategy);
    tester::apply("SEGMENT(21 10, 21 15)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(21 15,20 15.0022)",
                  strategy);


    //Segments on corners of box
    //left-top corner
    //generic
    tester::apply("SEGMENT(9 19.5, 11 21)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(9.8677 20.1571,10 20)",
                  strategy);
    //degenerate
    tester::apply("SEGMENT(9 19, 11 21)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(9.99514 20.0043,10 20)",
                  strategy);
    //left-bottom corner
    //generic
    tester::apply("SEGMENT(8.5 11, 11 9)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(9.90395 9.88218,10 10)",
                  strategy);
    //degenerate
    //zero distance, closest points not defined
    tester::apply("SEGMENT(9 11, 11 9)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(0 0,0 0)",
                  strategy);
    //right-top corner
    //generic
    tester::apply("SEGMENT(19 21, 21 19.5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20.1323 20.1571,20 20)",
                  strategy);
    //degenerate
    tester::apply("SEGMENT(19 21, 21 19)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20.0049 20.0043,20 20)",
                  strategy);
    //right-bottom corner
    //generic
    tester::apply("SEGMENT(19 9, 21 10.5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20.1197 9.84293,20 10)",
                  strategy);
    //degenerate
    tester::apply("SEGMENT(19 9, 21 11)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(0 0,0 0)",
                  strategy);

    //Segment and box on different hemispheres
    tester::apply("SEGMENT(10 20, 15 30)",
                  "BOX(10 -20,20 -10)",
                  "SEGMENT(10 20,10 -10)",
                  strategy);
    tester::apply("SEGMENT(0 10, 12 10)",
                  "BOX(10 -20,20 -10)",
                  "SEGMENT(12 10,12 -10)",
                  strategy);
    tester::apply("SEGMENT(10 10, 20 10)",
                  "BOX(10 -20,20 -10)",
                  "SEGMENT(10 10,10 -10)",
                  strategy);
    tester::apply("SEGMENT(0 -10, 12 -10)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(12 -10,12 10)",
                  strategy);
    tester::apply("SEGMENT(10 -10, 20 -10)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(10 -10,10 10)",
                  strategy);
/*

    //Segment and box on different hemispheres
    std::string const box_south = "BOX(10 -20,20 -10)";

    tester::apply("test_ns1", "SEGMENT(10 20, 15 30)", box_south,
                  ps_distance<Point>("POINT(10 -10)", "SEGMENT(10 20, 15 30)", strategy_ps),
                  strategy_sb);
    tester::apply("test_ns2", "SEGMENT(0 10, 12 10)", box_south,
                  pp_distance<Point>("POINT(12 10)", "POINT(12 -10)", strategy_pp),
                  strategy_sb);
    tester::apply("test_ns3", "SEGMENT(10 10, 20 10)", box_south,
                  pp_distance<Point>("POINT(10 10)", "POINT(10 -10)", strategy_pp),
                  strategy_sb);
    tester::apply("test_ns4", "SEGMENT(0 -10, 12 -10)", box_north,
                  pp_distance<Point>("POINT(12 10)", "POINT(12 -10)", strategy_pp),
                  strategy_sb);
    tester::apply("test_ns5", "SEGMENT(10 -10, 20 -10)", box_north,
                  pp_distance<Point>("POINT(10 -10)", "POINT(10 10)", strategy_pp),
                  strategy_sb);

    //Box crossing equator
    std::string const box_crossing_eq = "BOX(10 -10,20 10)";

    tester::apply("test_cr1", "SEGMENT(10 20, 15 30)", box_crossing_eq,
                  pp_distance<Point>("POINT(10 10)", "POINT(10 20)", strategy_pp),
                  strategy_sb);
    tester::apply("test_cr2", "SEGMENT(10 -20, 15 -30)", box_crossing_eq,
                  pp_distance<Point>("POINT(10 10)", "POINT(10 20)", strategy_pp),
                  strategy_sb);

    //Box crossing prime meridian

    std::string const box_crossing_mer = "BOX(-10 10,15 20)";

    tester::apply("test_cr3", "SEGMENT(-5 25, 10 30)", box_crossing_mer,
                  pp_distance<Point>("POINT(-5 25)", "POINT(-5 20)", strategy_pp),
                  strategy_sb);
    tester::apply("test_cr4", "SEGMENT(-5 5, 10 7)", box_crossing_mer,
                  pp_distance<Point>("POINT(10 7)", "POINT(10 10)", strategy_pp),
                  strategy_sb);
    tester::apply("test_cr5", "SEGMENT(-5 5, 10 5)", box_crossing_mer,
                  ps_distance<Point>("POINT(2.5 10)", "SEGMENT(-5 5, 10 5)", strategy_ps),
                  strategy_sb);


    //Geometries in south hemisphere
    tester::apply("test_south1", "SEGMENT(10 -30, 15 -30)", box_south,
                  ps_distance<Point>("POINT(10 -20)", "SEGMENT(10 -30, 15 -30)", strategy_ps),
                  strategy_sb);

    //Segments in boxes corner
    tester::apply("corner1", "SEGMENT(17 21, 25 20)", box_north,
                  ps_distance<Point>("POINT(20 20)", "SEGMENT(17 21, 25 20)", strategy_ps),
                  strategy_sb);
    tester::apply("corner2", "SEGMENT(17 21, 0 20)", box_north,
                  ps_distance<Point>("POINT(10 20)", "SEGMENT(17 21, 0 20)", strategy_ps),
                  strategy_sb);
    tester::apply("corner3", "SEGMENT(17 5, 0 10)", box_north,
                  ps_distance<Point>("POINT(10 10)", "SEGMENT(17 5, 0 10)", strategy_ps),
                  strategy_sb);
    tester::apply("corner4", "SEGMENT(17 5, 25 9)", box_north,
                  ps_distance<Point>("POINT(20 10)", "SEGMENT(17 5, 25 9)", strategy_ps),
                  strategy_sb);
                  */
}

//===========================================================================

template <typename Point, typename Strategy>
void test_closest_points_linestring_polygon_or_ring(Strategy const& strategy)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "linestring/polygon or ring closest_points tests" << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::ring<Point> Ring;
    typedef bg::model::polygon<Point> Polygon;
    typedef test_geometry<Linestring, Ring, Segment> tester;

    tester::apply("LINESTRING(2 0,0 2)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategy);

    typedef test_geometry<Linestring, Polygon, Segment> tester2;

    tester2::apply("LINESTRING(2 0,0 2)",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategy);

    tester2::apply("LINESTRING(2 0,0 2)",
                   "POLYGON((0 0,1 0,0 1,0 0)(0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4))",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategy);
}


//===========================================================================

template <typename Point, typename Strategy>
void test_closest_points_linestring_multi_polygon(Strategy const& strategy)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "linestring/multi-polygon closest_points tests" << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::polygon<Point> Polygon;
    typedef bg::model::multi_polygon<Polygon> MultiPolygon;
    typedef test_geometry<Linestring, MultiPolygon, Segment> tester;

    tester::apply("LINESTRING(2 0,0 2)",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategy);
}

//===========================================================================

template <typename Point, typename Strategy>
void test_closest_points_multi_linestring_polygon_or_ring(Strategy
                                                          const& strategy)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "multi-linestring/polygon or ring closest_points tests"
              << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::multi_linestring<Linestring> MultiLinestring;
    typedef bg::model::ring<Point> Ring;
    typedef bg::model::polygon<Point> Polygon;
    typedef test_geometry<MultiLinestring, Ring, Segment> tester;

    tester::apply("MULTILINESTRING((2 0,0 2)(0 3,3 0))",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategy);

    typedef test_geometry<MultiLinestring, Polygon, Segment> tester2;

    tester2::apply("MULTILINESTRING((2 0,0 2)(0 3,3 0))",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategy);

    tester2::apply("MULTILINESTRING((2 0,0 2)(0 3,3 0))",
                   "POLYGON((0 0,1 0,0 1,0 0)(0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4))",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategy);
}

//===========================================================================

template <typename Point, typename Strategy>
void test_closest_points_multi_linestring_multi_polygon(Strategy const& strategy)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "multi-linestring/polygon or ring closest_points tests"
              << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::multi_linestring<Linestring> MultiLinestring;
    typedef bg::model::polygon<Point> Polygon;
    typedef bg::model::multi_polygon<Polygon> MultiPolygon;

    typedef test_geometry<MultiLinestring, MultiPolygon, Segment> tester;


    tester::apply("MULTILINESTRING((2 0,0 2)(0 3,3 0))",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                  ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategy);
}

//===========================================================================
//===========================================================================
//===========================================================================

template <typename Point, typename PSStrategy, typename PBStrategy>
void test_all_l_ar(PSStrategy cp_strategy, PBStrategy sb_strategy)
{
    test_closest_points_segment_polygon_or_ring<Point>(cp_strategy);
    test_closest_points_segment_multi_polygon<Point>(cp_strategy);
    test_closest_points_segment_box<Point>(sb_strategy);

    test_closest_points_linestring_polygon_or_ring<Point>(cp_strategy);
    test_closest_points_linestring_multi_polygon<Point>(cp_strategy);
    //test_closest_points_linestring_box<Point>(cp_strategy);

    test_closest_points_multi_linestring_polygon_or_ring<Point>(cp_strategy);
    test_closest_points_multi_linestring_multi_polygon<Point>(cp_strategy);
    //test_closest_points_multi_linestring_box<Point>(cp_strategy);

    //test_more_empty_input_pointlike_areal<Point>(cp_strategy);
}

BOOST_AUTO_TEST_CASE( test_all_linear_areal )
{
    typedef bg::model::point
            <
                double, 2,
                bg::cs::geographic<bg::degree>
            > geo_point;

    test_all_l_ar<geo_point>(andoyer_cp(), andoyer_sb());
    test_all_l_ar<geo_point>(thomas_cp(), thomas_sb());
    test_all_l_ar<geo_point>(vincenty_cp(), vincenty_sb());
}