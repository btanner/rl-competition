/* -*-bison-*- */
/*
 *Copyright:

    Copyright (C) 2001 RoboCup Soccer Server Maintainance Group.
        Patrick Riley, Tom Howard, Itsuki Noda, Mikhail Prokopenko, Jan Wendler 

    This file is part of SoccerServer.

    SoccerServer is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *EndCopyright:
 */
%{
#include "clangbuilder.h"
#include "clangparser.h"
#include "serverparam.h"

#define    yymaxdepth RCSS_CLANG_maxdepth
#define    yyparse    RCSS_CLANG_parse
#define    yylex      RCSS_CLANG_lex
#define    yyerror    RCSS_CLANG_error
#define    yylval     RCSS_CLANG_lval
#define    yychar     RCSS_CLANG_char
#define    yydebug    RCSS_CLANG_debug
#define    yypact     RCSS_CLANG_pact
#define    yyr1       RCSS_CLANG_r1
#define    yyr2       RCSS_CLANG_r2
#define    yydef      RCSS_CLANG_def
#define    yychk      RCSS_CLANG_chk
#define    yypgo      RCSS_CLANG_pgo
#define    yyact      RCSS_CLANG_act
#define    yyexca     RCSS_CLANG_exca
#define    yyerrflag  RCSS_CLANG_errflag
#define    yynerrs    RCSS_CLANG_nerrs
#define    yyps       RCSS_CLANG_ps
#define    yypv       RCSS_CLANG_pv
#define    yys        RCSS_CLANG_s
#define    yy_yys     RCSS_CLANG_yys
#define    yystate    RCSS_CLANG_state
#define    yytmp      RCSS_CLANG_tmp
#define    yyv        RCSS_CLANG_v
#define    yy_yyv     RCSS_CLANG_yyv
#define    yyval      RCSS_CLANG_val
#define    yylloc     RCSS_CLANG_lloc
#define    yyreds     RCSS_CLANG_reds
#define    yytoks     RCSS_CLANG_toks
#define    yylhs      RCSS_CLANG_yylhs
#define    yylen      RCSS_CLANG_yylen
#define    yydefred   RCSS_CLANG_yydefred
#define    yydgoto    RCSS_CLANG_yydgoto
#define    yysindex   RCSS_CLANG_yysindex
#define    yyrindex   RCSS_CLANG_yyrindex
#define    yygindex   RCSS_CLANG_yygindex
#define    yytable    RCSS_CLANG_yytable
#define    yycheck    RCSS_CLANG_yycheck
#define    yyname     RCSS_CLANG_yyname
#define    yyrule     RCSS_CLANG_yyrule

#define YYERROR_VERBOSE

void yyerror (const char* s);
int yyerror (char* s);

#define YYPARSE_PARAM param

namespace
{
  rcss::clang::Parser::Param&
  getParam( void* param )
  { 
    static rcss::clang::Parser::Param* cached_param 
                   = reinterpret_cast< rcss::clang::Parser::Param* >( param );
    if( cached_param != param )
      cached_param = reinterpret_cast< rcss::clang::Parser::Param* >( param );
    return *cached_param;
  }

  inline
  rcss::clang::Builder&
  getBuilder( void* param )
  { return getParam( param ).getBuilder(); }

#define YYSTYPE rcss::clang::Parser::Lexer::Holder
  
  inline
  int
  yylex( YYSTYPE* holder, rcss::clang::Parser::Param& param )
  {
    int rval =  param.getLexer().lex( *holder );
//    cout << rval << endl;
    return rval;
  }

}

#define YYLEX_PARAM getParam( param )

#define BUILDER getBuilder( param )

%}
     
/* BISON DECLARATIONS */
%pure_parser

%token RCSS_CLANG_INT
%token RCSS_CLANG_REAL
%token RCSS_CLANG_STR
%token RCSS_CLANG_VAR

%token RCSS_CLANG_LP "("
%token RCSS_CLANG_RP ")"
%token RCSS_CLANG_LB "{"
%token RCSS_CLANG_RB "}"

%token RCSS_CLANG_SAY "say"
%token RCSS_CLANG_HEAR "hear"
%token RCSS_CLANG_COACH_SIDE

