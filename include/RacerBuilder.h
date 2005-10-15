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
    std::ostream& stream;

    /// Ctor
    explicit
    RacerBuilder(std::ostream&);

    /// Dtor
    virtual
    ~RacerBuilder();

  public:
    /**
     * Building method implemented by the children of RacerBuilder.
     *
     * @param q
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
    /// Ctor
    explicit
    RacerNullBuilder(std::ostream& s)
      : RacerBuilder(s)
    { }

    /// Dtor
    virtual
    ~RacerNullBuilder()
    { }

    /**
     * @brief Ignores input.
     */
    virtual void
    buildCommand(Query&)
    { }
  };


  /**
   * @brief Creates command for adding/removing an Individual/Pair to
   * a Concept/Role.
   */
  class RacerStateBuilder : public RacerBuilder
  {
  protected:
    virtual void
    buildPosInstance(const GAtom&, const std::string&);

    virtual void
    buildNegInstance(const GAtom&, const std::string&);

    virtual void
    buildPosRelated(const GAtom&, const std::string&);

//     virtual void
//     buildNegRelated(const GAtom&, const std::string&);

  public:
    explicit
    RacerStateBuilder(std::ostream&);

    virtual
    ~RacerStateBuilder();

    virtual void
    buildCommand(Query& q) throw (RacerBuildingError);
  };


  /**
   * @brief concept-instance? builder.
   */
  class RacerIsConceptMemberBuilder : public RacerBuilder
  {
  public:
    explicit
    RacerIsConceptMemberBuilder(std::ostream&);

    virtual
    ~RacerIsConceptMemberBuilder();

    virtual void
    buildCommand(Query& q) throw (RacerBuildingError);
  };


  /**
   * @brief related-individuals? builder.
   */
  class RacerIsRoleMemberBuilder : public RacerBuilder
  {
  public:
    explicit
    RacerIsRoleMemberBuilder(std::ostream&);

    virtual
    ~RacerIsRoleMemberBuilder();

    virtual void
    buildCommand(Query& q) throw (RacerBuildingError);
  };


  /**
   * @brief Creates a command to query concept instances.
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
   * @brief Opens an OWL from file or URL as KB "DEFAULT".
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
     * Just send the command to the stream.
     */
    virtual void
    buildCommand(Query&)
    {
      stream << command << std::endl;
    }
  };


  /**
   * @brief Requests listing of all individuals.
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
   */
  class RacerTempABoxBuilder : public RacerSimpleCommandBuilder
  {
  public:
    explicit
    RacerTempABoxBuilder(std::ostream& s)
      : RacerSimpleCommandBuilder(s, "(clone-abox DEFAULT :new-name temp-abox :overwrite t)")
    { }
  };

} // namespace racer
} // namespace dlvhex

#endif /* _RACERBUILDER_H */
