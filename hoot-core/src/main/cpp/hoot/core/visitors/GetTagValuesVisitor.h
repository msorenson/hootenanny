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
 * @copyright Copyright (C) 2013, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef GETTAGVALUESVISITOR_H
#define GETTAGVALUESVISITOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/OsmMapConsumer.h>

namespace hoot
{
using namespace std;

/**
 * Puts all values for the given key into a bag. If you want to filter based on type see
 * FilteredVisitor. If the values are a list then they're split before they're put in the bag.
 */
class GetTagValuesVisitor : public ElementVisitor, public OsmMapConsumer
{
public:

  /**
   * @param split If split is set to true then the values in the tag are split before they're placed
   *  in the bag.
   */
  GetTagValuesVisitor(QString key, set<QString>& bag, bool split=false) :
    _key(key),
    _bag(bag),
    _split(split)
  {}

  virtual ~GetTagValuesVisitor() {}

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(ElementType type, long id)
  {
    ConstElementPtr e = _map->getElement(type, id);
    Tags::const_iterator it = e->getTags().find(_key);
    if (it != e->getTags().end())
    {
      if (_split)
      {
        QStringList l;
        e->getTags().readValues(_key, l);
        for (int i = 0; i < l.size(); i++)
        {
          _bag.insert(l[i]);
        }
      }
      else
      {
        _bag.insert(it.value());
      }
    }
  }

private:
  QString _key;
  set<QString>& _bag;
  const OsmMap* _map;
  bool _split;
};

}

#endif // GETTAGVALUESVISITOR_H
