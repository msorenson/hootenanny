/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "WayJoin2Mapper.h"

// Hoot
#include <hoot/core/filters/TagFilter.h>
#include <hoot/core/io/PbfReader.h>
#include <hoot/core/io/PbfWriter.h>

// Pretty Pipes
#include <pp/DataOutputStream.h>
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>

// Standard
#include <stdlib.h>
#include <time.h>

#include "WayJoin1Mapper.h"

namespace hoot
{

PP_FACTORY_REGISTER(pp::Mapper, WayJoin2Mapper)

WayJoin2Mapper::WayJoin2Mapper()
{
  _reader = NULL;

  _rawValueStr.resize(sizeof(RawValueStruct));
  _rawValue = (RawValueStruct*)(_rawValueStr.data());
  _rawValue->dataType = RawData;

  _keyStr.resize(sizeof(KeyStruct));
  _key = (KeyStruct*)_keyStr.data();

  _nodeMap.reset(new OsmMap());
  srand(time(NULL));
}

void WayJoin2Mapper::_emitNode(const shared_ptr<Node>& n)
{
  _nodeMap->addNode(n);

  if (_nodeMap->getNodeMap().size() >= 100000)
  {
    _flushNodes();
  }
}

void WayJoin2Mapper::_flushNodes()
{
  // this map should only contain nodes.
  assert(_nodeMap->getWays().size() == 0);
  if (_nodeMap->getNodeMap().size() > 0)
  {
    LOG_INFO("Flushing nodes. " << _nodeMap->getNodeMap().size());
    _key->elementType = NodesType;
    _key->id = rand();
    stringstream ss(stringstream::out);
    pp::DataOutputStream dos(ss);

    dos.writeByte(PbfData);
    PbfWriter writer;
    // RHEL calls this ambiguous.
    const shared_ptr<const OsmMap>& co = _nodeMap;
    writer.writePb(co, &ss);
    _context->emit(_keyStr, ss.str());
    _nodeMap->clear();
  }
  else
  {
    LOG_INFO("No nodes to flush.");
  }
}

void WayJoin2Mapper::map(HadoopPipes::MapContext& context)
{
  _context = &context;
  if (_reader == NULL)
  {
    HadoopPipes::RecordReader* rr = pp::HadoopPipesUtils::getRecordReader(&context);
    _reader = dynamic_cast<WayJoin2RecordReader*>(rr);
    if (_reader == NULL)
    {
      throw InternalErrorException("Expected a record reader of type WayJoin2RecordReader");
    }
  }

  if (_reader->getRecordType() == WayJoin2InputSplit::PbfInputSplitType)
  {
    mapOsmMap(_reader->getMap());
  }
  else
  {
    const string& key = context.getInputKey();
    const string& value = context.getInputValue();

    if (key.size() != sizeof(int64_t))
    {
      throw InternalErrorException("Expected key to be a int64_t");
    }
    if (value.size() != sizeof(WayJoin1Reducer::Value))
    {
      throw InternalErrorException("Expected value to be a WayJoin1Reducer::Value");
    }

    int64_t* k = (int64_t*)key.data();
    WayJoin1Reducer::Value* v = (WayJoin1Reducer::Value*)value.data();
    mapWayPoints(*k, *v);
  }
}

void WayJoin2Mapper::mapOsmMap(shared_ptr<OsmMap> m)
{
  // The first byte on the value says if it is a PBF/WayJoin1Reducer::Value
  PbfWriter writer;

  // Remove all non-roads.
  m->removeWays(TagFilter(Filter::FilterMatches, "highway", ""));

  _key->elementType = NodesType;
  // Go through all the nodes
  const OsmMap::NodeMap& nm = m->getNodeMap();
  for (OsmMap::NodeMap::const_iterator it = nm.constBegin(); it != nm.constEnd(); ++it)
  {
    // add this node onto a map. Since the nodes aren't very important at this stage we'll just
    // ship a bunch at a time in one big record.
    _emitNode(it.value());
  }

  _key->elementType = WayType;
  // Go through all the ways
  const WayMap& wm = m->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    const shared_ptr<const Way>& w = it->second;

    _key->id = w->getId();
    stringstream ss(stringstream::out);
    pp::DataOutputStream dos(ss);

    //LOG_INFO("Writing way: " << _key->id);

    dos.writeByte(PbfData);
    writer.writePb(w, &ss);

    // Emit the way
    _context->emit(_keyStr, ss.str());
  }
}

void WayJoin2Mapper::mapWayPoints(int64_t& k, WayJoin1Reducer::Value& v)
{
  _key->elementType = WayType;
  _key->id = k;

  _rawValue->rawWay = v;
  //LOG_INFO("Writing way raw: " << _key->id);

  _context->emit(_keyStr, _rawValueStr);
}

}
