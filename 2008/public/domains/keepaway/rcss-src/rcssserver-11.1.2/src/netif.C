/* -*- Mode: C++ -*-
 *Header:
 *File: netif.C (for C++ & cc)
 *Author: Noda Itsuki
 *Date: 1996/02/14
 *EndHeader:
 */

/*
 *Copyright:

 Copyright (C) 1996-2000 Electrotechnical Laboratory.
 Itsuki Noda, Yasuo Kuniyoshi and Hitoshi Matsubara.
 Copyright (C) 2000, 2001 RoboCup Soccer Server Maintainance Group.
 Patrick Riley, Tom Howard, Daniel Polani, Itsuki Noda,
 Mikhail Prokopenko, Jan Wendler

 This file is a part of SoccerServer.

 This code is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *EndCopyright:
 */


// [2000/01/14:I.Noda]
//  escape prototype of recvfrom
//  for escape conversion of signed-unsigned ints
//#define recvfrom _escape_recvfrom
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rcssbase/net/udpsocket.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

//#if defined(HAVE_SYS_FILIO_H)
//#include <sys/filio.h>
//#elif defined(HAVE_SYS_IOCTL_H)
//#include <sys/ioctl.h>
//#endif

//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>
#include <cerrno>
#include <cmath>
//#include <fcntl.h>

#ifdef __APPLE__
extern "C" {
    int isnan (double);
}
#endif

#include <iostream>
#ifdef HAVE_SSTREAM
#include <sstream>
#else
#include <strstream>
#endif

#include "param.h"
#include "types.h"
#include "utility.h"
#include "object.h"
#include "coach.h"
#include "player.h"
#include "field.h"

#include "random.h"
#include "serializer.h"
#include "clangparser.h"
#include "clangmsg.h"
//#include "coach_lang_comp.h"
#include "clangmsgbuilder.h"
#include "xpmholder.h"
#include "remoteclient.h"
#include "monitor.h"


/*
 *===================================================================
 *Part: Recv
 *===================================================================
 */

template < class T >
void
Stadium::recv( std::vector< T >& clients )
{
    std::random_shuffle( clients.begin(), clients.end(),
                         irand ); //rcss::random::UniformRNG::instance() );

    for( typename std::vector< T >::iterator i = clients.begin();
         i != clients.end(); )
    {
        if ( (*i)->recv() == -1 )
            ++i;
    }
}


void
Stadium::udp_recv_message()
{
    recv( M_remote_players );
    recv( M_monitors );

    //for ( int iMsgLength = MaxMesg; iMsgLength >= 0; )
    int iMsgLength = MaxMesg;
    while ( iMsgLength >= 0 )
    {
        char message[MaxMesg];
        std::memset( &message, 0, sizeof( char ) * MaxMesg ) ;

        rcss::net::Addr cli_addr;

        iMsgLength = M_player_socket.recv( message, MaxMesg, cli_addr );

        if ( iMsgLength > 0 )
        {
            //              std::cerr << "Got: ";
            //              std::cerr.write( message, iMsgLength );
            //              std::cerr << std::endl;

            bool found = false;
            for ( PlayerCont::iterator i = M_remote_players.begin();
                  i != M_remote_players.end ();
                  ++i )
            {
                if ( (*i)->getDest() == cli_addr )
                {
                    (*i)->undedicatedRecv( message, iMsgLength );
                    found = true;
                    break;
                }
            }

            if ( ! found )
            {
                for ( MonitorCont::iterator i = M_monitors.begin ();
                      i != M_monitors.end ();
                      ++i )
                {
                    if ( (*i)->getDest () == cli_addr )
                    {
                        (*i)->undedicatedRecv( message, iMsgLength );
                        found = true;
                        break;
                    }
                }
            }

            if ( ! found )
            {
                // a new monitor or a new player
                //                unsigned long host = ntohl ( cli_addr.sin_addr.s_addr );
                //                int port = ntohs ( cli_addr.sin_port );

                /* chop newline */
                if ( message[iMsgLength - 1] == '\n' )
                {
                    iMsgLength--;
                }
                message[iMsgLength] = NULLCHAR ;

                if ( ! parseMonitorInit( message, cli_addr ) )
                {
                    parsePlayerInit( message, cli_addr );
                }
            }
        }
        else if( errno != EWOULDBLOCK )
        {
            std::cerr << __FILE__ << ": " << __LINE__
                      << ": Error recv'ing from socket: "
                      << std::strerror( errno ) << std::endl;
        }
    }
}



