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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PertyDuplicatePoiOp.h"

// boost
#include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/MapReprojector.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/RandomNumberUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, PertyDuplicatePoiOp)

PertyDuplicatePoiOp::PertyDuplicatePoiOp()
{
  _localRng.reset(new boost::minstd_rand(1));
  _rng = _localRng.get();

  setConfiguration(conf());
}

QString PertyDuplicatePoiOp::toString()
{
  return "_p: " + QString::number(_p) + ", _duplicateSigma: " + QString::number(_duplicateSigma) +
    ", _moveMultiplier: "  + QString::number(_moveMultiplier);
}

void PertyDuplicatePoiOp::apply(shared_ptr<OsmMap>& map)
{
  MapReprojector::reprojectToPlanar(map);
  boost::uniform_real<> uni(0.0, 1.0);
  boost::normal_distribution<> nd;
  boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<> > N(*_rng, nd);

  // make a copy since we'll be modifying the map as we go.
  OsmMap::NodeMap nm = map->getNodeMap();
  for (OsmMap::NodeMap::const_iterator it = nm.constBegin(); it != nm.constEnd(); ++it)
  {
    if (uni(*_rng) < _p)
    {
      const NodePtr& n = it.value();
      int copies = round(fabs(N() * _duplicateSigma)) + 1;

      for (int i = 0; i < copies; i++)
      {
        duplicateNode(n, map);
      }
    }
  }
}

void PertyDuplicatePoiOp::duplicateNode(const NodePtr& n, const OsmMapPtr& map)
{
  boost::normal_distribution<> nd;
  boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<> > N(*_rng, nd);

  Meters sigma = n->getCircularError() / 2.0;
  double x = n->getX() + N() * sigma * _moveMultiplier;
  double y = n->getY() + N() * sigma * _moveMultiplier;

  NodePtr newNode(new Node(n->getStatus(), map->createNextNodeId(), x, y,
    n->getCircularError()));

  map->addNode(newNode);
}

void PertyDuplicatePoiOp::setConfiguration(const Settings& conf)
{
  setDuplicateSigma(conf.getDouble(duplicateSigmaKey(), defaultDuplicateSigma()));
  setProbability(conf.getDouble(pKey(), defaultProbability()));
  setMoveMultiplier(conf.getDouble(moveMultiplierKey(), defaultMoveMultiplier()));
  ConfigOptions configOptions(conf);
  const int seed = configOptions.getPertySeed();
  LOG_VARD(seed);
  if (seed == -1)
  {
    _rng->seed(RandomNumberUtils::generateSeed());
  }
  else
  {
    _rng->seed(seed);
  }
}

}
