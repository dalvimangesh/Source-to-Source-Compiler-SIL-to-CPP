#include "../include/AbsSynTree.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

void writeToFile(const std::string &filename, const std::string &content)
{
    std::ofstream file("CPP_output/" + filename, std::ios::trunc);
    if (file.is_open())
    {
        file << content;
        file.close();
    }
}

void writeToFileAST(const std::string &filename, const std::string &content)
{
    std::ofstream file("AST_output/" + filename, std::ios::trunc);
    if (file.is_open())
    {
        file << content;
        file.close();
    }
}

using namespace std;
#include <map>

extern char *filename;

struct info
{
    string name;
    int val;
    int type;
    int *arr;
    int size_;
    int datatype;
    map<string, info *> symboltable;
    info(string s, int type, int size_)
    {
        this->name = s;
        this->type = type;
        if (type == 2)
        {
            arr = new int[size_];
            this->size_ = size_;
        }
        if (type == 3)
        {
            this->type = 3;
        }
    }
    info(string s, int type, int size_, int datatype)
    {
        this->name = s;
        this->type = type;
        this->datatype = datatype;
        if (type == 2)
        {
            arr = new int[size_];
            this->size_ = size_;
        }
        if (type == 3)
        {
            this->type = 3;
        }
    }
};

map<string, info *> symboltable;
ostringstream ast;
ostringstream err;
ostringstream cpp;
bool flag = false;

char *getNew(char *p)
{
    char *temp = new char[100];
    int i = 0;
    while ((*p) != '\0')
    {
        temp[i] = *p;
        p++;
        i++;
    }
    temp[i] = '\0';
    return temp;
}

void printExpr(node *root)
{

    if (root == NULL)
    {
        return;
    }

    if (root->isvar)
    {
        // ast << root->value << " ";
        if (root->value == -1)
        {
            // ast << "--------->"<<root->op << endl;
            // if (symboltable.find(root->op) == symboltable.end())
            // {
            //     err << "UNDEF VAR " << root->op << endl;
            //     flag = true;
            // }
            ast << " VAR ";
            cpp << " " << root->op << " ";
        }
        else
        {
            // if (symboltable.find(root->op) == symboltable.end())
            // {
            //     err << "UNDEF ARRAY VAR " << root->op << endl;
            //     flag = true;
            // }
            // ast << "--------->"<<root->op << endl;
            ast << " ARRREF "
                << " ";
            cpp << root->op << " [ ";
            printExpr(root->left);
            cpp << " ] ";
        }
        return;
    }

    if (root->op == "function call")
    {
        ast << "FUN CALL    ";

        node *paramlist = root->left->next;

        cpp << root->left->datatype << "(";

        while (paramlist)
        {
            printExpr(paramlist);
            paramlist = paramlist->next;
            if (paramlist)
            {
                cpp << ",";
            }
        }
        ast << endl;
        cpp << ")\n";
        return;
    }

    else if ((root->left == NULL) and (root->right == NULL))
    {
        ast << " NUM ";
        cpp << root->value;
        return;
    }

    if (root->op == "<")
    {
        cpp << "( ";
        ast << " LESS THAN ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " < ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == "+")
    {
        cpp << "( ";
        ast << " PLUS ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " + ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == "-")
    {
        cpp << "( ";
        ast << " SUB ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " - ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == "*")
    {
        cpp << "( ";
        ast << " MUL ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " * ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == "/")
    {
        cpp << "( ";
        ast << " DIV ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " / ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == ">")
    {
        cpp << "( ";
        ast << " GREATER THAN ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " > ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == "==")
    {
        cpp << "( ";
        ast << " IS EQUAL TO ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " == ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == "!=")
    {
        cpp << "( ";
        ast << " IS NOT EQUAL TO  ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " != ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == "<=")
    {
        cpp << "( ";
        ast << " LESS THAN EQUAL TO  ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " <= ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == ">=")
    {
        cpp << "( ";
        ast << " GREATER THAN EQUAL TO  ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " >= ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == "NOT")
    {
        cpp << "( ";
        ast << " NOT  ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " ! ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }
    if (root->op == "AND")
    {
        cpp << "( ";
        ast << " AND ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " & ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == "OR")
    {
        cpp << "( ";
        ast << " OR ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " | ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == "true")
    {
        cpp << "( ";
        ast << " TRUE ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " true ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }

    if (root->op == "false")
    {
        cpp << "( ";
        ast << " FALSE ";
        cpp << "( ";
        printExpr(root->left);
        cpp << ") ";
        cpp << " false ";
        cpp << "( ";
        printExpr(root->right);
        cpp << ") ";
        cpp << ") ";
    }
}