%token RCSS_CLANG_META "meta"
%token RCSS_CLANG_FREEFORM "freeform"
%token RCSS_CLANG_UNSUPP "unsupported_clang"
%token RCSS_CLANG_INFO "info"
%token RCSS_CLANG_ADVICE "advice"
%token RCSS_CLANG_DEFINE "define"
%token RCSS_CLANG_DEFINEC "definec"
%token RCSS_CLANG_DEFINED "defined"
%token RCSS_CLANG_DEFINER "definer"
%token RCSS_CLANG_DEFINEA "definea"
/* meta stuff */
%token RCSS_CLANG_VER "ver"
/* region stuff */
%token RCSS_CLANG_NULL_REGION "null"
%token RCSS_CLANG_QUAD "quad"
%token RCSS_CLANG_ARC "arc"
%token RCSS_CLANG_PT "pt"
%token RCSS_CLANG_REG "reg"
%token RCSS_CLANG_TRI "tri"
%token RCSS_CLANG_REC "rec"
%token RCSS_CLANG_BALL "ball"

/* action stuff */
%token RCSS_CLANG_POS "pos"
%token RCSS_CLANG_HOME "home"
%token RCSS_CLANG_BALLTO "bto"
%token RCSS_CLANG_MARK "mark"
%token RCSS_CLANG_MARK_LINE "markl"
%token RCSS_CLANG_OFFLINE "oline"
%token RCSS_CLANG_HTYPE "htype"
%token RCSS_CLANG_BMOVE_TOKEN
/* directive stuff */
%token RCSS_CLANG_DO_DONT
%token RCSS_CLANG_TEAM
/* condition stuff */
%token RCSS_CLANG_TRUE_KW "true"
%token RCSS_CLANG_FALSE_KW "false"
%token RCSS_CLANG_PPOS "ppos"
%token RCSS_CLANG_BPOS "bpos"
%token RCSS_CLANG_BOWNER "bowner"
%token RCSS_CLANG_AND "and"
%token RCSS_CLANG_OR "or"
%token RCSS_CLANG_NOT "not"
%token RCSS_CLANG_PLAY_MODE_KW "playm"
%token    RCSS_CLANG_PLAY_MODE

%token RCSS_CLANG_CLEAR "clear"

%token RCSS_CLANG_L "<"
%token RCSS_CLANG_G ">"
%token RCSS_CLANG_E "=="
%token RCSS_CLANG_NE "!="
%token RCSS_CLANG_LE "<="
%token RCSS_CLANG_GE ">="

%token RCSS_CLANG_PLUS "+"
%token RCSS_CLANG_MINUS "-"
%token RCSS_CLANG_MULT "*"
%token RCSS_CLANG_DIV "/"

%token RCSS_CLANG_TIME "time"
%token RCSS_CLANG_OPP_GOAL "opp_goals"
%token RCSS_CLANG_OUR_GOAL "own_goals"
%token RCSS_CLANG_GOAL_DIFF "goal_diff"

%token RCSS_CLANG_UNUM "unum"
%token RCSS_CLANG_PASS "pass"
%token RCSS_CLANG_DRIBBLE "dribble"
%token RCSS_CLANG_SHOOT "shoot"
%token RCSS_CLANG_HOLD "hold"
%token RCSS_CLANG_INTERCEPT "intercept"
%token RCSS_CLANG_TACKLE "tackle"

%token RCSS_CLANG_RULE "rule"
%token RCSS_CLANG_ON_OFF "on|off"
%token RCSS_CLANG_ALL "all"
%token RCSS_CLANG_DEL "delete"
%token RCSS_CLANG_DEFRULE "definerule"
%token RCSS_CLANG_MODEL "model"
%token RCSS_CLANG_DIREC "direc"



/* the ERROR token is an error from lex and is never used below */
%token    RCSS_CLANG_ERROR





%left RCSS_CLANG_E RCSS_CLANG_NE
%left RCSS_CLANG_L RCSS_CLANG_G RCSS_CLANG_LE RCSS_CLANG_GE
%left RCSS_CLANG_PLUS RCSS_CLANG_MINUS
%left RCSS_CLANG_MULT RCSS_CLANG_DIV
 


