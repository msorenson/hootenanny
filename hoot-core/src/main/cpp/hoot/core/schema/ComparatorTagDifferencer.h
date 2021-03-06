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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef COMPARATORTAGDIFFERENCER_H
#define COMPARATORTAGDIFFERENCER_H

#include "TagDifferencer.h"
#include "TagComparator.h"

namespace hoot
{

/**
 * A fascade for the TagComparator compareTags method. See TagComparator for details.
 */
class ComparatorTagDifferencer : public TagDifferencer
{
public:
  virtual double diff(const Tags& t1, const Tags& t2) const
  {
    return 1 - TagComparator::getInstance().compareTags(t1, t2);
  }
};

}

#endif // COMPARATORTAGDIFFERENCER_H
