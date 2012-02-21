#ifndef _DLVHEX_DF_DFCONVERTER_H_
#define _DLVHEX_DF_DFCONVERTER_H_

#include <dlvhex2/ComfortPluginInterface.h>
#include "Ontology.h"
#include "DFProcessor.h"
#include "Registry.h"

namespace dlvhex {
namespace df {

  class DFConverter : public PluginConverter
  {
  private:
    bool cqmode;
    int dftrans;
    std::string dlprogram;

    // current Ontology
    dlvhex::dl::Ontology::shared_pointer ontology;
    
    // list of individuals in the ontology
    std::list<std::string> individuals;
    
    void readIndividuals(dlvhex::dl::Ontology::shared_pointer);
    
  public:
    DFConverter();

    bool
      hasDefaults();

    void
      setCQmode(bool);

    void
      setDFTrans(int);
    
    void
      setOntology(const dlvhex::dl::Ontology::shared_pointer&);
    
    void
      convert(std::istream& i, std::ostream& o);
  };

} // namespace df
} // namespace dlvhex

#endif /* _DLVHEX_DF_DFCONVERTER_H_ */