%start command

%%

/* Grammar RULES */
command : RCSS_CLANG_LP RCSS_CLANG_SAY message RCSS_CLANG_RP
        | message 
        | RCSS_CLANG_LP RCSS_CLANG_HEAR RCSS_CLANG_INT
          RCSS_CLANG_COACH_SIDE RCSS_CLANG_INT message RCSS_CLANG_RP 
          { 
            $$ = $5;
            BUILDER.setTime( $3.getInt() );
            BUILDER.setSide( $4.getInt() );
            BUILDER.setTimeRecv( $5.getInt() );
          }
;

message : meta_mess     
        | freeform_mess 
        | info_mess     
        | advice_mess   
        | define_mess   
        | unsupp_mess   
        | rule_mess
        | del_mess
;

rule_mess : RCSS_CLANG_LP RCSS_CLANG_RULE activation_list RCSS_CLANG_RP
            { BUILDER.buildRuleMsg();
              BUILDER.setVer( 8 ); } 
;

activation_list : activation_list activation_element {} 
                | activation_element {}
;

activation_element : RCSS_CLANG_LP RCSS_CLANG_ON_OFF id_list RCSS_CLANG_RP
                { BUILDER.buildActivateRules( $2.getBool() ); }
;

id_list : RCSS_CLANG_VAR 
          { BUILDER.buildRuleID( $1.getStr() ); 
            BUILDER.buildRuleIDList(); }
        | RCSS_CLANG_LP id_list2 RCSS_CLANG_RP 
          { BUILDER.buildRuleIDList(); }
        | RCSS_CLANG_ALL
          { BUILDER.buildRuleIDListALL(); }
;

id_list2 : id_list2 RCSS_CLANG_VAR
            { BUILDER.buildRuleID( $2.getStr() ); }
          | RCSS_CLANG_VAR
            { BUILDER.buildRuleID( $1.getStr() ); }
;

del_mess : RCSS_CLANG_LP RCSS_CLANG_DEL id_list RCSS_CLANG_RP
           { BUILDER.buildDelMsg();
             BUILDER.setVer( 8 ); }
;
/* meta messages */
meta_mess : RCSS_CLANG_LP RCSS_CLANG_META meta_token_list RCSS_CLANG_RP
            { BUILDER.buildMetaMsg();
              BUILDER.setVer( 7 ); } 
;
meta_token_list : meta_token_list meta_token 
                | meta_token 
;

meta_token : RCSS_CLANG_LP RCSS_CLANG_VER num RCSS_CLANG_RP 
             { BUILDER.buildMetaTokenVer( $3.getDoub() ); } 
;

/* define messages */
define_mess : RCSS_CLANG_LP RCSS_CLANG_DEFINE define_token_list RCSS_CLANG_RP
              { BUILDER.buildDefineMsg();
                BUILDER.setVer( 8, 7 ); }
;

define_token_list : define_token_list define_token 
                  | define_token 
;

define_token : RCSS_CLANG_LP RCSS_CLANG_DEFINEC RCSS_CLANG_STR
               condition RCSS_CLANG_RP           
               { BUILDER.buildDefineCond( rcss::stripQuotes( $3.getStr() ) ); } 
             | RCSS_CLANG_LP RCSS_CLANG_DEFINED
               RCSS_CLANG_STR directive RCSS_CLANG_RP
               { BUILDER.buildDefineDir( rcss::stripQuotes( $3.getStr() ) ); } 
             | RCSS_CLANG_LP RCSS_CLANG_DEFINER
               RCSS_CLANG_STR region RCSS_CLANG_RP
               { BUILDER.buildDefineReg( rcss::stripQuotes( $3.getStr() ) ); } 
             | RCSS_CLANG_LP RCSS_CLANG_DEFINEA
               RCSS_CLANG_STR action RCSS_CLANG_RP
               { BUILDER.buildDefineAct( rcss::stripQuotes( $3.getStr() ) ); } 
             | RCSS_CLANG_LP RCSS_CLANG_DEFRULE define_rule RCSS_CLANG_RP
               { BUILDER.setVer( 8 ); }