void
Stadium::parsePlayerInit( const char *message,
                          const rcss::net::Addr& cli_addr )
{
		Player *p;

		if ( ! std::strncmp( message, "(reconnect ", std::strlen( "(reconnect " ) ) )
		{
        //              std::cerr << "Got reconnect" << std::endl;
				// a player reconnects to the server
				if ( playmode() == PM_PlayOn )
        {
            sendToPlayer( "(error cannot_reconnect_while_playon)", cli_addr );
        }
				else
				{
            p = reconnectPlayer( cli_addr, message );
            if ( p == (Player *)NULL )
            {
                //sendToPlayer( "(error reconnect)", cli_addr );
            }
            else
            {
                //                          std::cerr << "Sending info" << std::endl;
                //                              send_reconnect_info(*p) ;
                p->sendReconnect();
                p->sendServerParams();
                p->sendPlayerParams();
                p->sendPlayerTypes();
                p->sendChangedPlayers();
                //                          std::cerr << "Info Sent" << std::endl;
                writeTextLog( *p, message, RECV );
            }
				}
		}
		else if ( ! std::strncmp( message, "(init ", std::strlen( "(init " ) ) )
    {
        // a new player connects to the server
        p = newPlayer( cli_addr, message );
        if ( ! p )
        {
            //sendToPlayer( "(error illegal_teamname_or_no_more_team_or_player_or_goalie)",
            //              cli_addr );
        }
        else
        {
            p->sendInit();
            p->sendServerParams();
            p->sendPlayerParams();
            p->sendPlayerTypes();
            p->sendChangedPlayers();
            writeTextLog( *p, message, RECV );
        }
    }
    else
    {
        sendToPlayer( "(error unknown_command)", cli_addr );
    }
}


