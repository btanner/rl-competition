/* -*- Mode: C++ -*- */

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "hetroplayer.h"

#include <boost/random.hpp>

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h> /* needed for htonl, htons, ... */
#endif
#ifdef HAVE_WINSOCK2_H
#include <Winsock2.h> /* needed for htonl, htons, ... */
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

// bool HetroPlayer::seeded = false;
// boost::mt19937 HetroPlayer::S_rng;;

// void HetroPlayer::seed()
// {
//     if ( ! HetroPlayer::seeded )
//     {
//         if ( PlayerParam::instance().randomSeed() >= 0 )
//         {
//             std::cout << "Using given Hetero Player Seed: "
//                       << PlayerParam::instance().randomSeed() << std::endl;
//             //srandom( PlayerParam::instance().randomSeed() );
//             S_rng.seed( PlayerParam::instance().randomSeed() );
//         }
//         else
//         {
//             timeval now;
//             gettimeofday ( &now, NULL );
//             std::cout << "Hetero Player Seed: " << now.tv_usec << std::endl;
//             //srandom( now.tv_usec );
//             S_rng.seed( now.tv_usec );
//         }
//         HetroPlayer::seeded = true;
//     }
// }

Value
HetroPlayer::delta( const Value & min,
                    const Value & max )
{
    static bool s_seeded = false;
    static boost::mt19937 s_engine;

    if ( ! s_seeded )
    {
        if ( PlayerParam::instance().randomSeed() >= 0 )
        {
            std::cout << "Using given Hetero Player Seed: "
                      << PlayerParam::instance().randomSeed() << std::endl;
            //srandom( PlayerParam::instance().randomSeed() );
            s_engine.seed( PlayerParam::instance().randomSeed() );
        }
        else
        {
            timeval now;
            gettimeofday ( &now, NULL );
            std::cout << "Hetero Player Seed: " << now.tv_usec << std::endl;
            //srandom( now.tv_usec );
            s_engine.seed( static_cast< boost::mt19937::result_type >( now.tv_usec ) );
        }
        s_seeded = true;
    }

    //return drand( min, max );
    if ( min == max )
    {
        return min;
    }

    Value minv = min;
    Value maxv = max;

    if ( minv > maxv )
    {
      std::swap( minv, maxv );
      //return boost::uniform_real< Value >( max, min )( s_engine );
    }
    //return boost::uniform_real< Value >( min, max )( s_engine );
    boost::uniform_real< Value > rng( minv, maxv );
    boost::variate_generator< boost::mt19937&, boost::uniform_real<> > gen( s_engine, rng );
    return gen();
}

HetroPlayer::HetroPlayer()
{
    //seed();

    bool reject;
    do
    {
        Value tmp_delta = HetroPlayer::delta ( PlayerParam::instance().playerSpeedMaxDeltaMin (),
                                               PlayerParam::instance().playerSpeedMaxDeltaMax () );
        player_speed_max = ServerParam::instance().pspeed_max + tmp_delta;
        stamina_inc_max = ServerParam::instance().stamina_inc + tmp_delta
            * PlayerParam::instance().staminaIncMaxDeltaFactor ();

        tmp_delta = HetroPlayer::delta ( PlayerParam::instance().playerDecayDeltaMin (),
                                         PlayerParam::instance().playerDecayDeltaMax () );
        player_decay = ServerParam::instance().pdecay + tmp_delta;
        inertia_moment = ServerParam::instance().inertia_moment + tmp_delta
            * PlayerParam::instance().inertiaMomentDeltaFactor ();

        tmp_delta = HetroPlayer::delta ( PlayerParam::instance().dashPowerRateDeltaMin (),
                                         PlayerParam::instance().dashPowerRateDeltaMax () );
        dash_power_rate = ServerParam::instance().dprate + tmp_delta;
        player_size = ServerParam::instance().psize + tmp_delta
            * PlayerParam::instance().playerSizeDeltaFactor ();

        // trade-off stamina_inc_max with dash_power_rate
        tmp_delta = HetroPlayer::delta(PlayerParam::instance().newDashPowerRateDeltaMin(),
                                       PlayerParam::instance().newDashPowerRateDeltaMax());
        dash_power_rate = ServerParam::instance().dprate + tmp_delta;
        stamina_inc_max = ServerParam::instance().stamina_inc + tmp_delta *
            PlayerParam::instance().newStaminaIncMaxDeltaFactor();

        tmp_delta = HetroPlayer::delta ( PlayerParam::instance().kickableMarginDeltaMin (),
                                         PlayerParam::instance().kickableMarginDeltaMax () );
        kickable_margin = ServerParam::instance().kmargin + tmp_delta;
        kick_rand = ServerParam::instance().kick_rand + tmp_delta
            * PlayerParam::instance().kickRandDeltaFactor ();

        tmp_delta = HetroPlayer::delta ( PlayerParam::instance().extraStaminaDeltaMin (),
                                         PlayerParam::instance().extraStaminaDeltaMax () );
        extra_stamina = tmp_delta;
        effort_max = ServerParam::instance().effort_init + tmp_delta
            * PlayerParam::instance().effortMaxDeltaFactor ();
        effort_min = ServerParam::instance().effort_min + tmp_delta
            * PlayerParam::instance().effortMinDeltaFactor ();

        // the amount of power consumed by a default player while running at max speed
        static double homo_power_cons_at_max_vel
            = ( ( ServerParam::instance().pspeed_max
                  * ( 1 - ServerParam::instance().pdecay ) )
                / ServerParam::instance().dprate )
            - ServerParam::instance().stamina_inc;

        // the amount of power consumend by this player while running at max speed
        double hetero_power_cons_at_max_vel
            = ( ( player_speed_max
                  * ( 1 - player_decay ) )
                / dash_power_rate )
            - stamina_inc_max;

        // if the generated player can run faster than default and consume less power, then reject this player type and select a new one.
        reject = ( player_speed_max > ServerParam::instance().pspeed_max
                   && hetero_power_cons_at_max_vel < homo_power_cons_at_max_vel );
    }
    while( reject );
}

