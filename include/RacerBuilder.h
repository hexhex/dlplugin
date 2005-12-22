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

#include <iostream>
#include <string>

#include <dlvhex/Atom.h>
#include <dlvhex/Term.h>

#include "RacerError.h"
#include "RacerQuery.h"

namespace dlvhex {
namespace racer {

  /**
   * @brief Base Builder for building RACER commands.
   */
  class RacerBuilder
  {
  protected:
    /// the builders write commands to this stream
    std::ostream& stream;

    /**
     * Ctor
     *
     * @param s used as output stream
     */
    explicit
    RacerBuilder(std::ostream& s);

    /// Dtor
    virtual
    ~RacerBuilder();

  public:
    /**
     * Building method implemented by the children of RacerBuilder.
     *
     * @param q contains the information in order to create RACER commands
     */
    virtual void
    buildCommand(Query& q) = 0;
  };


  /**
   * @brief A Builder which does nothing. Mainly for testing purposes.
   */
  class RacerNullBuilder : public RacerBuilder
  {
  public:
    explicit
    RacerNullBuilder(std::ostream& s)
      : RacerBuilder(s)
    { }

    virtual
    ~RacerNullBuilder()
    { }

    /**
     * @brief Ignores its input.
     */
    virtual void
    buildCommand(Query&)
    { }
  };


  /**
   * @brief Creates a state command for adding a list of
   * Individuals/Pairs to Concepts/Roles.
   *
   * @see state macro in RacerPro Reference Manual
   */
  class RacerStateBuilder : public RacerBuilder
  {
  protected:
    /**
     * sends an instance command using atom and nspace to build the
     * individual and concept.
     *
     * @param atom
     * @param nspace
     *
     * @see instance macro in RacerPro Reference Manual
     */
    virtual void
    buildPosInstance(const GAtom& atom, const std::string& nspace);

    /**
     * sends an instance command using atom and nspace to build the
     * individual and negated concept.
     *
     * @param atom
     * @param nspace
     *
     * @see instance macro in RacerPro Reference Manual
     */
    virtual void
    buildNegInstance(const GAtom& atom, const std::string& nspace);

    /**
     * sends a related command using atom and nspace to build the
     * individual and role.
     *
     * @param atom
     * @param nspace
     *
     * @see related macro in RacerPro Reference Manual
     */
    virtual void
    buildPosRelated(const GAtom& atom, const std::string& nspace);

    /**
     * sends a related command using atom and nspace to build the
     * individual and role.
     *
     * @param atom
     * @param nspace
     *
     * @todo not implemented yet
     * @see related macro in RacerPro Reference Manual
     */
    virtual void
    buildNegRelated(const GAtom& atom, const std::string& nspace);

  public:
    explicit
    RacerStateBuilder(std::ostream&);

    virtual
    ~RacerStateBuilder();

    /**
     * Calls various building methods in order to generate a state command.
     *
     * @param q use the Interpretation of Query to generate a state command 
     */
    virtual void
    buildCommand(Query& q) throw (RacerBuildingError);
  };


  /**
   * @brief Creates an individual-instance? command in order to check
   * whether an individual is a member of a concept.
   *
   * @see individual-instance? macro in RacerPro Reference Manual
   */
  class RacerIsConceptMemberBuilder : public RacerBuilder
  {
  public:
    explicit
    RacerIsConceptMemberBuilder(std::ostream&);

    virtual
    ~RacerIsConceptMemberBuilder();

    /**
     * Generate a individual-instance? command.
     *
     * @param q use Query to get the individual and the concept name.
     */
    virtual void
    buildCommand(Query& q) throw (RacerBuildingError);
  };


  /**
   * @brief Creates an individuals-related? command in order to check
   * whether a pair is a member of a role.
   *
   * @see individuals-related? macro in RacerPro Reference Manual
   */
  class RacerIsRoleMemberBuilder : public RacerBuilder
  {
  public:
    explicit
    RacerIsRoleMemberBuilder(std::ostream&);

    virtual
    ~RacerIsRoleMemberBuilder();

    /**
     * Generate a individuals-related? command.
     *
     * @param q use Query to get the pair and the role name.
     */
    virtual void
    buildCommand(Query& q) throw (RacerBuildingError);
  };