bool
Stadium::parseMonitorInit( const char * message,
                           const rcss::net::Addr & addr )
{
    float ver = 1.0;
    if ( ! std::strcmp( message, "(dispinit)" ) )
    {
        Monitor * mon = new Monitor( *this, 1 );
        if( ! mon->connect( addr ) )
        {
            delete mon;
            return false;
        }
        std::cout << "a new (v1) monitor connected\n";
        mon->setEnforceDedicatedPort( false );
        M_monitors.push_back( mon );
        return true;
    }
    else if ( std::sscanf( message, "(dispinit version %f)", &ver ) == 1 )
    {
        // a new monitor connected
        Monitor * mon = new Monitor( *this, ver );

        if( ! mon->connect( addr ) )
        {
            delete mon;
            return false;
        }
        std::cout << "a new (v" << ver << ") monitor connected\n";
        mon->setEnforceDedicatedPort( ver >= 3.0 );
        M_monitors.push_back( mon );
        // send server parameter information to monitor
        if ( ver >= 2.0 )
        {
            dispinfo_t2 di;

            di.mode = htons( PARAM_MODE );
            di.body.sparams = ServerParam::instance().convertToStruct ();
            mon->RemoteClient::send( reinterpret_cast< const char* >( &di ),
                                     sizeof( dispinfo_t2 ) );

            di.mode = htons( PPARAM_MODE );
            di.body.pparams = PlayerParam::instance().convertToStruct ();
            mon->RemoteClient::send( reinterpret_cast< const  char* >( &di ),
                                     sizeof( dispinfo_t2 ) );

            di.mode = htons ( PT_MODE );
            for ( int i = 0; i < PlayerParam::instance().playerTypes(); ++i )
            {
                try
                {
                    di.body.ptinfo = M_player_types.at( i )->convertToStruct( i );
                    mon->RemoteClient::send( reinterpret_cast< const char* >( &di ),
                                             sizeof( dispinfo_t2 ) );
                }
                catch ( std::exception & e )
                {
                    std::cerr << __FILE__ << ':' << __LINE__
                              << " Exception caught! " << e.what() << std::endl;
                }
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

void
Stadium::udp_recv_from_coach()
{
    recv( M_remote_offline_coaches );

    //for ( int iMsgLength = MaxMesg; iMsgLength >= 0; )
    int iMsgLength = MaxMesg;
    while ( iMsgLength >= 0 )
    {
        char message[MaxMesg];
        std::memset( &message, 0, sizeof ( char ) * MaxMesg );

        rcss::net::Addr cli_addr;

        iMsgLength = M_offline_coach_socket.recv( message, MaxMesg,
                                                  cli_addr );

        if ( iMsgLength > 0 )
        {
            bool found = false;
            for ( OfflineCoachCont::iterator i = M_remote_offline_coaches.begin();
                  i != M_remote_offline_coaches.end ();
                  ++i )
            {
                if ( (*i)->getDest () == cli_addr )
                {
                    (*i)->undedicatedRecv ( message, iMsgLength );
                    found = true;
                    break;
                }
            }
            if ( ! found )
            {
                // a new coach

                /* chop newline */
                if ( message[iMsgLength - 1] == '\n' )
                {
                    iMsgLength--;
                }
                message[iMsgLength] = NULLCHAR ;

                int n = M_coach->parse_init( *this, message, cli_addr );
                if ( n == -1 )
                {
                    _Start( *this ); // need to remove this line if we
                    // dont want the server to start when the coach connects
                    M_coach->parse_command( message );
                    writeTextLog( *M_coach, message, RECV );
                }
                else if ( n == 1 )
                {
                    writeTextLog( *M_coach, message, RECV );
                }
            }
        }
        else if ( errno != EWOULDBLOCK )
        {
            std::cerr << __FILE__ << ": " << __LINE__
                      << ": Error recv'ing from socket: "
                      << strerror( errno ) << std::endl;
        }
    }
}

void
Stadium::udp_recv_from_online_coach()
{
    recv( M_remote_online_coaches );

    //for ( int iMsgLength = MaxMesg; iMsgLength >= 0; )
    int iMsgLength = MaxMesg;
    while ( iMsgLength >= 0 )
    {
        char message[MaxMesg];
        std::memset( &message, 0, sizeof ( char ) * MaxMesg );

        rcss::net::Addr cli_addr;

        iMsgLength = M_online_coach_socket.recv( message, MaxMesg,
                                                 cli_addr );

        if ( iMsgLength > 0 )
        {
            bool found = false;
            for ( OnlineCoachCont::iterator i = M_remote_online_coaches.begin ();
                  i != M_remote_online_coaches.end ();
                  ++i )
            {
                if ( (*i)->getDest() == cli_addr )
                {
                    (*i)->undedicatedRecv( message, iMsgLength );
                    found = true;
                    break;
                }
            }
            if ( ! found )
            {
                // a new online coach

                /* chop newline */
                if ( message[iMsgLength - 1] == '\n' )
                {
                    iMsgLength--;
                }
                message[iMsgLength] = NULLCHAR ;

                parseOnlineCoachInit( message, cli_addr );
            }
        }
        else if ( errno != EWOULDBLOCK )
        {
            std::cerr << __FILE__ << ": " << __LINE__
                      << ": Error recv'ing from socket: "
                      << strerror( errno ) << std::endl;
        }
    }
}

void
Stadium::parseOnlineCoachInit( const char* message,
                               const rcss::net::Addr & addr )
{
    //char tmp[MaxMesg];

    //std::sscanf( message, "(%s", tmp );
    if ( ! std::strncmp( message, "(init ", std::strlen( "(init " ) ) )
    {
        OnlineCoach * olc = newCoach( addr, message );
        if ( olc == NULL)
        {
            //sendToOnlineCoach( "(error no_such_team_or_already_have_coach)",
            //                   addr );
        }
        else
        {
            writeTextLog( *olc, message, RECV );
        }
    }
    else
    {
        sendToOnlineCoach( "(error illegal_command_form)",
                           addr );
        //char errmsg[] = "(error illegal_command_form)";
        //M_online_coach_socket.send( errmsg, std::strlen( errmsg ) + 1,
        //                            addr );
    }
}

void
Stadium::say( const char *message, bool ref )
{
    if ( ref )
    {
        sendRefAudio( message );
    }
    else
    {
        sendCoachAudio( *M_coach, message );
    }

    //    send_audio_info(message, NULL, FALSE, standard) ;

    if ( text_log_open()
         || game_log_open()
         || ServerParam::instance().sendComms () )
	  {
        M_minfo.body.msg.board = htons( MSG_BOARD );
        std::snprintf( M_minfo.body.msg.message,
                       sizeof( M_minfo.body.msg.message ),
                       "(%s %s)\n",
                       REFEREE_NAME, message );
        // write to text log
        if ( text_log_open() )
	      {
            text_log_stream() << time() << "\t" << M_minfo.body.msg.message;
	      }

        // send to monitors
        if ( ServerParam::instance().sendComms() )
        {
            for ( MonitorCont::iterator i = M_monitors.begin();
                  i != M_monitors.end();
                  ++i )
            {
                if ( (*i)->getVersion() == 1.0 )
                {
                    (*i)->RemoteClient::send( reinterpret_cast< const char * >( &M_minfo ),
                                              sizeof( dispinfo_t ) );
                }
                else
                {
                    dispinfo_t2 minfo2;
                    minfo2.mode = M_minfo.mode;
                    minfo2.body.msg = M_minfo.body.msg;
                    (*i)->RemoteClient::send( reinterpret_cast< const char * >( &minfo2 ),
                                              sizeof( dispinfo_t2 ) );
                }
            }
        }

        // write to game log
        if ( game_log_open() )
        {
            if ( playmode() != PM_BeforeKickOff
                 && playmode() != PM_TimeOver )
            {
                writeGameLog( &M_minfo );
            }
        }
	  }
}


void
Stadium::sendToPlayer( const char *msg, const rcss::net::Addr & cli_addr )
{
    if ( M_player_socket.send( msg, std::strlen( msg ) + 1, cli_addr ) == -1 )
    {
        std::cerr << __FILE__ ": " << __LINE__
                  << ": Error sending to socket: "
                  << strerror( errno ) << std::endl;
    }
}

void
Stadium::sendToOnlineCoach( const char *msg, const rcss::net::Addr & cli_addr )
{
    if ( M_online_coach_socket.send( msg, std::strlen( msg ) + 1, cli_addr ) == -1 )
    {
        std::cerr << __FILE__ ": " << __LINE__
                  << ": Error sending to socket: "
                  << strerror( errno ) << std::endl;
    }
}


void
Stadium::broadcastSubstitution( const int side,
                                const int unum,
                                const int player_type )
{
    char allies[64];
    char enemies[64];
    std::snprintf( allies, 64, "(change_player_type %d %d)", unum, player_type );
    std::snprintf( enemies, 64, "(change_player_type %d)", unum );

    // tell players
    for ( int i = 0 ; i < MAX_PLAYER * 2; ++i )
    {
        if ( M_players[i]->alive == DISABLE || M_players[i]->version() < 7.0 )
            continue ;
        if ( ( side == LEFT && M_players[i]->team() == M_team_l )
             || ( side == RIGHT && M_players[i]->team() == M_team_r ) )
        {
            M_players[i]->send( allies );
        }
        else
        {
            M_players[i]->send( enemies );
        }
    }

    // tell coaches
    if ( M_olcoaches[1]
         && M_olcoaches[1]->assignedp()
         && M_olcoaches[1]->version() >= 7.0 )
    {
        if ( side == RIGHT )
        {
            //team_r->olcoach()->send( allies );
            M_olcoaches[1]->send( allies );
        }
        if ( side == LEFT )
        {
            //team_r->olcoach()->send( enemies );
            M_olcoaches[1]->send( enemies );
        }
    }

    if ( M_olcoaches[0]
         && M_olcoaches[0]->assignedp()
         && M_olcoaches[0]->version() >= 7.0 )
    {
        if ( side == LEFT )
        {
            //team_l->olcoach()->send( allies );
            M_olcoaches[0]->send( allies );
        }
        if ( side == RIGHT )
        {
            //team_l->olcoach()->send( enemies );
            M_olcoaches[0]->send( enemies );
        }
    }

    char buffer[64];
    std::snprintf( buffer, 64,
                   "(change_player_type %s %d %d)",
                   ( side == LEFT ? "l" : "r" ),
                   unum, player_type );
    for ( MonitorCont::iterator i = M_monitors.begin ();
          i != M_monitors.end (); ++i )
    {
        (*i)->send( buffer );
    }

    std::strcat( buffer, "\n" );
    writeTextLog( buffer, SUBS );
}
