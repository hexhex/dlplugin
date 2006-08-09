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

#include "RacerError.h"

#include <iosfwd>
#include <string>

namespace dlvhex {
namespace racer {

  //
  // forward declarations
  //
  class Query;


  /**
   * @brief Base Builder for building RACER commands.
   */
  class RacerBuilder
  {
  protected:
    /// the builders write commands to this stream
    std::ostream& stream;

    /**
     * protected ctor.
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
     * @param q contains the information in order to create a RACER command
     *
     * @return true if command was built and sent to the #stream,
     * false otherwise.
     */
    virtual bool
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

    /**
     * @brief Ignores its input.
     */
    virtual bool
    buildCommand(Query&)
    { return true; }
  };


  /**
   * @brief Creates a state command for adding a list of
   * Individuals/Pairs to Concepts/Roles.
   *
   * @see state macro in RacerPro Reference Manual
   */
  class RacerStateBuilder : public RacerBuilder
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

    /**
     * Generate a individual-instance? command.
     *
     * @param q use Query to get the individual and the concept name.
     */
    virtual bool
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

    /**
     * Generate a individuals-related? command.
     *
     * @param q use Query to get the pair and the role name.
     */
    virtual bool
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

    virtual bool
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

    virtual bool
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

    virtual bool
    buildCommand(Query& q) throw (RacerBuildingError);
  };


  /**
   * @brief Creates a command to query datatype role individual
   * fillers.
   *
   * @see individual-told-datatype-fillers function in RacerPro
   * Reference manual
   */
  class RacerIndividualDatatypeFillersBuilder : public RacerBuilder
  {
  public:
    explicit
    RacerIndividualDatatypeFillersBuilder(std::ostream&);

    virtual bool
    buildCommand(Query& q) throw (RacerBuildingError);
  };


  /**
   * @brief Creates a nRQL retrieve command.
   *
   * @see retrieve function in RacerPro Reference manual
   */
  class RacerNRQLBuilder : public RacerBuilder
  {
  public:
    explicit
    RacerNRQLBuilder(std::ostream&);

    virtual bool
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

    virtual bool
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
    RacerSimpleCommandBuilder(std::ostream& s, const std::string& cmd);

    /**
     * Just send a simple command to the stream.
     */
    virtual bool
    buildCommand(Query&);
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

  /**
   * @brief Enable data substrate mirroring.
   *
   * @see 
   */
  class RacerDataSubstrateMirrorBuilder : public RacerSimpleCommandBuilder
  {
  public:
    explicit
    RacerDataSubstrateMirrorBuilder(std::ostream& s)
      : RacerSimpleCommandBuilder(s, "(enable-data-substrate-mirroring)")
    { }
  };

  /**
   * @brief Enable imports in KB DEFAULT, i.e. read all owl:imports.
   *
   * @see 
   */
  class RacerImportOntologies : public RacerSimpleCommandBuilder
  {
  public:
    explicit
    RacerImportOntologies(std::ostream& s)
      : RacerSimpleCommandBuilder(s, "(kb-ontologies DEFAULT)")
    { }
  };


} // namespace racer
} // namespace dlvhex

#endif /* _RACERBUILDER_H */