;

define_rule : RCSS_CLANG_VAR RCSS_CLANG_MODEL rule 
            { BUILDER.buildDefineModelRule( $1.getStr() ); }
            | RCSS_CLANG_VAR RCSS_CLANG_DIREC rule
            { BUILDER.buildDefineDirectiveRule( $1.getStr() ); }
;

rule : RCSS_CLANG_LP condition directive_list RCSS_CLANG_RP
       { BUILDER.buildSimpleRule(); }
     | RCSS_CLANG_LP condition rule_list RCSS_CLANG_RP
       { BUILDER.buildNestedRule(); }
     | id_list
       { BUILDER.buildIDRule(); }
;

rule_list : rule_list rule
          | rule
;

/* freeform messages */
freeform_mess : RCSS_CLANG_LP RCSS_CLANG_FREEFORM RCSS_CLANG_STR RCSS_CLANG_RP
                {
                  std::string str = rcss::stripQuotes( $3.getStr() );
                  if( (int)str.length() > ServerParam::instance().SayCoachMsgSize )
                    YYERROR;
                  else
                    {
                      BUILDER.buildFreeformMsg( str ); 
                      BUILDER.setVer( 8, 7 ); 
                    }
                }
;

/* unsupported messages */
unsupp_mess : RCSS_CLANG_LP RCSS_CLANG_UNSUPP RCSS_CLANG_RP
                {
                  BUILDER.buildUnsuppMsg(); 
                  BUILDER.setVer( 8, 7 ); 
                }
;

info_mess : RCSS_CLANG_LP RCSS_CLANG_INFO
            token_list RCSS_CLANG_RP
            { BUILDER.buildInfoMsg();
              BUILDER.setVer( 7 ); }
;

advice_mess : RCSS_CLANG_LP RCSS_CLANG_ADVICE
              token_list RCSS_CLANG_RP
              { BUILDER.buildAdviceMsg();
                BUILDER.setVer( 7 ); }
;

token : RCSS_CLANG_LP RCSS_CLANG_INT
        condition directive_list RCSS_CLANG_RP 
        { BUILDER.buildTokenRule( $2.getInt() ); }
      | RCSS_CLANG_LP RCSS_CLANG_CLEAR RCSS_CLANG_RP
        { BUILDER.buildTokenClear(); } 
;

token_list : token_list token 
           | token 
;

action : RCSS_CLANG_LP RCSS_CLANG_POS
         region RCSS_CLANG_RP
         { BUILDER.buildActPos(); } 
       | RCSS_CLANG_LP RCSS_CLANG_HOME
         region RCSS_CLANG_RP
         { BUILDER.buildActHome(); }
       | RCSS_CLANG_LP RCSS_CLANG_BALLTO 
         region bmove_token_set RCSS_CLANG_RP
         { BUILDER.buildActBallToReg(); } 
       | RCSS_CLANG_LP RCSS_CLANG_BALLTO
         unum_set RCSS_CLANG_RP
         { BUILDER.buildActBallToPlayer(); }
       | RCSS_CLANG_LP RCSS_CLANG_MARK
         unum_set RCSS_CLANG_RP
         { BUILDER.buildActMark(); }
       | RCSS_CLANG_LP RCSS_CLANG_MARK_LINE
         unum_set RCSS_CLANG_RP 
         { BUILDER.buildActMarkLinePlayer(); }
       | RCSS_CLANG_LP RCSS_CLANG_MARK_LINE
         region RCSS_CLANG_RP
         { BUILDER.buildActMarkLineReg(); }
       | RCSS_CLANG_LP RCSS_CLANG_OFFLINE
         region RCSS_CLANG_RP
         { BUILDER.buildActOffsideLine(); }
       | RCSS_CLANG_LP RCSS_CLANG_HTYPE RCSS_CLANG_INT RCSS_CLANG_RP 
         { BUILDER.buildActHetType( $3.getInt() ); } 
       | RCSS_CLANG_STR 
         { BUILDER.buildActNamed( rcss::stripQuotes( $1.getStr() ) ); }
       | RCSS_CLANG_LP RCSS_CLANG_PASS 
         region RCSS_CLANG_RP
        { BUILDER.buildActPassReg();
           BUILDER.setVer( 8 ); }
       | RCSS_CLANG_LP RCSS_CLANG_PASS 
         unum_set RCSS_CLANG_RP
         { BUILDER.buildActPassUNum();
           BUILDER.setVer( 8 ); }
       | RCSS_CLANG_LP RCSS_CLANG_DRIBBLE
         region RCSS_CLANG_RP
         { BUILDER.buildActDribble();
           BUILDER.setVer( 8 ); }
       | RCSS_CLANG_LP RCSS_CLANG_CLEAR
         region RCSS_CLANG_RP
         { BUILDER.buildActClear();
           BUILDER.setVer( 8 ); }
       | RCSS_CLANG_LP RCSS_CLANG_SHOOT RCSS_CLANG_RP
         { BUILDER.buildActShoot();
           BUILDER.setVer( 8 ); }
       | RCSS_CLANG_LP RCSS_CLANG_HOLD RCSS_CLANG_RP
         { BUILDER.buildActHold();
           BUILDER.setVer( 8 ); }
       | RCSS_CLANG_LP RCSS_CLANG_INTERCEPT RCSS_CLANG_RP
         { BUILDER.buildActIntercept();
           BUILDER.setVer( 8 ); }
       | RCSS_CLANG_LP RCSS_CLANG_TACKLE
         unum_set RCSS_CLANG_RP
         { BUILDER.buildActTackle();
           BUILDER.setVer( 8 ); }
