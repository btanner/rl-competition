// -*-c++-*-

/***************************************************************************
                                clangdelmsg.cc  
                       Class for CLang Delete messages
                             -------------------
    begin                : 28-MAY-2002
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

#include "clangdelmsg.h"
#include "rule.h"
#include "types.h"

namespace rcss
{
  namespace clang
  {
    DelMsg::DelMsg()
      : Msg()
    {}
    
    DelMsg::DelMsg( const RuleIDList& list )
      : Msg(),
        m_rids( list )
    {}
    
    DelMsg::~DelMsg()
    {}
    
    std::auto_ptr< Msg >
    DelMsg::deepCopy() const
    { return std::auto_ptr< Msg >( new DelMsg( *this ) ); }
    
//     void
//     DelMsg::accept( Visitor& v )
//     { v.startVisit( this ); }
    
//     void
//     DelMsg::accept( ConstVisitor& v ) const
//     { v.startVisit( this ); }
    
    std::ostream&
    DelMsg::print( std::ostream& out ) const
    {
      out << "(delete " << m_rids << ")"; 
      return out;
    }
    
    std::ostream&
    DelMsg::printPretty( std::ostream& out,
                         const std::string& line_header ) const
    { 
      out << line_header << "Delete" << std::endl;
      return m_rids.printPretty( out, line_header + " -" );
    }
    
    const RuleIDList&
    DelMsg::getList() const
    { return m_rids; }      
  }
}