HetroPlayer::HetroPlayer ( int )
{
    player_speed_max = ServerParam::instance().pspeed_max;
    stamina_inc_max = ServerParam::instance().stamina_inc;

    player_decay = ServerParam::instance().pdecay;
    inertia_moment = ServerParam::instance().inertia_moment;

    dash_power_rate = ServerParam::instance().dprate;
    player_size = ServerParam::instance().psize;

    kickable_margin = ServerParam::instance().kmargin;
    kick_rand = ServerParam::instance().kick_rand;

    extra_stamina = PlayerParam::instance().extraStaminaDeltaMin ();
    effort_max = ServerParam::instance().effort_init;
    effort_min = ServerParam::instance().effort_min;
}

HetroPlayer::~HetroPlayer ()
{
}

std::ostream&
HetroPlayer::print( std::ostream & o ) const
{
    o << "Hetro Player Type:\n";
    o << "\tPlayer speed max = " << playerSpeedMax () << '\n';
    o << "\tStamina inc = " << staminaIncMax () <<  '\n';
    o << "\tPlayer decay = " << playerDecay () << '\n';
    o << "\tInertia moment = " << inertiaMoment () << '\n';
    o << "\tDash power rate = " << dashPowerRate () << '\n';
    o << "\tPlayer size = " << playerSize () << '\n';
    o << "\tKickable margin = " << kickableMargin () << '\n';;
    o << "\tKick rand = " << kickRand () << '\n';
    o << "\tExtra stamina = " << extraStamina () << '\n';
    o << "\tEffort max = " << effortMax () << '\n';
    o << "\tEffort min = " << effortMin () << '\n';
    double power_cons_at_max_vel
        = ( ( playerSpeedMax()
              * ( 1 - playerDecay() ) )
            / dashPowerRate() )
        - staminaIncMax();
    o << "\tPower Cons @ max vel = " << power_cons_at_max_vel << std::endl;
    return o;
}

player_type_t
HetroPlayer::convertToStruct ( int id )
{
    player_type_t tmp;

    tmp.id = htons ( id );
    tmp.player_speed_max = htonl ( (Int32)rint((player_speed_max * SHOWINFO_SCALE2) ));
    tmp.stamina_inc_max = htonl ( (Int32)rint((stamina_inc_max * SHOWINFO_SCALE2) ));
    tmp.player_decay = htonl ( (Int32)rint((player_decay * SHOWINFO_SCALE2) ));
    tmp.inertia_moment = htonl ( (Int32)rint((inertia_moment * SHOWINFO_SCALE2) ));
    tmp.dash_power_rate = htonl ( (Int32)rint((dash_power_rate * SHOWINFO_SCALE2) ));
    tmp.player_size = htonl ( (Int32)rint((player_size * SHOWINFO_SCALE2) ));
    tmp.kickable_margin = htonl ( (Int32)rint((kickable_margin * SHOWINFO_SCALE2) ));
    tmp.kick_rand = htonl ( (Int32)rint((kick_rand * SHOWINFO_SCALE2) ));
    tmp.extra_stamina = htonl ( (Int32)rint((extra_stamina * SHOWINFO_SCALE2) ));
    tmp.effort_max = htonl ( (Int32)rint((effort_max * SHOWINFO_SCALE2) ));
    tmp.effort_min = htonl ( (Int32)rint((effort_min * SHOWINFO_SCALE2) ));

    return tmp;
}