;

directive : RCSS_CLANG_LP RCSS_CLANG_DO_DONT
            RCSS_CLANG_TEAM unum_set action_list RCSS_CLANG_RP
            { BUILDER.buildDirComm( $2.getBool(),
                                                $3.getBool() ); } 
          | RCSS_CLANG_STR
            { BUILDER.buildDirNamed( rcss::stripQuotes( $1.getStr() ) ); } 
;

action_list : action_list action {}
            | action {}
;

directive_list : directive_list directive 
               | directive 
;

condition: RCSS_CLANG_LP RCSS_CLANG_TRUE_KW RCSS_CLANG_RP
           { BUILDER.buildCondTrue(); } 
         | RCSS_CLANG_LP RCSS_CLANG_FALSE_KW RCSS_CLANG_RP 
           { BUILDER.buildCondFalse(); } 
         | RCSS_CLANG_LP RCSS_CLANG_PPOS RCSS_CLANG_TEAM unum_set
           RCSS_CLANG_INT RCSS_CLANG_INT region RCSS_CLANG_RP
           { BUILDER.buildCondPlayerPos( $3.getBool(),
                                                     $5.getInt(),
                                                     $6.getInt() ); } 
         | RCSS_CLANG_LP RCSS_CLANG_BPOS
           region RCSS_CLANG_RP
           { BUILDER.buildCondBallPos(); } 
         | RCSS_CLANG_LP RCSS_CLANG_BOWNER
           RCSS_CLANG_TEAM unum_set RCSS_CLANG_RP
           { BUILDER.buildCondBallOwner( $3.getBool() ); } 
         | RCSS_CLANG_LP RCSS_CLANG_PLAY_MODE_KW RCSS_CLANG_PLAY_MODE
           RCSS_CLANG_RP
           { BUILDER.buildCondPlayMode( $3.getPM() ); } 
         | RCSS_CLANG_LP RCSS_CLANG_AND
           condition_list RCSS_CLANG_RP
           { BUILDER.buildCondAnd(); }
         | RCSS_CLANG_LP RCSS_CLANG_OR
           condition_list RCSS_CLANG_RP
           { BUILDER.buildCondOr(); } 
         | RCSS_CLANG_LP RCSS_CLANG_NOT
           condition RCSS_CLANG_RP
           { BUILDER.buildCondNot(); }
         | RCSS_CLANG_STR 
           { BUILDER.buildCondNamed( rcss::stripQuotes( $1.getStr() ) ); } 
         | RCSS_CLANG_LP cond_comp RCSS_CLANG_RP 
           { BUILDER.setVer( 8 ); }
         | RCSS_CLANG_LP RCSS_CLANG_UNUM
           RCSS_CLANG_VAR unum_set RCSS_CLANG_RP
           { BUILDER.buildCondUNum( rcss::clang::UNum( $3.getStr() ) );
             BUILDER.setVer( 8 ); }
         | RCSS_CLANG_LP RCSS_CLANG_UNUM
           RCSS_CLANG_STR unum_set RCSS_CLANG_RP
           { BUILDER.buildCondUNum( rcss::clang::UNum( $3.getStr() ) );
             BUILDER.setVer( 8 ); }
