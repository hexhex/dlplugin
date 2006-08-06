/* -*- C++ -*- */

/**
 * @file   Ontology.h
 * @author Thomas Krennwallner
 * @date   Sat Aug  5 13:50:01 2006
 * 
 * @brief  
 * 
 * 
 */


#ifndef _ONTOLOGY_H
#define _ONTOLOGY_H

#include <dlvhex/Term.h>

#include <string>
#include <set>

#include <boost/shared_ptr.hpp>

namespace dlvhex {
namespace racer {

  /**
   * @brief Describes an ontology.
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
    /// is this a temporary local file?
    bool isTemp;

    /// default namespace of OWL
    std::string nspace;

    // the next members must be mutable, otw. we would not be able to
    // declare the getter methods with a const qualifier

    /// concept names
    mutable ObjectsPtr concepts;
    /// role names
    mutable ObjectsPtr roles;
    /// individual names
    mutable ObjectsPtr individuals;


    Ontology();

    Ontology(const std::string& uri);

    Ontology(const Ontology&);


    bool
    isLocal() const;

  public:
    virtual
    ~Ontology();

    static Ontology::shared_pointer
    createOntology(const std::string& uri);

    const std::string&
    getURI() const;
    
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
    operator== (const Ontology& o1, const Ontology& o2);

    friend bool
    operator!= (const Ontology& o1, const Ontology& o2);
  };

  /**
   * @brief put the string representation of o into os
   * @param os
   * @param o
   * @return os
   */
  std::ostream&
  operator<< (std::ostream& os, const Ontology& o);

  /**
   * @brief lexicographically compare o1 to o2 and check if o1 is less than o2
   * @param o1
   * @param o2
   * @return true if o1 < o2, false otherwise.
   */
  bool
  operator< (const Ontology& o1, const Ontology& o2);

  /**
   * @brief lexicographically compare o1 to o2 and check if both queries are equal
   * @param o1
   * @param o2
   * @return true if o1 equals o2, false otherwise.
   */
  bool
  operator== (const Ontology& o1, const Ontology& o2);

  /**
   * @brief converse of operator==
   * @param o1
   * @param o2
   * @return true if !(o1 == o2), false otherwise.
   */
  bool
  operator!= (const Ontology& o1, const Ontology& o2);


} // namespace racer
} // namespace dlvhex

#endif /* _ONTOLOGY_H */
