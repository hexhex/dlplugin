/* -*- C++ -*- */

/**
 * @file   Ontology.h
 * @author Thomas Krennwallner
 * @date   Sat Aug  5 13:50:01 2006
 * 
 * @brief  Basic information about ontologies.
 * 
 * 
 */


#ifndef _ONTOLOGY_H
#define _ONTOLOGY_H

#include "URI.h"

#include <dlvhex/Term.h>

#include <iosfwd>
#include <string>
#include <set>

#include <boost/shared_ptr.hpp>

namespace dlvhex {
namespace dl {


  /**
   * @brief Represents the ABox component of an Ontology.
   */
  class ABox
  {
  public:
    typedef std::set<Term> Objects;
    typedef boost::shared_ptr<Objects> ObjectsPtr;

  private:
    /// set of individual names
    ObjectsPtr individuals;

  public:
    /// default ctor
    ABox() : individuals(new Objects) { }

    inline ObjectsPtr
    getIndividuals() const
    {
      return individuals;
    }

    inline void
    addIndividual(const Term& t)
    {
      individuals->insert(t);
    }
  };


  /**
   * @brief Represents the TBox component of an Ontology.
   */
  class TBox
  {
  public:
    typedef std::set<Term> Objects;
    typedef boost::shared_ptr<Objects> ObjectsPtr;

  private:
    /// set of concept names
    ObjectsPtr concepts;
    /// set of role names
    ObjectsPtr roles;
    /// set of datatype role names
    ObjectsPtr datatypeRoles;

  public:
    /// default ctor
    TBox() : concepts(new Objects), roles(new Objects), datatypeRoles(new Objects) { }

    inline ObjectsPtr
    getConcepts() const
    {
      return concepts;
    }

    inline void
    addConcept(const Term& t)
    {
      concepts->insert(t);
    }

    inline ObjectsPtr
    getRoles() const
    {
      return roles;
    }

    inline void
    addRole(const Term& t)
    {
      roles->insert(t);
    }

    inline ObjectsPtr
    getDatatypeRoles() const
    {
      return datatypeRoles;
    }

    inline void
    addDatatypeRole(const Term& t)
    {
      datatypeRoles->insert(t);
    }
  };


  /**
   * @brief Basic information about ontologies.
   *
   * This factory stores all available ontologies and provides fast
   * access to them, i.e. it caches TBox/ABox components and provides
   * download support for non-local OWL ontologies.
   */
  class Ontology
  {
  public:
    typedef boost::shared_ptr<Ontology> shared_pointer;

  private:
    /// Ontology URI
    URI uri;
    /// the real ontology URI
    const URI realuri;

    /// default namespace of OWL document
    std::string nspace;

    //
    // the next members must be mutable, otw. we would not be able to
    // declare the getter methods with a const qualifier
    //

    /// concept/role names
    mutable TBox* tbox;
    /// individual names
    mutable ABox* abox;

    //
    // we don't want Ontology to be constructed by the user, so keep
    // those ctors private such that we can only create Ontology by
    // calling Ontology::createOntology
    //

    /// private std::string ctor
    explicit
    Ontology(const URI& uri, const std::string& tempfile = "");

    /// private copy ctor
    Ontology(const Ontology&);

    /// private assignment op
    Ontology&
    operator= (const Ontology&);

  public:
    virtual
    ~Ontology();

    /// factory method
    static Ontology::shared_pointer
    createOntology(const std::string& uri);

    const URI&
    getURI() const;

    const URI&
    getRealURI() const;
    
    const std::string&
    getNamespace() const;

    const ABox&
    getABox() const;
    
    const TBox&
    getTBox() const;

    friend std::ostream&
    operator<< (std::ostream& os, const Ontology& o);

    friend bool
    operator< (const Ontology& o1, const Ontology& o2);

    friend bool
    operator> (const Ontology& o1, const Ontology& o2);

    friend bool
    operator== (const Ontology& o1, const Ontology& o2);

    friend bool
    operator!= (const Ontology& o1, const Ontology& o2);
  };

  /**
   * @brief put the string representation of @a o into @a os.
   *
   * @param os
   * @param o
   *
   * @return @a os
   */
  inline std::ostream&
  operator<< (std::ostream& os, const Ontology& o)
  {
    return os << o.getRealURI() << '(' << o.getNamespace() << ')';
  }

  /**
   * @brief lexicographically compare @a o1 to @a o2 and check if @a o1 is less than @a o2.
   *
   * @param o1
   * @param o2
   *
   * @return true if @a o1 < @a o2, false otherwise.
   */
  inline bool
  operator< (const Ontology& o1, const Ontology& o2)
  {
    return o1.getRealURI().compare(o2.getRealURI()) < 0;
  }

  /**
   * @brief lexicographically compare @a o1 to @a o2 and check if @a o1 is greater than @a o2.
   *
   * @param o1
   * @param o2
   *
   * @return true if @a o1 > @a o2, false otherwise.
   */
  inline bool
  operator> (const Ontology& o1, const Ontology& o2)
  {
    return o1.getRealURI().compare(o2.getRealURI()) > 0;
  }

  /**
   * @brief lexicographically compare @a o1 to @a o2 and check if both
   * are equal.
   *
   * @param o1
   * @param o2
   *
   * @return true if @a o1 equals @a o2, false otherwise.
   */
  inline bool
  operator== (const Ontology& o1, const Ontology& o2)
  {
    return o1.getRealURI() == o2.getRealURI();
  }

  /**
   * @brief converse of operator==.
   *
   * @param o1
   * @param o2
   *
   * @return true if !(@a o1 == @a o2), false otherwise.
   */
  inline bool
  operator!= (const Ontology& o1, const Ontology& o2)
  {
    return !(o1 == o2);
  }


} // namespace dl
} // namespace dlvhex

#endif /* _ONTOLOGY_H */