;

cond_comp : time_comp {}
          | opp_goal_comp {}
          | our_goal_comp {}
          | goal_diff_comp {}
;

time_comp : RCSS_CLANG_TIME comp RCSS_CLANG_INT
            { BUILDER.buildCondTime( $3.getInt(),
                                       $2.getCompOp() ); }
          | RCSS_CLANG_INT comp RCSS_CLANG_TIME
            { BUILDER.buildCondTime( $1.getInt(),
                                   $2.getCompOp().swap() ); }
;

opp_goal_comp : RCSS_CLANG_OPP_GOAL comp RCSS_CLANG_INT
                { BUILDER.buildCondOppGoal( $3.getInt(),
                                       $2.getCompOp() ); }
              | RCSS_CLANG_INT comp RCSS_CLANG_OPP_GOAL
                { BUILDER.buildCondOppGoal( $1.getInt(),
                                   $2.getCompOp().swap() ); }
;
 
our_goal_comp : RCSS_CLANG_OUR_GOAL comp RCSS_CLANG_INT
                { BUILDER.buildCondOurGoal( $3.getInt(),
                                       $2.getCompOp() ); }
              | RCSS_CLANG_INT comp RCSS_CLANG_OUR_GOAL
                { BUILDER.buildCondOurGoal( $1.getInt(),
                                   $2.getCompOp().swap() ); }
;

goal_diff_comp : RCSS_CLANG_GOAL_DIFF comp RCSS_CLANG_INT
                 { BUILDER.buildCondGoalDiff( $3.getInt(),
                                       $2.getCompOp() ); }
               | RCSS_CLANG_INT comp RCSS_CLANG_GOAL_DIFF
                 { BUILDER.buildCondGoalDiff( $1.getInt(),
                                   $2.getCompOp().swap() ); }
;

comp : RCSS_CLANG_L | RCSS_CLANG_LE | RCSS_CLANG_GE | RCSS_CLANG_G
     | RCSS_CLANG_E | RCSS_CLANG_NE
;

condition_list : condition_list condition 
		{ BUILDER.buildAddToCondList(); }
               | condition 
		{ BUILDER.buildCreateCondList(); }
;

region : RCSS_CLANG_LP RCSS_CLANG_NULL_REGION RCSS_CLANG_RP
         { BUILDER.buildRegNull(); } 
       | RCSS_CLANG_LP RCSS_CLANG_QUAD
         point point point point RCSS_CLANG_RP
         { BUILDER.buildRegQuad();
           BUILDER.setVer( 7 ); } 
       | RCSS_CLANG_LP RCSS_CLANG_ARC
         point num num num num RCSS_CLANG_RP 
         { BUILDER.buildRegArc( $4.getDoub(),
                                            $5.getDoub(),
                                            $6.getDoub(),
                                            $7.getDoub() ); } 
       | RCSS_CLANG_LP RCSS_CLANG_REG
         region_list RCSS_CLANG_RP 
         { BUILDER.buildRegUnion(); } 
       | RCSS_CLANG_STR
         { BUILDER.buildRegNamed( rcss::stripQuotes( $1.getStr() ) ); } 
       | point
         { BUILDER.buildRegPoint(); } 
       | RCSS_CLANG_LP RCSS_CLANG_TRI
         point point point RCSS_CLANG_RP
         { BUILDER.buildRegTri();
           BUILDER.setVer( 8 );  } 
       | RCSS_CLANG_LP RCSS_CLANG_REC
         point point RCSS_CLANG_RP
         { BUILDER.buildRegRec();
           BUILDER.setVer( 8 );  } 
