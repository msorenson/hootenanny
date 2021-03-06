/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "WayFilterChain.h"

// Hoot
#include <hoot/core/elements/Way.h>

namespace hoot
{

WayFilterChain::WayFilterChain()
{
  _aggregator = Or;
}

WayFilterChain::WayFilterChain(shared_ptr<WayFilter> f1)
{
  addFilter(f1);
  _aggregator = Or;
}

WayFilterChain::WayFilterChain(shared_ptr<WayFilter> f1, shared_ptr<WayFilter> f2)
{
  addFilter(f1);
  addFilter(f2);
  _aggregator = Or;
}

bool WayFilterChain::isFiltered(const shared_ptr<const Way>& w) const
{
  if (_aggregator == Or)
  {
    for (size_t i = 0; i < _filters.size(); i++)
    {
      if (_filtersFast[i]->isFiltered(w) == true)
      {
        return true;
      }
    }
    return false;
  }
  else
  {
    for (size_t i = 0; i < _filters.size(); i++)
    {
      if (_filtersFast[i]->isFiltered(w) == false)
      {
        return false;
      }
    }
    return true;
  }

  return false;
}

}
