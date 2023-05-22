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

#include<iostream>
#include<vector>
#include<set>
using namespace std;

#include <stdio.h>
#include "../include/AbsSynTree.hpp"

#define YYDEBUG 10
int	lineno = 1;
bool flagcur = false;
set<string> e;
vector<string> errs;

int yylex();
int main(int argc,char* argv[]);
int yyerror(char const *s);
int warning(char const *s, char const *t);

int i;	
extern FILE *yyin; 

%}

%union{
	struct node* tnode;
}

%token BEG END
%token T_INT T_BOOL
%token READ WRITE
%token DECL ENDDECL
%token <tnode> VAR NUM
%token IF THEN ELSE ENDIF
%token LOGICAL_AND LOGICAL_NOT LOGICAL_OR
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token WHILE DO ENDWHILE FOR 
%token T F 
%token MAIN RETURN


%left '<' '>'
%left EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%left '+' '-'
%left '*' '/'
%left '%'
%left LOGICAL_AND LOGICAL_OR
%left LOGICAL_NOT

%type <tnode> Prog Gdecl_sec Gdecl_list Gdecl ret_type Glist Gid func arg_list

%type <tnode> arg_list1 arg arg_type var_list Fdef_sec Fdef func_ret_type func_name

%type <tnode> FargList ret_stmt MainBlock Ldecl_sec main Ldecl_list Ldecl type Lid_list

%type <tnode> Lid stmt_list statement read_stmt write_stmt assign_stmt cond_stmt func_stmt

%type <tnode> func_call param_list param_list1 para expr str_expr var_expr
%%

Prog: Gdecl_sec Fdef_sec MainBlock {
	if(flagcur) {
		for(string s : e){
			cout << s << endl;
		}
		exit(0);
	}
		parse($1,$2,$3);
	}
	;

// ****************** GLOBAL DECLARATION SECTION ************* //

Gdecl_sec:	DECL Gdecl_list ENDDECL {
		$$ = $2;
	}
	;
	
Gdecl_list: { $$ = NULL; }
	| 	Gdecl Gdecl_list {
		$1->addNode($2,4);
		// $$ = new node($1,$2,4);
		$$ = $1;
	}
	| error {
		// cout << "syntax error at lineno " << lineno << endl;
		e.insert( "syntax error at lineno " + string(to_string(lineno)));
		flagcur = true;
	}
	;

Gdecl:	ret_type Glist ';' {
		// cout << " yacc "<< $1->datatype << endl;
		string temp = $1->datatype;
		$$ = new node(temp,$2);
	}
	;
		
ret_type:	T_INT   {
		$$ = new node("integer"); // string node
 	} 
	|	T_BOOL 		{ 
		$$ = new node("bool");
	}
	;

Glist:	Gid {
		$$ = $1;
	}
	| 	func {
		$$ = $1;
	}
	|	Gid ',' Glist  {
		// $$ = new node($1,$3,6);
		$1->addNode($3,6);
		$$ = $1;
	}
	|	func ',' Glist {
		// $$ = new node($1,$3,7);
		$1->addNode($3,7);
		$$ = $1;
	}
	;
	
Gid:	VAR		{ 
		$$ = new node($1->datatype, -1);
 	}
	|	Gid '[' NUM ']'	{ 
		// cout << "yacc" << " " << $1->varname << endl;
		$$ = new node($1->varname, $3->size_ );
	}
	;
	
func:	VAR '(' arg_list ')' {
		// $$ = new node( $1, $3 , 7);
		$1->addFunctionNode($3,7);
		$$ = $1;
	}
	;
			
arg_list:	{
		$$ = NULL;
	}
	|	arg_list1 {
		$$ = $1;
	}
	;
		
arg_list1:	arg_list1 ';' arg {
		// $$ = new node($1,$3, 8);
		$3->addNodeNode($1,8);
		$$ = $3;

	}
	|	arg {
		// $$ = new node(NULL,$1, 8);
		$1->addNodeNode(NULL,8);
		$$ = $1;
	}
	;
	
arg:	arg_type var_list {
		// $$ = new node($1,$2,9);
		// cout << "yacc " << $1->datatype << endl;
		$1->addNode($2,9);
		$$ = $1;
	}
	;
		
arg_type:	T_INT		 { 
		$$ = new node("integer");
	}
	|	T_BOOL 		 { 
		$$ = new node("bool");
	}
	;

var_list:	VAR 		 { 
		// $$ = new node($1->datatype, -1);
		// cout << "yacc " << $1->datatype << endl;
		$1->addNode(NULL,10);
		$$ = $1;
 	}
	|	VAR ',' var_list { 
		// $$ = new node($1, $3, 10);
		$1->addNode($3,10);
		$$ = $1;
	}
	;


// ****************** FUNCTION DEFINATION SECTION ************* //

Fdef_sec:	{ $$ = NULL; }
	|	Fdef_sec Fdef {
		// $$ = new node($1,$2,101);
		$2->addNode($1,14);
		// $$ = new node($1,$2,4);
		$$ = $2;
	}
	;
		
Fdef:	func_ret_type func_name '(' FargList ')' '{' Ldecl_sec BEG stmt_list ret_stmt END '}'	{
		$$ = new node($1,$2,$4,$7,$9,$10);
	}
	;
		
func_ret_type:	T_INT		{ 
		$$ = new node("integer"); // string node
	}
	|	T_BOOL		{ 
		$$ = new node("bool"); // string node
	}
	| error {
		// cout << "syntax error at lineno " << lineno << endl;
		e.insert( "syntax error at lineno " + string(to_string(lineno)));
		flagcur = true;
		
	} 
	;

func_name:	VAR		{
		$$ = $1;
	}
	;
		
FargList:	arg_list	{
		$$ = $1;
	}
	;

ret_stmt:	{
		$$ = NULL;
	}
	| RETURN expr ';'	{
		$$ = $2;
	}
	;


// ****************** MAIN BLOCK SECTION ************* //

MainBlock: 	func_ret_type main '('')''{' Ldecl_sec BEG stmt_list ret_stmt END  '}'		{ 
		$$ = new node($1,$6,$8,$9);
	}
	;
		
main:	MAIN	{ $$ = NULL; }
	;
		
Ldecl_sec:	{ $$ = NULL; }
	| DECL Ldecl_list ENDDECL	{
		$$ = $2;
	}
	;

Ldecl_list:		{
		$$ = NULL;
	}
	|	Ldecl Ldecl_list {
		// $$ = new node($1,$2,500);
		$1->addNodeNode($2,500);
		$$ = $1;
	}
	;

Ldecl:	type Lid_list ';' {
		// $$ = new node($1,$2,501);
		$1->addNode($2,501);
		$$ = $1;
	}
	;

type:	T_INT			{ 
		$$ = new node("integer"); 
	}
	|	T_BOOL			{ 
		$$ = new node("bool"); 
	}
	;

Lid_list:	Lid {
		$$ = $1;
	}
	|	Lid ',' Lid_list {
		// $$ = new node($1,$3,502);
		$1->addNode($3,502);
		$$ = $1;
	}
	;
		
Lid:	VAR			{
		$$ = new node($1->datatype,-1);
	}
	;

stmt_list:	/* NULL */		{ 
		$$ = NULL;
	}
	|	statement stmt_list	{ 
		// $$ = new node($1,$2,503);
		$1->addNode($2);
		$$ = $1;
	}
	;

statement:	assign_stmt  ';'		{  $$ = $1;	 } 
	|	read_stmt ';'		{ $$ = $1; }
	|	write_stmt ';'		{ $$ = $1; }
	|	cond_stmt 		{ $$ = $1; }
	|	func_stmt ';'		{ $$ = $1; }
	| error {
		// cout << "syntax error at lineno " << lineno << endl;
		e.insert( "syntax error at lineno " + string(to_string(lineno)));
		flagcur = true;
	}
	;

read_stmt:	READ '(' var_expr ')' {	
		$$ = new node($3,NULL,505);
	}
	;

write_stmt:	WRITE '(' expr ')' 	{ 
		$$ = new node($3,NULL,506);
	}
	| WRITE '(''"' str_expr '"'')'      { 
		$$ = new node($4,NULL,516);
	}
	;
	
assign_stmt:	var_expr '=' expr 	{ 

		// cout<< " -- yacc " << $1->varname << endl;

		$$ = new node($1,$3,507);
	}
	;

cond_stmt:	IF expr THEN stmt_list ENDIF  ';'	{
		$$ = new node($2,$4,510);
	}
	|	IF expr THEN stmt_list ELSE stmt_list ENDIF ';'	{
		$$ = new node($2,$4,$6,511);
	}
	|	WHILE expr DO stmt_list ENDWHILE ';' {
		// left ,, right
		$$ = new node($2,$4,512);
	}
	|    FOR '(' assign_stmt  ';'  expr ';'  assign_stmt ')' '{' stmt_list '}' {
		$$ = new node($3,$5,$7,$10,513);
	}
	;
	
func_stmt:	func_call 		{
		// $$ = $1;
		// cout << $1->next->op << endl;
		$$ = new node($1,NULL,600);
	}
	;
		
func_call:	VAR '(' param_list ')'	{
		// $$ = new node($1,$3,600);
		$1->addNode($3,600);
		$$ = $1;
	}
	;

param_list:	{
		$$ = NULL;
	}	
	|	param_list1	{
		$$ = $1;
	}
	;

param_list1:	para {
		$$ = $1;
	}
	|	para ',' param_list1 {
		// $$ = new node($1,$3,601);
		$1->addNode($3,10);
		$$ = $1;
	}
	;

para:	expr	{
		$$ = $1;
	}
	;

// **************** EXPR SECTION ********************** //

expr:	NUM 			{ 
		// cout << "yacc "<<$1->size_ << endl;
		$$ = new node(NULL,"",NULL,$1->size_,false,false);
	}
	|	'-' NUM			{
		node* l = new node(NULL,"",NULL,0,false,false);
		node* r = new node(NULL,"",NULL,$2->size_,false,false);
		$$ = new node(l,"-",r,$2->size_,false,false);
	}
	|	var_expr		{
		$$ = new node($1->next,$1->varname,NULL,$1->size_,true,false);
	}
	|	T			{
		$$ = new node(NULL,"true",NULL,1,false,true);
	}
	|	F			{
		$$ = new node(NULL,"false",NULL,0,false,true);
	}
	|	'(' expr ')'		{
		$$ = $2;
	}
	|	expr '+' expr 		{
		$$ = new node($1,"+",$3,-1,false,false);
	}
	|	expr '-' expr	 	{
		$$ = new node($1,"-",$3,-1,false,false);
	}
	|	expr '*' expr 		{
		$$ = new node($1,"*",$3,-1,false,false);
	}
	|	expr '/' expr 		{
		$$ = new node($1,"/",$3,-1,false,false);
	}
	|	expr '%' expr 		{
		$$ = new node($1,"%",$3,-1,false,false);
	}
	|	expr '<' expr		{
		// cout << "yacc " << "<" << endl;
		$$ = new node($1,"<",$3,-1,false,false);
	}
	|	expr '>' expr		{
		$$ = new node($1,">",$3,-1,false,false);
	}
	|	expr GREATERTHANOREQUAL expr	{
		$$ = new node($1,">=",$3,-1,false,false);
	}
	|	expr LESSTHANOREQUAL expr	{
		$$ = new node($1,"<=",$3,-1,false,false);
	}
	|	expr NOTEQUAL expr			{
		$$ = new node($1,"!=",$3,-1,false,false);
	}
	|	expr EQUALEQUAL expr	{
		$$ = new node($1,"==",$3,-1,false,false);
	}
	|	LOGICAL_NOT expr	{
		$$ = new node(NULL,"!",$2,-1,false,false);
	}
	|	expr LOGICAL_AND expr	{
		$$ = new node($1,"and",$3,-1,false,false);
	}
	|	expr LOGICAL_OR expr	{
		$$ = new node($1,"or",$3,-1,false,false);
	}
	|	func_call		{
		$$ = new node($1,"function call",NULL,-1,false,false);
	}
	;

str_expr:  VAR {
		$$ = new node($1->datatype,-1);
	}
    | str_expr VAR   {
		$$ = new node($1,$2,400);
	}
    ;
	
var_expr:	VAR	{ 
		// cout << "yacc " << $1->datatype << endl;
		$$ = new node($1->datatype,-1);
	}
	|	var_expr '[' expr ']'	{
		// $$ = new node($1,$3,201);
		$1->addArrayNode($3,700);
		$$ = $1;
		// $$ = new node($1->varname, $3->size_ );
	}
	;
%%

/* end of grammar */

#include <stdio.h>
using namespace std;
#include <ctype.h>
char	*progname;	/* for error messages */
char* filename;

int main(int argc,char* argv[])
{
	// progname = argv[0];
	// yyparse();
	// cout << argv[1] << endl;
	filename = argv[2];
	yyin = fopen(argv[1], "r");
	yyparse();
}

int yyerror(char const *s) /* called for yacc syntax error */
{
	warning(s, (char *) 0);
	// fprintf(stderr, "%s\n", s);
	// yyerrok(); // continue parsing after error
}

int warning(char const *s, char const *t)  /* print warning message */
{
	fprintf(stderr, "%s: %s", progname, s);
	if (t)
		fprintf(stderr, " %s", t);
	fprintf(stderr, " near line %d\n", lineno);
}
