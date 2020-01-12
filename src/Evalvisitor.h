#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Python3BaseVisitor.h"
#include "Rec.h"
#include "bigint.h"
#include <bits/stdc++.h>
using namespace std;

//vector<Rec> Q;
//return Q;

enum Command {Nono,Return,Break,Continue};
Command Conditon = Nono;
map<string,Rec> AllR[2003];
map<string,Rec> Funcset[2003]; //record value(in the form of Rec)
map<string , int> ftolis={
        {"int",1},
        {"float",2},
        {"str",3},
        {"bool",4}
};   //set a Func to int relations in order to catch Lis[]
vector<string> Funcname[2003]; //record Funcs
int depth = 0;
int cntfunc = 4;  //record numbers of Func and have four convertional funcs
#define debug cout<<"bug"<<endl;  //debug!!!!!debug!!!!!
int constfunc=0,notin[2003];
Python3Parser::SuiteContext *Lis[2003];
int Stack[2003],top=0;  //stack!!!!!

class EvalVisitor: public Python3BaseVisitor {
    virtual antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override{
        for(const auto i : ctx->stmt())
            visitStmt(i);
        vector<Rec> Q;
        return Q;
    }
    virtual antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
        string Name = ctx->NAME()->getText();
        ftolis[Name]=++cntfunc;  //func numbers plus
        Lis[cntfunc]=ctx->suite();  //tie the Lis to suite
        return visitParameters(ctx->parameters());
    }
    virtual antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
        if(ctx->typedargslist()) return visitTypedargslist(ctx->typedargslist());
        vector<Rec> Q;
        return Q;
    }
    virtual antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
        //Though it is a key part,I ignore its importance,resulting in many bugs.
        int nowcntfunc=cntfunc;
        //from back to front
        int i=0,j=0; //i->tppedef  j->test
        for(;j<ctx->test().size();++i,++j){
            string now = visitTfpdef(ctx->tfpdef()[ctx->tfpdef().size()-i-1]);
            vector<Rec> tmp = visitTest(ctx->test()[ctx->test().size()-j-1]);
            Funcset[nowcntfunc][now] = tmp[0];
            Funcname[nowcntfunc].push_back(now);
        }
        for(;i<ctx->tfpdef().size();++i){
            string now = visitTfpdef(ctx->tfpdef()[ctx->tfpdef().size()-i-1]);
            Rec tmp;
            Funcset[nowcntfunc][now] = tmp;
            Funcname[nowcntfunc].push_back(now);
        }
        for(int k=0;k<Funcname[nowcntfunc].size()/2;k++)
            swap(Funcname[nowcntfunc][k],Funcname[nowcntfunc][Funcname[nowcntfunc].size()-1-k]);
        vector<Rec> Q;
        return Q;
    }
    virtual antlrcpp::Any visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
        return ctx->NAME()->getText();
    }
    virtual antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override {
        //debug
        if(ctx->simple_stmt()) return visit(ctx->simple_stmt());
        if(ctx->compound_stmt()) return visitCompound_stmt(ctx->compound_stmt());
    }
    virtual antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
        return visitSmall_stmt(ctx->small_stmt());
    }
    virtual antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
        if(ctx->expr_stmt()) return visitExpr_stmt(ctx->expr_stmt());
        if(ctx->flow_stmt()) return visitFlow_stmt(ctx->flow_stmt());
    }
    virtual antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
        vector<Rec> tmp = visitTestlist(ctx->testlist()[ctx->testlist().size() - 1]).as<vector<Rec>>(); // back to forth
        int nowdepth=depth;
        if (ctx->augassign()){
            string op = ctx->augassign()->getText();
            for (unsigned int i = 0; i < tmp.size(); ++i) {
                string now = ctx->testlist()[0]->test()[i]->getText();
                //cout<<now<<endl;
                int tot = AllR[nowdepth].count(now) ? nowdepth : 0;  // determine global or limited
                if (op == "+=") AllR[tot][now] += tmp[i];
                else if (op == "-=") AllR[tot][now] -= tmp[i];
                else if (op == "*=") AllR[tot][now] *= tmp[i];
                else if (op == "/=") AllR[tot][now] /= tmp[i];
                else if (op == "//=") AllR[tot][now] = ZC(AllR[tot][now], tmp[i]);
                else AllR[tot][now] %= tmp[i];
            }
        }else {
            //debug
            for (int k = ctx->testlist().size() - 2; k >= 0; k--) {
                for (int i = 0; i < tmp.size(); i++) {
                    string now = ctx->testlist()[k]->test()[i]->getText();
                    //cout<<now<<endl;
                    int tot = AllR[nowdepth].count(now) ? nowdepth : 0;
                    AllR[tot][now] = tmp[i];
                }
            }
        }
        vector<Rec> Q;
        return Q;
    }
    virtual antlrcpp::Any visitAugassign(Python3Parser::AugassignContext *ctx) override {
        vector<Rec> Q;
        return Q;
    }
    virtual antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
        if(ctx->break_stmt()) return visitBreak_stmt(ctx->break_stmt());
        if(ctx->continue_stmt()) return visitContinue_stmt(ctx->continue_stmt());
        if(ctx->return_stmt()) return visitReturn_stmt(ctx->return_stmt());
    }
    virtual antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
        Conditon=Break;
        vector<Rec> Q;
        return Q;
    }
    virtual antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
        Conditon=Continue;
        vector<Rec> Q;
        return Q;
    }
    virtual antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
        int funct=constfunc;
        if(ctx->testlist()) {
            vector<Rec> tmp = visitTestlist(ctx->testlist()).as<vector<Rec>>();
            Conditon=Return;
            //return tmp;
            return tmp;
        }
        vector<Rec> Q;
        return Q;
    }
    virtual antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
        if(ctx->if_stmt()) return visitIf_stmt(ctx->if_stmt());
        if(ctx->while_stmt()) return visitWhile_stmt(ctx->while_stmt());
        if(ctx->funcdef()) return visitFuncdef(ctx->funcdef());
    }
    virtual antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
        vector<Rec> tmp = visitTest(ctx->test()[0]);
        int pos = 0;
        tmp[0] = tmp[0].transbool();
        while (!tmp[0].getbool() && pos + 1 < ctx->test().size()) {
            pos++;
            vector<Rec> T = visitTest(ctx->test()[pos]);
            T[0] = T[0].transbool();
            tmp = T;
        }
        if (tmp[0].getbool()) {
            vector<Rec> T = visitSuite(ctx->suite()[pos]);
            return T;
        } else if (pos + 1 < ctx->suite().size()) {
            vector<Rec> T = visitSuite(ctx->suite()[pos + 1]);
            return T;
        }
        vector<Rec> Q;
        return Q;
    }
    virtual antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
        vector<Rec> tmp = visitTest(ctx->test());
        tmp[0] = tmp[0].transbool();
        //cout<<tmp.getbool()<<endl;
        while (tmp[0].getbool()) {
            vector<Rec> T = visitSuite(ctx->suite());
            //cout<<ctx->suite()->getText()<<endl;
            if (Conditon == Break) {
                Conditon = Nono;
                break;
            } else if (Conditon == Continue)
                Conditon = Nono;
            else if (Conditon == Return)
                return T;
            vector<Rec> tmp1 = visitTest(ctx->test());
            tmp1[0] = tmp1[0].transbool();
            tmp = tmp1;
        }
        vector<Rec> Q;
        return Q;
    }
    virtual antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override {
        //debug
        if(ctx->simple_stmt()){
            //debug
            vector<Rec> tmp=visitSimple_stmt(ctx->simple_stmt());
            if(Conditon==Return) return tmp;
        }else{
            for (unsigned int i = 0; i < ctx->stmt().size(); i++) {
                vector<Rec> tmp = visitStmt(ctx->stmt()[i]);
                if(Conditon==Break||Conditon==Continue) break;
                if(Conditon==Return) return tmp;
            }
        }
        //debug
        vector<Rec> Q;
        return Q;
    }
    virtual antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override {
        //cerr<<"bug"<<endl;
        return visitOr_test(ctx->or_test());
    }
    virtual antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override {
        const auto tmp= ctx->and_test();
        vector<Rec> Q;
        if((signed)tmp.size()==1)
            return visitAnd_test(tmp[0]);
        for(auto i:tmp){
            vector<Rec> T=visitAnd_test(i);
            if(T[0].transbool().getbool()) {
               Q.push_back( Rec(bool(true)));
               return Q;
            }
        }
        Q.push_back( Rec(bool(false)));
        return Q;
    }
    virtual antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override {
        const auto tmp= ctx->not_test();
        vector<Rec> Q;
        if((signed)tmp.size()==1)
            return visitNot_test(tmp[0]);
        for(auto i:tmp){
            vector<Rec> T=visitNot_test(i);
            if(!T[0].transbool().getbool()) {
                Q.push_back(Rec(bool(false)));
                return Q;
            }
        }
        Q.push_back( Rec(bool(true)));
        return Q;
    }
    virtual antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override {
        //cerr<<"bug"<<endl;
        if (ctx->comparison())
            return visitComparison(ctx->comparison());
        vector<Rec> Q;
        vector<Rec> tmp=visitNot_test(ctx->not_test());
        if(tmp[0].transbool().getbool()){
            Q.push_back(Rec(bool(false)));
            return Q;
        }
        Q.push_back( Rec(bool(true)));
        return Q;
    }
    virtual antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override {
        auto tmp = ctx->comp_op();
        vector<Rec> Q;
        if (tmp.empty())
            return visitArith_expr(ctx->arith_expr()[0]);
        vector<Rec> now = visitArith_expr(ctx->arith_expr()[0]);
        for (int i = 0; i < tmp.size(); ++i) {
            auto op = tmp[i]->getText();
            vector<Rec> news = visitArith_expr(ctx->arith_expr()[i + 1]);
            bool flag = false;
            if (op == "<") {
                if (now[0] < news[0]) flag = true;
            } else if (op == ">") {
                if (now[0] > news[0]) flag = true;
            } else if (op == "==") {
                if (now[0] == news[0]) flag = true;
            } else if (op == ">=") {
                if (now[0] >= news[0]) flag = true;
            } else if (op == "<=") {
                if (now[0] <= news[0]) flag = true;
            } else {
                if (now[0] != news[0]) flag = true;
            }
            if (!flag) {
                Q.push_back(Rec(bool(false)));
                return Q;
            }
            now[0] = news[0];
        }
        Q.push_back(Rec(bool(true)));
        return Q;
    }
    virtual antlrcpp::Any visitComp_op(Python3Parser::Comp_opContext *ctx) override {
        vector<Rec> Q;
        Q.push_back(Rec(bool(true)));
        return Q;
    }
    virtual antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
        vector<Rec> T = visitTerm(ctx->term()[0]);
        string now = ctx->getText();
        now.erase(0, ctx->term()[0]->getText().length());
        for (unsigned int i = 1; i < ctx->term().size(); i++) {
            vector<Rec> tmp = visitTerm(ctx->term()[i]);
            if (now[0] == '+')
                T[0] += tmp[0];
            else
                T[0] -= tmp[0];
            now.erase(0, 1 + ctx->term()[i]->getText().length());
        }
        return T;
    }
    virtual antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override {
        vector<Rec> T = visitFactor(ctx->factor()[0]);
        string now=ctx->getText();
        now.erase(0,ctx->factor()[0]->getText().length());
        for(int i=1;i<ctx->factor().size();i++){
            int ext=1;
            vector<Rec> tmp=visitFactor(ctx->factor()[i]);
            if(now[0]=='*'){
                T[0]*=tmp[0];
            }else if(now[0]=='%'){
                T[0]%=tmp[0];
            }else if(now[0]=='/'&&now[1]!='/'){
                T[0]/=tmp[0];
            }else if(now[0]=='/'&&now[1]=='/'){
                ext++;
                T[0]=ZC(T[0],tmp[0]);
            }
            now.erase(0,ext+ctx->factor()[i]->getText().length());
        }
        return T;
    }
    virtual antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override {
        if(ctx->factor()){
            string now=ctx->getText();
            if(now[0] == '+') return visitFactor(ctx->factor());
            vector<Rec> T=visitFactor(ctx->factor());
            T[0]=-T[0];
            return T;
        }
        return visitAtom_expr(ctx->atom_expr());
    }
    virtual antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
        if (!ctx->trailer()) {
            vector<Rec> Q;
            Q.push_back(visitAtom(ctx->atom()));
            return Q;
        }

        string Text = ctx->atom()->getText();
        if (Text == "print") {
            if (ctx->trailer()->arglist()) {
                const auto Arguments = ctx->trailer()->arglist()->argument();
                for (int i = 0; i < Arguments.size(); ++i) {
                    vector<Rec> data = visitTest(Arguments[i]->test());
                    data[0].toprint();
                    if ((i + 1) != Arguments.size()) cout << " ";
                }
            }
            cout << endl;
            vector<Rec> Q;
            return Q;
        } //out is special

        if (ctx->atom()->NAME()) {
            int funct = constfunc = ftolis[ctx->atom()->NAME()->getText()];  //catch the number of a NAME
            Stack[++top] = funct;  // push into a stack
            if (funct <= 4) {   // transfuntion is special
                Rec tmp = visitTrailer(ctx->trailer());
                constfunc = Stack[--top];
                vector<Rec> Q;
                Q.push_back(tmp);
                return Q;
            } else {
                int nowdepth = ++depth;
                notin[nowdepth] = notin[nowdepth - 1] + 1;
                for (int i = 0; i < Funcname[funct].size(); ++i) {
                    auto p = Funcname[funct][i];
                    if (Funcset[funct][p].gettype() != Non) AllR[nowdepth][p] = Funcset[funct][p];
                }
                visitTrailer(ctx->trailer());
                notin[nowdepth] = 0;
                vector<Rec> tmp = visitSuite(Lis[funct]);
                AllR[depth].clear();  //clear current depth
                Conditon = Nono;
                depth--;
                constfunc = Stack[--top];
                return tmp;
            }
        }
        vector<Rec> Q;
        return Q;
    }
    virtual antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override {
        if(ctx->arglist())
            return visitArglist(ctx->arglist());
        return Rec(Non);
    }
    virtual antlrcpp::Any visitAtom(Python3Parser::AtomContext *ctx) override {
        string Text = ctx->getText();
        if(ctx->NAME()){
            int nowdepth=depth,qwq=notin[depth];
            if(AllR[nowdepth-qwq].count(ctx->NAME()->getText()))   // global or not
                return AllR[nowdepth-qwq][ctx->NAME()->getText()];
            else return AllR[0][ctx->NAME()->getText()];
        }else if(ctx->test()){
            vector<Rec> T=visitTest(ctx->test());
            return T[0];
        }else if(ctx->NUMBER()){
            int flag = 0;
            for (int i = 0; i < Text.size(); ++i) {
                if (Text[i] == '.') {
                    flag = 1;
                    break;
                }
            }
            if (flag == 1) {
                double a = 0;
                int i = 0;
                while (Text[i] != '.') {
                    a = a * 10 + (Text[i] - '0');
                    ++i;
                }
                double dot = 0.1;
                for (int j = i + 1; j < Text.length(); ++j) {
                    a = a + dot * (Text[j] - '0');
                    dot *= 0.1;
                }
                return Rec(a);
            }else return Rec(bigint(Text));
        }else if(ctx->STRING().size()){
            string tmp = "";
            for (auto i: ctx->STRING()) {
                string a = i->getText();
                tmp = tmp + a.substr(1, a.length() - 2);
            }
            return Rec(tmp);
        }else if(Text == "True") return Rec(true);
        else if(Text == "False") return Rec(false);
        else return Rec(Non);
    }
    virtual antlrcpp::Any visitTestlist(Python3Parser::TestlistContext *ctx) override {
        auto tmp=ctx->test();
        vector<Rec> T;
        for(auto i:tmp) {
            vector<Rec> Q =visitTest(i);
            for(unsigned int j=0;j<Q.size();++j)
                T.push_back(Q[j]);
        }
        return T;
    }
    virtual antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override {
        //debug
        int funct = constfunc, nowdepth = depth;
        Rec tmp = visitArgument(ctx->argument()[0]).as<Rec>();
        if (funct > 4 && !ctx->argument()[0]->NAME())
            AllR[nowdepth][Funcname[funct][0]] = tmp;
        for (int i = 1; i < ctx->argument().size(); ++i) {
            tmp = visitArgument(ctx->argument()[i]).as<Rec>();
            if (funct > 4 && !ctx->argument()[i]->NAME())
                AllR[nowdepth][Funcname[funct][i]] = tmp;
        }
        if (funct == 1) tmp = tmp.transint();
        else if (funct == 2)tmp = tmp.transdouble();
        else if (funct == 3)tmp = tmp.transstr();
        else if (funct == 4)tmp = tmp.transbool();
        return tmp;
    }
    virtual antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override {
        Rec tmp;
        int nowdepth=depth;
        vector<Rec> Q=visitTest((ctx->test()));
        if(ctx->NAME()) tmp= AllR[nowdepth][ctx->NAME()->getText()]=Q[0];
        else tmp=Q[0];
        return tmp;
    }
};


#endif //PYTHON_INTERPRETER_EVALVISITOR_H