int getExprValue(node *root)
{

    if (root == NULL)
    {
        return 0;
    }

    if (root->isvar)
    {
        // ast << root->value << " ";
        if (root->value == -1)
        {
            // ast << " GET " << root->op << endl;
            return symboltable[root->op]->val;
        }
        else
        {
            // ast << " ARR VAR "<< " ";
            // ast << root->op <<  " expr ";
            int index = getExprValue(root->left);
            // ast << index << endl;
            return symboltable[root->op]->arr[index];
        }
    }

    else if ((root->left == NULL) and (root->right == NULL))
    {
        return root->value;
    }

    if (root->op == "+")
    {
        int l = getExprValue(root->left);
        int r = getExprValue(root->right);
        return l + r;
    }

    if (root->op == "-")
    {
        int l = getExprValue(root->left);
        int r = getExprValue(root->right);
        return l - r;
    }

    if (root->op == "*")
    {
        int l = getExprValue(root->left);
        int r = getExprValue(root->right);
        return l * r;
    }

    if (root->op == "/")
    {
        int l = getExprValue(root->left);
        int r = getExprValue(root->right);
        return l / r;
    }

    if (root->op == "<")
    {
        int l = getExprValue(root->left);
        int r = getExprValue(root->right);
        return l < r;
    }

    if (root->op == ">")
    {
        int l = getExprValue(root->left);
        int r = getExprValue(root->right);
        return l > r;
    }

    if (root->op == "==")
    {

        int l = getExprValue(root->left);
        int r = getExprValue(root->right);
        return l == r;
    }

    if (root->op == "!=")
    {
        int l = getExprValue(root->left);
        int r = getExprValue(root->right);
        return l != r;
    }
    return 0;
}

// <, >, ! =, > =, < =.

void assingment(node *stmts)
{
    ast << "ASSIGN ";

    // ast << local->type << " ";

    if (stmts->left->size_ == -1)
    {
        ast << " VAR ";
        cpp << stmts->left->varname << " = ";
        // if (symboltable.find(stmts->left->varname) == symboltable.end())
        // {
        //     err << "UNDEF VAR " << stmts->left->varname << endl;
        //     flag = true;
        // }
    }
    else
    {
        ast << " ARRREF ";
        cpp << stmts->left->varname << "[ ";
        // if (symboltable.find(stmts->left->varname) == symboltable.end())
        // {
        //     err << "UNDEF ARRAY VAR " << stmts->left->varname << endl;
        //     flag = true;
        // }
        printExpr(stmts->left->next);

        cpp << " ] "
            << " = ";
    }
    printExpr(stmts->right);
}

void setassingment(node *stmts)
{
    // ast << "ASSIGN ";

    // ast << local->type << " ";
    int val = getExprValue(stmts->right);

    if (stmts->left->size_ == -1)
    {
        // ast << " VAR ";
        // ast << "SET " <<stmts->left->varname << endl;
        symboltable[stmts->left->varname]->val = val;
    }
    else
    {
        // ast << " ARR VAR ";
        // ast << stmts->left->varname << endl;
        int index = getExprValue(stmts->left->next);
        symboltable[stmts->left->varname]->arr[index] = val;
        // ast << " vnmae "<< stmts->left->varname << " vnmae ";
    }
    // printExpr(stmts->right);
}

void pritntSyb()
{
    cout << "\n----------SYMBOL-TABLE---------\n";
    for (auto e : symboltable)
    {
        if (e.second->type == 1)
        {
            cout << e.first << " " << e.second->val << endl;
        }
        else if (e.second->type == 2)
        {
            cout << e.first << " ";
            for (int i = 0; i < e.second->size_; i++)
            {
                cout << e.second->arr[i] << " ";
            }
            cout << endl;
        }
    }
    cout << "-------------------------------";
    cout << endl;
}

