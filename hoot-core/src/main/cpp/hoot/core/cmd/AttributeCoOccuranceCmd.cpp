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

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/scoring/AttributeCoOccurence.h>


namespace hoot
{

class AttributeCoOccurenceCmd : public BaseCommand
{
public:

  static string className() { return "hoot::AttributeCoOccurenceCmd"; }

  AttributeCoOccurenceCmd() { }

  virtual QString getHelp() const
  {
    // 80 columns
    //  | <---                                                                      ---> |
    return getName() + " [--no-table] (input1 input2)\n"
        "  Reads from inputs and outputs a co-occurence matrix based on attribute values\n"
        "  * --no-table - print the matrix as a list The default is to print as a table\n"
        "  * input1 - Input 1 (e.g. .osm file).\n"
        "  * input2 - Input 2 (e.g. .osm file).\n";
  }

  virtual QString getName() const { return "attr-co-occurence"; }

  virtual int runSimple(QStringList args)
  {
    bool tableFormat = true;
    AttributeCoOccurence cooccurence;

    // Print a table or just a list
    if (args.contains("--no-table"))
    {
      args.removeAll("--no-table");
      tableFormat = false;
    }

    if (args.size() != 2)
    {
      LOG_VAR(args);
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").
                          arg(getName()));
    }


    shared_ptr<OsmMap> map(new OsmMap());
    loadMap(map, args[0], false, Status::Unknown1);
    loadMap(map, args[1], false, Status::Unknown2);

    cooccurence.addToMatrix(map);

    if (tableFormat)
      cout << cooccurence.printTable();
    else
      cout << cooccurence.printList();

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, AttributeCoOccurenceCmd)

}

