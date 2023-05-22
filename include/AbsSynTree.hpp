#include <iostream>
#include <string>
#include <string.h>
using namespace std;

#include <stdbool.h>

#define inf 1e6

#define program 1

#ifndef _structres_
#define _structres_

/*
TODO
for each function sepearte symbol table
*/

struct node
{

    node *left, *middle, *right, *rightr, *next, *nextt;
    int type;
    string datatype;

    node(node *p1, node *p2, node *p3)
    {
        this->type = 1; // body of program
        this->left = p1;
        this->middle = p2;
        this->right = p3;
    }

    node(string datatype)
    {
        this->type = 2; // to store datatype
        this->datatype = datatype;
    }

    node(node *p1, node *p2, int type)
    {
        // gdeclist 4
        this->type = type;
        this->left = p1;
        this->right = p2;
        this->middle = NULL;
    }

    node(node *p1, node *p2, node *p3, int type)
    {
        this->type = type;
        this->left = p1;
        this->middle = p2;
        this->right = p3;
    }

    node(node *p1, node *p2, node *p3, node *p4, int type)
    {
        this->type = type;
        this->left = p1;
        this->middle = p2;
        this->right = p3;
        this->rightr = p4;
    }

    node(string s, node *p1)
    {
        this->type = 6; // gdecl
        this->datatype = s;
        this->right = p1;
    }

    string varname;
    int size_ = -1;

    node(int num)
    {
        this->type = 4;
        this->size_ = num;
        this->left = NULL;
        this->right = NULL;
        this->middle = NULL;
    }

    node(string vname, int size_)
    {
        this->type = 6; // variable and arrays
        this->varname = vname;
        this->size_ = size_;
    }

    node *fret, *fname, *farglist, *flocaldict, *stmtlist, *fretstmt;

    node(node *fret, node *fname, node *farglist, node *flocaldict, node *stmtlist, node *fretstmt)
    {
        this->type = 100;
        this->fret = fret;
        this->fname = fname;
        this->farglist = farglist;
        this->flocaldict = flocaldict;
        this->stmtlist = stmtlist;
        this->fretstmt = fretstmt;
    }

    string op;
    int value;
    bool isvar, isbool;

    // expr
    node(node *left, string op, node *right, int value, bool isvar, bool isbool)
    {
        this->type = 200;
        this->value = value;
        this->isvar = isvar;
        this->isbool = isbool;
        this->left = left;
        this->right = right;
        this->op = op;
    }

    // main function
    node(node *fret, node *ldecl, node *stmtlist, node *retstmt)
    {
        this->fret = fret;
        this->flocaldict = ldecl;
        this->stmtlist = stmtlist;
        this->fretstmt = retstmt;
    }

    void addNode(node *n, int type)
    {
        this->type = type;
        this->next = n;
    }

    void addArrayNode(node *n, int value)
    {
        this->size_ = value;
        this->next = n;
    }

    void addFunctionNode(node *n, int value)
    {
        this->size_ = value;
        this->farglist = n;
    }

    void addNode(node *n)
    {
        // this->type = type;
        this->next = n;
    }

    void addNodeNode(node *n, int type)
    {
        this->type = type;
        this->nextt = n;
    }
};

typedef struct node node;

char *getNew(char *p);
#endif

void parse(node *, node *, node *);