  /**
   * @brief Creates a command to query concept instances.
   *
   * @see concept-instances macro in RacerPro Reference Manual
   */
  class RacerConceptInstancesBuilder : public RacerBuilder
  {
  public:
    explicit
    RacerConceptInstancesBuilder(std::ostream&);

    virtual
    ~RacerConceptInstancesBuilder();

    virtual void
    buildCommand(Query& q) throw (RacerBuildingError);
  };


  /**
   * @brief Creates a command to query role individuals.
   *
   * @see related-individuals macro in RacerPro Reference manual
   */
  class RacerRoleIndividualsBuilder : public RacerBuilder
  {
  public:
    explicit
    RacerRoleIndividualsBuilder(std::ostream&);

    virtual
    ~RacerRoleIndividualsBuilder();

    virtual void
    buildCommand(Query& q) throw (RacerBuildingError);
  };

  /**
   * @brief Creates a command to query role individual fillers.
   *
   * @see individual-fillers macro in RacerPro Reference manual
   */
  class RacerIndividualFillersBuilder : public RacerBuilder
  {
  public:
    explicit
    RacerIndividualFillersBuilder(std::ostream&);

    virtual
    ~RacerIndividualFillersBuilder();

    virtual void
    buildCommand(Query& q) throw (RacerBuildingError);
  };

  /**
   * @brief Opens an OWL from file or URL as KB "DEFAULT".
   *
   * @see owl-read-file and owl-read-document functions in RacerPro
   * Reference manual.
   */
  class RacerOpenOWLBuilder : public RacerBuilder
  {
  public:
    explicit
    RacerOpenOWLBuilder(std::ostream&);

    virtual
    ~RacerOpenOWLBuilder();

    virtual void
    buildCommand(Query& q) throw (RacerBuildingError);
  };


  /**
   * @brief Creates simple commands.
   */
  class RacerSimpleCommandBuilder : public RacerBuilder
  {
  private:
    /// command used to send it to the stream
    std::string command;

  public:
    RacerSimpleCommandBuilder(std::ostream& s,
			      const std::string& cmd)
      : RacerBuilder(s),
	command(cmd)
    { }

    virtual
    ~RacerSimpleCommandBuilder()
    { }

    /**
     * Just send a simple command to the stream.
     */
    virtual void
    buildCommand(Query&)
    {
      stream << command << std::endl;
    }
  };


  /**
   * @brief Requests listing of all individuals.
   *
   * @see all-individuals function in RacerPro Reference manual.
   */
  class RacerAllIndividualsBuilder : public RacerSimpleCommandBuilder
  {
  public:
    explicit
    RacerAllIndividualsBuilder(std::ostream& s)
      : RacerSimpleCommandBuilder(s, "(all-individuals)")
    { }
  };


  /**
   * @brief Sets the Unique Name Assumption.
   *
   * @see set-unique-name-assumption function in RacerPro Reference manual. 
   */
  class RacerUNABuilder : public RacerSimpleCommandBuilder
  {
  public:
    explicit
    RacerUNABuilder(std::ostream& s)
      : RacerSimpleCommandBuilder(s, "(set-unique-name-assumption t)")
    { }
  };


  /**
   * @brief Clone the "DEFAULT" ABox into "temp-abox".
   *
   * @see clone-abox macro in RacerPro Reference manual.
   */
  class RacerTempABoxBuilder : public RacerSimpleCommandBuilder
  {
  public:
    explicit
    RacerTempABoxBuilder(std::ostream& s)
      : RacerSimpleCommandBuilder(s, "(clone-abox DEFAULT :new-name temp-abox :overwrite t)")
    { }
  };

  /**
   * @brief Checks ABox consistency.
   *
   * @see abox-consistent? macro in RacerPro Reference manual.
   */
  class RacerABoxConsistentBuilder : public RacerSimpleCommandBuilder
  {
  public:
    explicit
    RacerABoxConsistentBuilder(std::ostream& s)
      : RacerSimpleCommandBuilder(s, "(abox-consistent?)")
    { }
  };

  /**
   * @brief Don't check ABox consistency.
   */
  class RacerCheckABoxConsistencyOffBuilder : public RacerSimpleCommandBuilder
  {
  public:
    explicit
    RacerCheckABoxConsistencyOffBuilder(std::ostream& s)
      : RacerSimpleCommandBuilder(s, "(dont-check-abox-consistency-before-querying)")
    { }
  };


} // namespace racer
} // namespace dlvhex

#endif /* _RACERBUILDER_H */
