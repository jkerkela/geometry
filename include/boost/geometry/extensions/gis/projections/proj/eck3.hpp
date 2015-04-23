#ifndef BOOST_GEOMETRY_PROJECTIONS_ECK3_HPP
#define BOOST_GEOMETRY_PROJECTIONS_ECK3_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 4.8.0

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


#include <boost/core/ignore_unused.hpp>
#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace eck3{ 

            struct par_eck3
            {
                double C_x, C_y, A, B;
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_eck3_spheroid : public base_t_fi<base_eck3_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_eck3 m_proj_parm;

                inline base_eck3_spheroid(const Parameters& par)
                    : base_t_fi<base_eck3_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    xy_y = this->m_proj_parm.C_y * lp_lat;
                    xy_x = this->m_proj_parm.C_x * lp_lon * (this->m_proj_parm.A + asqrt(1. - this->m_proj_parm.B * lp_lat * lp_lat));
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    lp_lat = xy_y / this->m_proj_parm.C_y;
                    lp_lon = xy_x / (this->m_proj_parm.C_x * (this->m_proj_parm.A + asqrt(1. - this->m_proj_parm.B * lp_lat * lp_lat)));
                }
            };

            template <typename Parameters>
            void setup(Parameters& par, par_eck3& proj_parm) 
            {
                boost::ignore_unused(par);
                boost::ignore_unused(proj_parm);
                par.es = 0.;
                // par.inv = s_inverse;
                // par.fwd = s_forward;
            }


            // Eckert III
            template <typename Parameters>
            void setup_eck3(Parameters& par, par_eck3& proj_parm)
            {
                proj_parm.C_x = .42223820031577120149;
                proj_parm.C_y = .84447640063154240298;
                proj_parm.A = 1.;
                proj_parm.B = 0.4052847345693510857755;
                setup(par, proj_parm);
            }

            // Putnins P1
            template <typename Parameters>
            void setup_putp1(Parameters& par, par_eck3& proj_parm)
            {
                proj_parm.C_x = 1.89490;
                proj_parm.C_y = 0.94745;
                proj_parm.A = -0.5;
                proj_parm.B = 0.30396355092701331433;
                setup(par, proj_parm);
            }

            // Wagner VI
            template <typename Parameters>
            void setup_wag6(Parameters& par, par_eck3& proj_parm)
            {
                proj_parm.C_x = proj_parm.C_y = 0.94745;
                proj_parm.A = 0.;
                proj_parm.B = 0.30396355092701331433;
                setup(par, proj_parm);
            }

            // Kavraisky VII
            template <typename Parameters>
            void setup_kav7(Parameters& par, par_eck3& proj_parm)
            {
                proj_parm.C_x = 0.2632401569273184856851;
                proj_parm.C_x = 0.8660254037844;
                proj_parm.C_y = 1.;
                proj_parm.A = 0.;
                proj_parm.B = 0.30396355092701331433;
                setup(par, proj_parm);
            }

        }} // namespace detail::eck3
    #endif // doxygen 

    /*!
        \brief Eckert III projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Example
        \image html ex_eck3.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct eck3_spheroid : public detail::eck3::base_eck3_spheroid<Geographic, Cartesian, Parameters>
    {
        inline eck3_spheroid(const Parameters& par) : detail::eck3::base_eck3_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::eck3::setup_eck3(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Putnins P1 projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Example
        \image html ex_putp1.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct putp1_spheroid : public detail::eck3::base_eck3_spheroid<Geographic, Cartesian, Parameters>
    {
        inline putp1_spheroid(const Parameters& par) : detail::eck3::base_eck3_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::eck3::setup_putp1(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Wagner VI projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Example
        \image html ex_wag6.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct wag6_spheroid : public detail::eck3::base_eck3_spheroid<Geographic, Cartesian, Parameters>
    {
        inline wag6_spheroid(const Parameters& par) : detail::eck3::base_eck3_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::eck3::setup_wag6(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Kavraisky VII projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Example
        \image html ex_kav7.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct kav7_spheroid : public detail::eck3::base_eck3_spheroid<Geographic, Cartesian, Parameters>
    {
        inline kav7_spheroid(const Parameters& par) : detail::eck3::base_eck3_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::eck3::setup_kav7(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class eck3_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<eck3_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        class putp1_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<putp1_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        class wag6_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<wag6_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        class kav7_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<kav7_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void eck3_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("eck3", new eck3_entry<Geographic, Cartesian, Parameters>);
            factory.add_to_factory("putp1", new putp1_entry<Geographic, Cartesian, Parameters>);
            factory.add_to_factory("wag6", new wag6_entry<Geographic, Cartesian, Parameters>);
            factory.add_to_factory("kav7", new kav7_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail 
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_ECK3_HPP