void ASTStatements(node *stmts)
{

    if (stmts == NULL)
    {
        return;
    }

    int TYPE_S = stmts->type;

    // cout << TYPE_S << endl;

    // ast << TYPE_S << " ";

    if (TYPE_S == 507)
    { // assingment
        assingment(stmts);
    }

    if (TYPE_S == 512)
    { // while

        ast << "WHILE ";
        cpp << "while (";

        node *expr = stmts->left;
        node *lstmts = stmts->right;

        cpp << "( ";

        printExpr(expr);

        cpp << ") ";

        cpp << "){\n";

        ast << endl;

        while (lstmts)
        {
            ASTStatements(lstmts);
            if (lstmts->next)
            {
                lstmts = lstmts->next->next;
            }
            else
            {
                lstmts = lstmts->next;
            }
            ast << endl;
        }
        cpp << "}\n";
        ast << "END WHILE ";
    }

    if (TYPE_S == 505)
    { // READ
        // cout << "inside read";
        ast << "READ ";
        cpp << "cin >> ";
        // printExpr(lstmts->left);
        node *varexpr = stmts->left;
        // cout << varexpr->type << endl;
        if (varexpr->size_ == -1)
        {
            // ast << varexpr->varname << " ";
            ast << " VAR "
                << " ";
            cpp << varexpr->varname;
        }
        else
        {
            ast << varexpr->varname;
            cpp << varexpr->varname << " [ ";
            printExpr(varexpr->next);
            cpp << " ]\n";
        }
    }

    if (TYPE_S == 506)
    {
        // WRITE
        cpp << " cout << ";
        ast << "WRITE ";

        cpp << "( ";
        printExpr(stmts->left);
        cpp << ") ";
        cpp << "<< endl;";
    }

    if (TYPE_S == 511)
    { // if-else

        ast << "IF     ";
        cpp << "if ( ";

        node *expr = stmts->left;
        node *lstmts = stmts->middle;

        cpp << "( ";
        printExpr(expr);
        cpp << ") ";

        cpp << " ) { ";

        ast << endl;

        // ast << "THEN\n";

        while (lstmts)
        {

            ASTStatements(lstmts);
            // lstmts = lstmts->next;
            if (lstmts->next)
            {
                lstmts = lstmts->next->next;
            }
            else
            {
                lstmts = lstmts->next;
            }
            ast << endl;
            ast << endl;
        }

        cpp << " } else { \n";

        ast << "ELSE     ";
        lstmts = stmts->right;

        while (lstmts)
        {

            ASTStatements(lstmts);
            // lstmts = lstmts->next;
            if (lstmts->next)
            {
                lstmts = lstmts->next->next;
            }
            else
            {
                lstmts = lstmts->next;
            }
            ast << endl;
            ast << endl;
        }

        // ast << endl;
        cpp << "}\n";
        ast << "END IF";
    }

    if (TYPE_S == 510)
    { // if-else

        ast << "IF     ";
        cpp << "if ( ";

        node *expr = stmts->left;
        node *lstmts = stmts->right;

        cpp << "( ";
        printExpr(expr);
        cpp << ") ";

        cpp << " ) { ";

        ast << endl;

        // ast << "THEN\n";

        while (lstmts)
        {

            ASTStatements(lstmts);
            // lstmts = lstmts->next;
            if (lstmts->next)
            {
                lstmts = lstmts->next->next;
            }
            else
            {
                lstmts = lstmts->next;
            }
            ast << endl;
        }
        cpp << "}\n";
    }

    if (TYPE_S == 600)
    { // function call

        ast << "FUN CALL    ";

        node *paramlist = stmts->left->next;

        cpp << stmts->left->datatype << "( ";

        // cpp << "( ";
        // // printExpr(stmts);
        // cpp << ") ";

        while (paramlist)
        {
            cpp << "( ";
            printExpr(paramlist);
            cpp << ") ";
            paramlist = paramlist->next;
            if (paramlist)
            {
                cpp << ",";
            }
        }
        cpp << ")\n";
        ast << endl;
    }

    // if( TYPE_S  == )
    ast << endl;
    cpp << ";\n";
    ASTStatements(stmts->next);
}

