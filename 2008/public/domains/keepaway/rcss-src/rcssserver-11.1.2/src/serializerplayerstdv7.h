// -*-c++-*-

/***************************************************************************
                            serializerplayerstdv7.h
                  Class for serializing data to std v7 players
                             -------------------
    begin                : 27-MAY-2002
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

#ifndef SERIALIZERPLAYERSTDV7_H
#define SERIALIZERPLAYERSTDV7_H

#include "serializerplayerstdv1.h"

namespace rcss
{


class SerializerPlayerStdv7
    : public SerializerPlayerStdv1
{
protected:
    SerializerPlayerStdv7( const SerializerCommon& common );

public:
    virtual
    ~SerializerPlayerStdv7();

    static
    const SerializerPlayerStdv7*
    instance();

    virtual
    void
    serializeCoachAudio( std::ostream& strm,
                         const int& time,
                         const std::string& name,
                         const char* msg ) const;

    virtual
    void
    serializeCoachStdAudio( std::ostream& strm,
                            const int& time,
                            const std::string& name,
                            const rcss::clang::Msg& msg ) const;

    virtual
    void
    serializeSelfAudio( std::ostream& strm,
                        const int& time,
                        const char* msg ) const;

    virtual
    void
    serializePlayerAudio( std::ostream& strm,
                          const int& time,
                          const double& dir,
                          const char* msg ) const;

    virtual
    void
    serializeServerParamBegin( std::ostream& strm ) const;

    virtual
    void
    serializeServerParamEnd( std::ostream& strm ) const;

    virtual
    void
    serializePlayerParamBegin( std::ostream& strm ) const;

    virtual
    void
    serializePlayerParamEnd( std::ostream& strm ) const;

    virtual
    void
    serializePlayerTypeBegin( std::ostream& strm ) const;

    virtual
    void
    serializePlayerTypeEnd( std::ostream& strm ) const;

    virtual
    void
    serializeParam( std::ostream& strm, int param ) const;

    virtual
    void
    serializeParam( std::ostream& strm, unsigned int param ) const;

    virtual
    void
    serializeParam( std::ostream& strm, bool param ) const;

    virtual
    void
    serializeParam( std::ostream& strm, double param ) const;

    virtual
    void
    serializeParam( std::ostream& strm, const std::string& param ) const;

    virtual
    void
    serializeChangePlayer( std::ostream& strm,
                           int unum ) const;

    virtual
    void
    serializeChangePlayer( std::ostream& strm,
                           int unum,
                           int type ) const;

};
}
#endif
