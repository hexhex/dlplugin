/**
 * TODO copyright etc.
 *
 * This file contains the base converter used for DL syntactic sugar: the parser
 * detects DL syntactic sugar and passes it on to the DLAtomConverter for
 * rewriting, everything that is not DL syntactic sugar is simply streamed out
 * output unaltered.
 */
#ifndef _HEXDLCONVERTER_H_INCLUDED
#define _HEXDLCONVERTER_H_INCLUDED

#include "Ontology.h"

#include <dlvhex/PluginInterface.h>

namespace dlvhex {
namespace dl {

class HexDLConverter : public PluginConverter
{
public:
  HexDLConverter();
  virtual ~HexDLConverter();

  void convert(std::istream& i, std::ostream& o);

  void setOntology(const Ontology::shared_pointer& o) {} // <-- TODO

  Ontology::shared_pointer getOntology() const { return Ontology::shared_pointer(); } // <-- TODO
};

}
}

#endif // _HEXDLCONVERTER_H_INCLUDED
// vim:ts=8:tw=100:
