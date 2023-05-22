/*
 *   This file is part of SIL Compiler.
 *
 *  SIL Compiler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SIL Compiler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SIL Compiler.  If not, see <http://www.gnu.org/licenses/>.
 */

%{
#include <iostream>
using namespace std;	
#include "y.tab.h"
#include "../include/AbsSynTree.hpp"


extern int lineno;
// int wflag;
%}

%option noyywrap
digit	[0-9]
letter	[a-zA-Z]

%%
"main"	            { 
    // cout << "main" << endl; 
    return MAIN; }	
"return" 			{ return RETURN; }
"begin"				{ 
    // cout << "begin" << endl; 
    return BEG; }	
"end"		 	    { return END; }	
"read"				{ return READ; }
"write"			    { return WRITE; }	
"integer"			{ 
    // cout << "integer\n";  
    return T_INT; }	
"boolean"		    { 
    // cout << "bool\n"; 
    return T_BOOL; }	
"decl"				{ 
    // cout << "decl\n"; 
    return DECL; }
"enddecl"		    { 
    // cout << "enddecl\n"; 
    return ENDDECL; }
"if"				{ return IF;}
"then"				{ return THEN;}	
"else"				{ return ELSE;}	
"endif"				{ return ENDIF;}
"do"				{ 
    // cout << "DO" << endl;
    return DO; }
"while"				{ 
    // cout << "while" << endl; 
    return WHILE; }
"endwhile"			{ 
    // cout << "endwhile" << endl;
    return ENDWHILE;
    }
"for" 				{ return FOR;}		
"=="				{ return EQUALEQUAL; }	
"<="				{ return LESSTHANOREQUAL; }
">="				{ return GREATERTHANOREQUAL; }
"!="				{ return NOTEQUAL; }	
"AND"				{ return LOGICAL_AND; }
"OR"				{ return LOGICAL_OR; }
"NOT"			    { return LOGICAL_NOT; }
"true"			    { return T; }
"false"			    { return F; }
{digit}+			{
    int number = atoi(yytext);
    // cout << number << endl; 
    yylval.tnode = new node(number);
    return NUM;
}

{letter}({letter}|{digit})*	{ 
    
    string s = getNew(yytext);
    // cout << s << endl; 
    yylval.tnode = new node(s);
    return VAR;
}
[-+*/=()><;,\{\}\[\]\%]         { return *yytext; }
			
["]                             { return *yytext; }

[\n]   {lineno++;}

[ ]     {;}

[ \t]				;
.				;
%%
