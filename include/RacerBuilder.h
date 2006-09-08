/* -*- C++ -*- */

/**
 * @file   RacerBuilder.h
 * @author Thomas Krennwallner
 * @date   Fri Jul 15 14:28:59 2005
 * 
 * @brief  Various builders for RACER commands.
 * 
 * 
 */

#ifndef _RACERBUILDER_H
#define _RACERBUILDER_H

#include "DLError.h"
#include "Query.h"
#include "QueryDirector.h"
#include "KBManager.h"

#include <iostream>
#include <string>

namespace dlvhex {
namespace dl {
namespace racer {


  /**
   * @brief A Builder which does nothing. Mainly for testing purposes.
   */
  class RacerNullBuilder : public QueryBaseBuilder
  {
  public:
    explicit
    RacerNullBuilder(std::ostream& s)
      : QueryBaseBuilder(s)
    { }

    /**
     * @brief Ignores its input.
     */
    virtual bool
    buildCommand(Query&) throw (DLBuildingError)
    { return true; }
  };


  /**
   * @brief Creates a state command for adding a list of
   * Individuals/Pairs to Concepts/Roles.
   *
   * @see state macro in RacerPro Reference Manual
   */
  class RacerStateBuilder : public QueryBaseBuilder
  {
  public:
    explicit
    RacerStateBuilder(std::ostream&);

    /**
     * Calls various other building methods in order to generate a
     * state command.
     *
     * @param q use the interpretation of Query to generate a state
     * command
     *
     * @return true if we could add something to the abox, false otherwise.
     */
    virtual bool
    buildCommand(Query& q) throw (DLBuildingError);
  };


  /**
   * @brief Creates an individual-instance? command in order to check
   * whether an individual is a member of a concept.
   *
   * @see individual-instance? macro in RacerPro Reference Manual
   */
  class RacerIsConceptMemberBuilder : public QueryBaseBuilder
  {
  public:
    explicit
    RacerIsConceptMemberBuilder(std::ostream&);

    /**
     * Generate a individual-instance? command.
     *
     * @param q use Query to get the individual and the concept name.
     */
    virtual bool
    buildCommand(Query& q) throw (DLBuildingError);
  };


  /**
   * @brief Creates an individuals-related? command in order to check
   * whether a pair is a member of a role.
   *
   * @see individuals-related? macro in RacerPro Reference Manual
   */
  class RacerIsRoleMemberBuilder : public QueryBaseBuilder
  {
  public:
    explicit
    RacerIsRoleMemberBuilder(std::ostream&);

    /**
     * Generate a individuals-related? command.
     *
     * @param q use Query to get the pair and the role name.
     */
    virtual bool
    buildCommand(Query& q) throw (DLBuildingError);
  };


  /**
   * @brief Creates a command to query concept instances.
   *
   * @see concept-instances macro in RacerPro Reference Manual
   */
  class RacerConceptInstancesBuilder : public QueryBaseBuilder
  {
  public:
    explicit
    RacerConceptInstancesBuilder(std::ostream&);

    virtual bool
    buildCommand(Query& q) throw (DLBuildingError);
  };


  /**
   * @brief Creates a command to query role individuals.
   *
   * @see related-individuals macro in RacerPro Reference manual
   */
  class RacerRoleIndividualsBuilder : public QueryBaseBuilder
  {
  public:
    explicit
    RacerRoleIndividualsBuilder(std::ostream&);

    virtual bool
    buildCommand(Query& q) throw (DLBuildingError);
  };


  /**
   * @brief Creates a command to query role individual fillers.
   *
   * @see individual-fillers macro in RacerPro Reference manual
   */
  class RacerIndividualFillersBuilder : public QueryBaseBuilder
  {
  public:
    explicit
    RacerIndividualFillersBuilder(std::ostream&);

    virtual bool
    buildCommand(Query& q) throw (DLBuildingError);
  };


  /**
   * @brief Opens an OWL from file or URL as KB "DEFAULT".
   *
   * @see owl-read-file and owl-read-document functions in RacerPro
   * Reference manual.
   */
  class RacerOpenOWLBuilder : public QueryBaseBuilder
  {
  public:
    explicit
    RacerOpenOWLBuilder(std::ostream&);

    virtual bool
    buildCommand(Query& q) throw (DLBuildingError);
  };


  /**
   * @brief An adapter for classes with a ctor
   * Adaptee::Adaptee(Query&) and an operator<<(ostream&, const
   * Adaptee&).
   *
   * @see NRQLQuery
   */
  template<class Adaptee>
  class RacerAdapterBuilder : public QueryBaseBuilder
  {
  public:
    explicit
    RacerAdapterBuilder(std::ostream& stream)
      : QueryBaseBuilder(stream)
    { }

