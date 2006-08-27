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

#include <dlvhex/Term.h>

#include <iosfwd>
#include <string>
#include <set>

#include <boost/shared_ptr.hpp>

namespace dlvhex {
namespace dl {

  /**
   * @brief Basic information about ontologies.
   *
   * This factory stores all available ontologies and provides fast
   * access to them, i.e. it caches concepts/roles/individuals and
   * downloads non-local OWL ontologies.
   */
  class Ontology
  {
  public:
    typedef std::set<Term> Objects;
    typedef boost::shared_ptr<Objects> ObjectsPtr;
    typedef boost::shared_ptr<Ontology> shared_pointer;

  private:
    /// Ontology URI
    std::string uri;
    /// the real ontology URI
    const std::string realuri;

    /// default namespace of OWL document
    std::string nspace;

    //
    // the next members must be mutable, otw. we would not be able to
    // declare the getter methods with a const qualifier
    //

    /// concept names
    mutable ObjectsPtr concepts;
    /// role names
    mutable ObjectsPtr roles;
    /// individual names
    mutable ObjectsPtr individuals;

    //
    // we don't want Ontology to be constructed by the user, so keep
    // those ctors private such that we can only create Ontology by
    // calling Ontology::createOntology
    //

    /// private std::string ctor
    explicit
    Ontology(const std::string& uri, const std::string& tempuri);

    /// private copy ctor
    Ontology(const Ontology&);

    /// @return true if #uri is a local file, false otw.
    bool
    isLocal() const;

  public:
    virtual
    ~Ontology();

    /// factory method
    static Ontology::shared_pointer
    createOntology(const std::string& uri);

    const std::string&
    getURI() const;

    const std::string&
    getRealURI() const;
    
    const std::string&
    getNamespace() const;

    ObjectsPtr
    getConcepts() const;
    
    ObjectsPtr
    getRoles() const;

    ObjectsPtr
    getIndividuals() const;

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
