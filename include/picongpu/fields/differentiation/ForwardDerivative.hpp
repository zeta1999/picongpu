/* Copyright 2013-2020 Heiko Burau, Rene Widera, Axel Huebl, Sergei Bastrakov
 *
 * This file is part of PIConGPU.
 *
 * PIConGPU is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PIConGPU is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PIConGPU.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "picongpu/simulation_defines.hpp"
#include "picongpu/fields/differentiation/Derivative.def"
#include "picongpu/fields/differentiation/Traits.hpp"
#include "picongpu/traits/GetMargin.hpp"

#include <pmacc/meta/accessors/Identity.hpp>

#include <cstdint>


namespace picongpu
{
namespace fields
{
namespace differentiation
{

    /** Functor for forward difference derivative along the given direction
     *
     * Computes (upper - current) / step, previously called DifferenceToUpper.
     *
     * @tparam T_direction direction to take derivative in, 0 = x, 1 = y, 2 = z
     */
    template< uint32_t T_direction >
    struct ForwardDerivativeFunctor
    {
        /** Return derivative value at the given point
         *
         * @tparam T_DataBox data box type with field data
         * @param data position in the data box to compute derivative at
         */
        template< typename T_DataBox >
        HDINLINE typename T_DataBox::ValueType operator()(
            T_DataBox const & data ) const
        {
            pmacc::DataSpace< simDim > const currentIndex;
            pmacc::DataSpace< simDim > upperIndex;
            upperIndex[ T_direction ] = 1;
            return ( data( upperIndex ) - data( currentIndex ) ) /
                cellSize[ T_direction ];
        }
    };

namespace traits
{

    /** Functor type trait specialization for forward derivative
     *
     * @tparam T_direction direction to take derivative in, 0 = x, 1 = y, 2 = z
     */
    template< uint32_t T_direction >
    struct DerivativeFunctor<
        Forward,
        T_direction
    > : pmacc::meta::accessors::Identity<
            ForwardDerivativeFunctor< T_direction >
    >
    {
    };

} // namespace traits
} // namespace differentiation
} // namespace fields

namespace traits
{

    //! Get margin of the forward derivative
    template<>
    struct GetMargin< fields::differentiation::Forward >
    {
        using LowerMargin = typename pmacc::math::CT::make_Int<
            simDim,
            0
        >::type;
        using UpperMargin = typename pmacc::math::CT::make_Int<
            simDim,
            1
        >::type;
    };

} // namespace traits
} // namespace picongpu