    virtual bool
    buildCommand(Query& q) throw (DLBuildingError)
    {
      try
	{
	  stream << Adaptee(q) << std::endl;
	}
      catch (std::exception& e)
	{
	  throw DLBuildingError(e.what());
	}

      return true;
    }

  };


  /**
   * @brief An adapter for classes with a ctor
   * AdapteeFun::AdapteeFun(Query&), a method AdapteeFun::operator()()
   * and an operator<<(ostream&, const Adaptee&).
   *
   * @see
   */
  template<class AdapteeFun>
  class RacerFunAdapterBuilder : public QueryBaseBuilder
  {
  public:
    explicit
    RacerFunAdapterBuilder(std::ostream& stream)
      : QueryBaseBuilder(stream)
    { }

    virtual bool
    buildCommand(Query& q) throw (DLBuildingError)
    {
      try
	{
	  AdapteeFun f;
	  stream << f(q) << std::endl;
	}
      catch (std::exception& e)
	{
	  throw DLBuildingError(e.what());
	}

      return true;
    }

  };



  //
  // the following commands can be used in as template parameter in
  // RacerFunAdapterBuilder
  //


  /**
   * @brief Requests listing of all individuals.
   *
   * @see all-individuals function in RacerPro Reference manual.
   */
  struct RacerAllIndividualsCmd
  {
    const std::string
    operator() (Query& query)
    {
      return "(all-individuals " + query.getKBManager().getKBName() + ")";
    }
  };


  /**
   * @brief Sets the Unique Name Assumption.
   *
   * @see set-unique-name-assumption function in RacerPro Reference manual. 
   */
  struct RacerUNACmd
  {
    const char*
    operator() (Query&)
    {
      return (*this)();
    }

    const char*
    operator() ()
    {
      return "(set-unique-name-assumption t)";
    }
  };


  /**
   * @brief Clone the Ontology's |realuri| ABox into the KBManager's
   * kb-name.
   *
   * @see clone-abox macro in RacerPro Reference manual.
   */
  struct RacerCloneABoxCmd
  {
    const std::string
    operator() (Query& query)
    {
      return "(clone-abox |"
	+ query.getDLQuery()->getOntology()->getRealURI().getString() 
	+ "| :new-name "
	+ query.getKBManager().getKBName()
	+ " :overwrite t)";
    }
  };


  /**
   * @brief Checks ABox consistency.
   *
   * @see abox-consistent? macro in RacerPro Reference manual.
   */
  struct RacerABoxConsistentCmd
  {
    const std::string
    operator() (Query& query)
    {
      return "(abox-consistent? " + query.getKBManager().getKBName()  + ")";
    }
  };


  /**
   * @brief Enable data substrate mirroring.
   *
   * @see enable-data-substrate-mirroring function in RacerPro Reference manual.
   */
  struct RacerDataSubstrateMirroringCmd
  {
    const char*
    operator() (Query&)
    {
      return (*this)();
    }

    const char*
    operator() ()
    {
      return "(enable-data-substrate-mirroring)";
    }
  };


  /**
   * @brief Enable imports in KB |realuri|, i.e. read all owl:imports.
   *
   * @see kb-ontologies in RacerPro Reference manual.
   */
  struct RacerImportOntologiesCmd
  {
    const std::string
    operator() (Query& query)
    {
      return "(kb-ontologies |" + query.getDLQuery()->getOntology()->getRealURI().getString() + "|)";
    }
  };


  /**
   * @brief Remove an ABox from RACER.
   *
   * @see forget-abox function in RacerPro Reference manual.
   */
  struct RacerForgetABoxCmd
  {
    const std::string
    operator() (Query& query) const
    {
      return (*this)(query.getKBManager());
    }

    const std::string
    operator() (KBManager& k) const
    {
      return "(forget-abox " + k.getKBName() + ")";
    }
  };


  /**
   * @brief Request a list of all currently loaded TBoxes.
   *
   * @see all-tboxes function in RacerPro Reference manual.
   */
  struct RacerAllTBoxesCmd
  {
    const char *
    operator() (Query&) const
    {
      return (*this)();
    }

    const char *
    operator() () const
    {
      return "(all-tboxes)";
    }
  };



} // namespace racer
} // namespace dl
} // namespace dlvhex

#endif /* _RACERBUILDER_H */