;


region_list : region_list region 
            | region 
;

point : RCSS_CLANG_LP RCSS_CLANG_PT num num RCSS_CLANG_RP
        { BUILDER.buildPointSimple( $3.getDoub(), 
                                                $4.getDoub() ); } 
      | RCSS_CLANG_LP RCSS_CLANG_PT num num point RCSS_CLANG_RP
        { BUILDER.buildPointRel( $3.getDoub(),
                                             $4.getDoub() ); } 
      | RCSS_CLANG_LP RCSS_CLANG_PT RCSS_CLANG_BALL RCSS_CLANG_RP
        { BUILDER.buildPointBall(); }
      | RCSS_CLANG_LP RCSS_CLANG_PT RCSS_CLANG_TEAM RCSS_CLANG_INT
        RCSS_CLANG_RP 
        { BUILDER.buildPointPlayer( $3.getBool(),
                     rcss::clang::UNum( ( rcss::clang::UNum::unum_t )$4.getInt() ) ); }
      | RCSS_CLANG_LP RCSS_CLANG_PT RCSS_CLANG_TEAM RCSS_CLANG_VAR
        RCSS_CLANG_RP
        { BUILDER.buildPointPlayer( $3.getBool(),
                                                rcss::clang::UNum( $4.getStr() ) );
          BUILDER.setVer( 8 ); }
      | RCSS_CLANG_LP RCSS_CLANG_PT RCSS_CLANG_TEAM RCSS_CLANG_STR
        RCSS_CLANG_RP
        { BUILDER.buildPointPlayer( $3.getBool(),
                                                rcss::clang::UNum( $4.getStr() ) );
          BUILDER.setVer( 8 ); }
      | RCSS_CLANG_LP point_list RCSS_CLANG_RP {}
;

point_list : point_list RCSS_CLANG_PLUS point_list
           { BUILDER.buildPointArith( $2.getArithOp() );
                BUILDER.setVer( 8 ); }
            | point_list RCSS_CLANG_MINUS point_list
           { BUILDER.buildPointArith( $2.getArithOp() );
                BUILDER.setVer( 8 ); }
            | point_list RCSS_CLANG_MULT point_list
           { BUILDER.buildPointArith( $2.getArithOp() );
                BUILDER.setVer( 8 ); }
            | point_list RCSS_CLANG_DIV point_list
           { BUILDER.buildPointArith( $2.getArithOp() );
                BUILDER.setVer( 8 ); }
            | point {}
;

num : RCSS_CLANG_INT
      { $$ = (double)$1.getInt(); }
    | RCSS_CLANG_REAL
      { $$ = $1; }
;

unum_set : RCSS_CLANG_LB unum_list RCSS_CLANG_RB 
           { BUILDER.buildUNumSet(); }
;

unum_list : unum
          | unum_list unum
;

unum : RCSS_CLANG_INT 
            { BUILDER.buildUNum( 
            rcss::clang::UNum( ( rcss::clang::UNum::unum_t )$1.getInt() ) ); }
          | RCSS_CLANG_VAR
            { 
     BUILDER.buildUNum( rcss::clang::UNum( $1.getStr() ) ); 
              BUILDER.setVer( 8 );
            }
          | RCSS_CLANG_STR
            {
     BUILDER.buildUNum( rcss::clang::UNum( $1.getStr() ) );
              BUILDER.setVer( 8 );
            }
;

bmove_token_set : RCSS_CLANG_LB bmove_token_list RCSS_CLANG_RB
;

bmove_token_list : bmove_token_list RCSS_CLANG_BMOVE_TOKEN 
         { BUILDER.buildBallMoveToken( $2.getBMT() ); } 
         | RCSS_CLANG_BMOVE_TOKEN 
         { BUILDER.buildBallMoveToken( $1.getBMT() ); } 
;
 
%%


extern "C" void parse()
{
  // dumby function use to make the library easy to find with autoconf
}

void yyerror (const char*)
{
/*   std::cerr << s << std::endl;*/
  //do nothing
}

int yyerror (char* s)
{
	yyerror ( (const char*)s );
	return 0;
}
