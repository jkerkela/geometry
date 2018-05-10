// Boost.Geometry - gis-projections (based on PROJ4)

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// This file was modified by Oracle on 2017, 2018.
// Modifications copyright (c) 2017-2018, Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 5.0.0

// Original copyright notice:

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef BOOST_GEOMETRY_PROJECTIONS_LOXIM_HPP
#define BOOST_GEOMETRY_PROJECTIONS_LOXIM_HPP

#include <boost/geometry/util/math.hpp>

#include <boost/geometry/srs/projections/impl/base_static.hpp>
#include <boost/geometry/srs/projections/impl/base_dynamic.hpp>
#include <boost/geometry/srs/projections/impl/projects.hpp>
#include <boost/geometry/srs/projections/impl/factory_entry.hpp>

namespace boost { namespace geometry
{

namespace srs { namespace par4
{
    struct loxim {}; // Loximuthal

}} //namespace srs::par4

namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace loxim
    {
            static const double EPS = 1e-8;

            template <typename T>
            struct par_loxim
            {
                T phi1;
                T cosphi1;
                T tanphi1;
            };

            // template class, using CRTP to implement forward/inverse
            template <typename CalculationType, typename Parameters>
            struct base_loxim_spheroid : public base_t_fi<base_loxim_spheroid<CalculationType, Parameters>,
                     CalculationType, Parameters>
            {

                typedef CalculationType geographic_type;
                typedef CalculationType cartesian_type;

                par_loxim<CalculationType> m_proj_parm;

                inline base_loxim_spheroid(const Parameters& par)
                    : base_t_fi<base_loxim_spheroid<CalculationType, Parameters>,
                     CalculationType, Parameters>(*this, par) {}

                // FORWARD(s_forward)  spheroid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    static const CalculationType FORTPI = detail::FORTPI<CalculationType>();
                    static const CalculationType HALFPI = detail::HALFPI<CalculationType>();

                    xy_y = lp_lat - this->m_proj_parm.phi1;
                    if (fabs(xy_y) < EPS)
                        xy_x = lp_lon * this->m_proj_parm.cosphi1;
                    else {
                        xy_x = FORTPI + 0.5 * lp_lat;
                        if (fabs(xy_x) < EPS || fabs(fabs(xy_x) - HALFPI) < EPS)
                            xy_x = 0.;
                        else
                            xy_x = lp_lon * xy_y / log( tan(xy_x) / this->m_proj_parm.tanphi1 );
                    }
                }

                // INVERSE(s_inverse)  spheroid
                // Project coordinates from cartesian (x, y) to geographic (lon, lat)
                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    static const CalculationType FORTPI = detail::FORTPI<CalculationType>();
                    static const CalculationType HALFPI = detail::HALFPI<CalculationType>();

                    lp_lat = xy_y + this->m_proj_parm.phi1;
                    if (fabs(xy_y) < EPS) {
                        lp_lon = xy_x / this->m_proj_parm.cosphi1;
                    } else {
                        lp_lon = FORTPI + 0.5 * lp_lat;
                        if (fabs(lp_lon) < EPS || fabs(fabs(lp_lon) - HALFPI) < EPS)
                            lp_lon = 0.;
                        else
                            lp_lon = xy_x * log( tan(lp_lon) / this->m_proj_parm.tanphi1 ) / xy_y ;
                    }
                }

                static inline std::string get_name()
                {
                    return "loxim_spheroid";
                }

            };

            // Loximuthal
            template <typename Parameters, typename T>
            inline void setup_loxim(Parameters& par, par_loxim<T>& proj_parm)
            {
                static const T FORTPI = detail::FORTPI<T>();

                proj_parm.phi1 = pj_get_param_r(par.params, "lat_1");
                proj_parm.cosphi1 = cos(proj_parm.phi1);
                if (proj_parm.cosphi1 < EPS)
                    BOOST_THROW_EXCEPTION( projection_exception(-22) );

                proj_parm.tanphi1 = tan(FORTPI + 0.5 * proj_parm.phi1);

                par.es = 0.;
            }

    }} // namespace detail::loxim
    #endif // doxygen

    /*!
        \brief Loximuthal projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Projection parameters
         - lat_1: Latitude of first standard parallel (degrees)
        \par Example
        \image html ex_loxim.gif
    */
    template <typename CalculationType, typename Parameters>
    struct loxim_spheroid : public detail::loxim::base_loxim_spheroid<CalculationType, Parameters>
    {
        inline loxim_spheroid(const Parameters& par) : detail::loxim::base_loxim_spheroid<CalculationType, Parameters>(par)
        {
            detail::loxim::setup_loxim(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Static projection
        BOOST_GEOMETRY_PROJECTIONS_DETAIL_STATIC_PROJECTION(srs::par4::loxim, loxim_spheroid, loxim_spheroid)

        // Factory entry(s)
        template <typename CalculationType, typename Parameters>
        class loxim_entry : public detail::factory_entry<CalculationType, Parameters>
        {
            public :
                virtual base_v<CalculationType, Parameters>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<loxim_spheroid<CalculationType, Parameters>, CalculationType, Parameters>(par);
                }
        };

        template <typename CalculationType, typename Parameters>
        inline void loxim_init(detail::base_factory<CalculationType, Parameters>& factory)
        {
            factory.add_to_factory("loxim", new loxim_entry<CalculationType, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

} // namespace projections

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_PROJECTIONS_LOXIM_HPP