void parse(node *gdecl, node *fdecl, node *maindecl)
{

    ast << "\n\nPARSING STARTED\n";
    // cpp << "\n\n---CPP CODE---\n\n";

    node *iterate = gdecl;

    ast << "\n\n---GLOBAL DECL---\n\n";

    cpp << "#include<iostream>\nusing namespace std;\n";

    while (iterate != NULL)
    {
        ast << "DECL ";
        int datatype;

        if (iterate->datatype == "integer")
        {
            ast << "INT ";
            cpp << "int ";
            datatype = 1;
        }
        else if (iterate->datatype == "bool")
        {
            ast << "BOOL ";
            cpp << "bool ";
            datatype = 2;
        }

        node *vars = iterate->right;

        bool fun = false;

        while (vars)
        {
            // ast << vars->type << " " << vars->size_ << endl;
            if (vars->type == 6)
            { // array of variable"PARSING STARTED\n"

                if (vars->size_ == -1)
                { // varaibles
                    // ast << "VAR " << vars->varname << " ";
                    // if (symboltable.find(vars->varname) != symboltable.end())
                    // {
                    //     err << "REDECL VAR" << vars->varname << endl;
                    //     flag = true;
                    // }
                    // symboltable[vars->varname] = new info(vars->varname, 1, -1);
                    symboltable[vars->varname] = new info(vars->varname, 1, -1, datatype);
                    ast << " VAR "
                        << "";
                    cpp << vars->varname << " ";
                    // cout << vars->type << endl;
                }
                else
                { // array
                    // ast << "ARR VAR " << vars->varname << " " << vars->size_ << " ";
                    // if (symboltable.find(vars->varname) != symboltable.end())
                    // {
                    //     err << "REDECL ARREF " << vars->varname << endl;
                    //     flag = true;
                    // }
                    // symboltable[vars->varname] = new info(vars->varname, 2, vars->size_);
                    symboltable[vars->varname] = new info(vars->varname, 2, vars->size_, datatype);
                    ast << " ARR VAR " << vars->size_ << " ";
                    cpp << vars->varname << "[" << vars->size_ << "] ";
                }
            }

            if (vars->type == 2)
            {                                            // function declaration
                ast << " FUN " << vars->datatype << " "; // name
                string fname = vars->datatype;
                symboltable[fname] = new info(vars->datatype, 3, -1, datatype);

                cpp << vars->datatype << "(";
                // ast << " FUN ";

                node *args = vars->farglist;

                while (args)
                {

                    // ast << args->datatype << " "; // datatype
                    int datatypedecl;

                    string d = "";

                    if (args->datatype == "integer")
                    {
                        datatypedecl = 1;
                        ast << "INT ";
                        d = "int ";
                    }
                    if (args->datatype == "bool")
                    {
                        datatypedecl = 2;
                        ast << "BOOL ";
                        d = "bool ";
                    }

                    // ast << args->type << " "; // datatype

                    node *v = args->next;

                    // ast << args->datatype << endl;

                    while (v)
                    {
                        symboltable[fname]->symboltable[v->datatype] = new info(v->datatype, 1, -1, datatypedecl);
                        cpp << d << " " << v->datatype;

                        ast << v->datatype << " "; // varnames
                        v = v->next;
                        // cpp << d;
                        if (v)
                        {
                            cpp << ",";
                        }
                    }
                    cpp << ")";
                    args = args->nextt;
                    if (args)
                    {
                        cpp << ",";
                    }
                }
            }

            vars = vars->next;
            if (vars)
            {
                cpp << ",";
            }
            else
            {
                cpp << ";\n";
            }
        }

        iterate = iterate->next;
        ast << endl;
    }

    ast << "\n\n---FUNCTION DECL---\n\n";

    iterate = fdecl;

    while (iterate)
    {

        ast << "FUN ";

        ast << iterate->fname->datatype << endl;

        cpp << "int ";

        cpp << iterate->fname->datatype << " (";

        node *args = iterate->farglist;

        while (args)
        {

            // ast << args->datatype << " "; // datatype

            string d = "";

            if (args->datatype == "integer")
            {
                // cpp << "int ";
                d = "int ";
                ast << "INT ";
            }
            if (args->datatype == "bool")
            {
                // cpp << "bool ";
                d = "bool ";
                ast << "BOOL ";
            }

            // ast << args->type << " "; // datatype

            node *v = args->next;

            // ast << args->datatype << endl;

            while (v)
            {
                cpp << d;
                ast << v->datatype << " "; // varnames
                cpp << v->datatype;
                v = v->next;
                if (v)
                {
                    cpp << ",";
                }
            }

            cpp << ") {\n";

            args = args->nextt;
        }

        node *localdecl = iterate->flocaldict;
        ast << '\n';

        while (localdecl)
        {
            ast << "LDECL ";

            if (localdecl->datatype == "integer")
            {
                ast << "INT ";
                cpp << "int ";
            }
            if (localdecl->datatype == "bool")
            {
                ast << "BOOL ";
                cpp << "bool ";
            }

            node *varptr = localdecl->next;

            while (varptr)
            {

                // ast << varptr->varname <<  " ";
                ast << " VAR "
                    << " ";

                cpp << varptr->varname;

                varptr = varptr->next;
                if (varptr)
                {
                    cpp << ",";
                }
            }
            cpp << ";\n";

            localdecl = localdecl->nextt;
            ast << '\n';
        }

        node *stmts = iterate->stmtlist;

        ASTStatements(stmts);

        ast << "RETURN ";
        cpp << " return ";

        printExpr(iterate->fretstmt);

        cpp << ";\n}\n";

        iterate = iterate->next;
        ast << '\n';
    }

    ast << "\n\n---MAIN BLOCK---\n\n";

    iterate = maindecl;

    ast << "FUN ";
    if (iterate->fret->datatype == "integer")
    {
        cpp << "int main() { \n";
        ast << "INT MAIN ";
    }
    ast << endl;

    /*     Local Declaration       */

    node *localdecl = iterate->flocaldict;
    ast << '\n';

    while (localdecl)
    {
        ast << "LDECL ";

        if (localdecl->datatype == "integer")
        {
            ast << "INT ";
            cpp << "int ";
        }
        if (localdecl->datatype == "bool")
        {
            ast << "BOOL ";
            cpp << "bool ";
        }

        node *varptr = localdecl->next;

        while (varptr)
        {

            // ast << varptr->varname <<  " ";
            cpp << varptr->varname;
            ast << " VAR "
                << " ";

            varptr = varptr->next;
            if (varptr)
            {
                cpp << ",";
            }
        }
        cpp << ";\n";

        localdecl = localdecl->nextt;
        ast << '\n';
    }

    node *stmts = iterate->stmtlist;
    ASTStatements(stmts);

    ast << "RETURN ";
    cpp << "return ";

    printExpr(iterate->fretstmt);

    cpp << ";\n}\n";

    iterate = iterate->next;
    ast << '\n';

    if (flag)
    {
        cout << err.str() << endl;
        exit(0);
    }

    cout << ast.str() << endl;

    cout << cpp.str() << endl;

    string s = (string)filename;

    if (s == "test")
    {
        std::ofstream file("./"
                           "test.ast",
                           std::ios::trunc);
        if (file.is_open())
        {
            file << ast.str();
            file.close();
        }

        std::ofstream file1("./"
                            "test.cpp",
                            std::ios::trunc);
        if (file1.is_open())
        {
            file1 << cpp.str();
            file1.close();
        }
        return;
    }

    string ast_file = filename;
    s += ".cpp";
    ast_file += ".ast";

    // cout << s << endl;

    string tempcpp = cpp.str();

    writeToFile(s, tempcpp);

    tempcpp = ast.str();

    writeToFileAST(ast_file, tempcpp);
}

/*
507 --> assingment
512 --> while
511 --> if else
*/
