// -*-c++-*-

/***************************************************************************
                                    rule.h  
                           Classes for clang rules
                             -------------------
    begin                : 25-APR-2002
    copyright            : (C) 2002 by The RoboCup Soccer Server 
                           Maintenance Group.
    email                : sserver-admin@lists.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU LGPL as published by the Free Software  *
 *   Foundation; either version 2 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/

#ifndef _RULE_H_
#define _RULE_H_

#include <iostream>
#include <string>
#include <list>
#include "hasa.h"


namespace rcss
{
  namespace clang
  {
    typedef std::string RuleID;

    // If the rule list is empty then it counts a containing all
    // rules.  This also means that there is no support for an empty
    // rule ID list, but that is just the way we want it.
    class RuleIDList
      : public std::list< RuleID >
    {
    private:
      bool
      empty() const;
    public:
      bool
      all() const;

      bool
      unary() const;

      std::ostream&
      print( std::ostream& o ) const;

      std::ostream&
      printPretty( std::ostream& out, const std::string& line_header ) const;
    };

    class ActivateRules
    {
    public:

      ActivateRules();

      ActivateRules( const bool& on, const RuleIDList& list );

      ~ActivateRules();

      bool
      on() const;

      bool
      all() const;

      bool
      unary() const;

      const RuleIDList&
      getList() const;

      void
      set( const bool& on );

      void
      set( const RuleIDList& rids );

      std::ostream&
      print( std::ostream& o ) const;

      std::ostream&
      printPretty( std::ostream& o, const std::string& line_header ) const;

    private:
	bool m_on;
	RuleIDList m_rids;
    };

    class Cond;
    class Dir;

    class Rule
    {
    public:
      virtual
      ~Rule();

      virtual
      std::auto_ptr< Rule >
      deepCopy() const = 0;

      virtual
      std::ostream& 
      print( std::ostream& out ) const = 0;
      
      virtual
      std::ostream&
      printPretty( std::ostream& out,
                   const std::string& line_header ) const = 0;
    };

    class CondRule
      : public Rule
    {
    public:

	CondRule( std::auto_ptr< Cond > cond );
	
	virtual
	~CondRule();
	
	const Cond*
	getCond() const;
	
	std::auto_ptr< Cond >
	detachCond();
	
    private:
	std::auto_ptr< Cond > m_cond;
   };

    
    class SimpleRule
      : public CondRule
    {
    public:
      typedef std::list< Dir* > Storage;

      SimpleRule( std::auto_ptr< Cond > cond );
      
      SimpleRule( std::auto_ptr< Cond > cond,
                  const Storage& dirs );

      virtual
      ~SimpleRule();

      virtual
      std::ostream& 
      print( std::ostream& out ) const;
      
      virtual
      std::ostream&
      printPretty( std::ostream& out, const std::string& line_header ) const;

      virtual
      std::auto_ptr< Rule >
      deepCopy() const;

      const Storage&
      getDirs() const;

    private:
	Storage m_dirs;
    };

    class NestedRule
      : public CondRule
    {
    public:
      typedef std::list< Rule* > Storage;

      NestedRule( std::auto_ptr< Cond > cond );
      
      NestedRule( std::auto_ptr< Cond > cond,
                  const Storage& rules );

      virtual
      ~NestedRule();

      virtual
      std::ostream& 
      print( std::ostream& out ) const;
      
      virtual
      std::ostream&
      printPretty( std::ostream& out, const std::string& line_header ) const;

      virtual
      std::auto_ptr< Rule >
      deepCopy() const;
      
      const Storage&
      getRules() const;

    private:
	Storage m_rules;
    };

    class IDListRule
      : public Rule
    {
    public:

      IDListRule();
      
      IDListRule( const RuleIDList& rules );

      virtual
      ~IDListRule();

      virtual
      std::ostream& 
      print( std::ostream& out ) const;
      
      virtual
      std::ostream&
      printPretty( std::ostream& out, const std::string& line_header ) const;

      virtual
      std::auto_ptr< Rule >
      deepCopy() const;
      
      const RuleIDList&
      getIDList() const;
	
    private:
	RuleIDList m_rids;
    };


std::ostream&
operator<<( std::ostream& o, const rcss::clang::RuleIDList& id_list );

std::ostream&
operator<<( std::ostream& o, const rcss::clang::ActivateRules& activ_rules );

std::ostream&
operator<<( std::ostream & os, const rcss::clang::Rule& r );

  }
}
 
#endif
