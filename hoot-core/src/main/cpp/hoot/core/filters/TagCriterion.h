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
#ifndef TAGCRITERION_H
#define TAGCRITERION_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QString>

namespace hoot
{
using namespace boost;

/**
 * Returns true if k==v
 */
class TagCriterion : public ElementCriterion, public Configurable
{
public:
  static string className() { return "hoot::TagCriterion"; }

  TagCriterion();
  TagCriterion(const QString& k, const QString& v) : _k(k), _v(v) {}

  bool isSatisfied(const shared_ptr<const Element> &e) const
  {
    assert(!_k.isEmpty());
    return e->getTags().get(_k) == _v;
  }

  void setConfiguration(const Settings& s);

private:
  QString _k, _v;
};

}

#endif // TAGCRITERION_H
