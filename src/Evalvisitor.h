#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H


#include "Python3BaseVisitor.h"
#include "Rec.h"
#include "bigint.h"
#include <bits/stdc++.h>
using namespace std;

enum Command{Nono,Return,Break,Continue};
Command Conditon=Nono;
map<string,Rec> AllR[2003];
map<string,Rec> Funcset[2003]; //record value(in the form of Rec)
map<string , int>ftolis;   //set a Func to int relations in order to catch Lis[]
vector<string> Funcname[2003]; //record Funcs
int depth=0;
int cntfunc=4;  //record numbers of NAME
#define debug cout<<"bug"<<endl;
int constfunc=0,notin[2003];
Python3Parser::SuiteContext *Lis[2003];
int Stack[2003],top=0;  //stack!!!!!

class EvalVisitor: public Python3BaseVisitor {
    virtual antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override{
        ftolis["int"]=1;
        ftolis["str"]=3;
        ftolis["float"]=2;
        ftolis["bool"]=4;
        for(const auto i : ctx->stmt())
            visitStmt(i);
        return Rec(Non);
    }
    virtual antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
        string Name = ctx->NAME()->getText();
        ftolis[Name]=++cntfunc;
        Lis[cntfunc]=ctx->suite();
        return visitParameters(ctx->parameters());
    }
    virtual antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
        if(ctx->typedargslist()) return visitTypedargslist(ctx->typedargslist());
        return Rec(Non);
    }
    virtual antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
        //Though it is a key part,I ignore its importance,resulting in many bugs.
        int nowcntfunc=cntfunc;
        //from back to front
        int i=0,j=0; //i->tppedef  j->test
        for(;j<ctx->test().size();++i,++j){
            string now = visitTfpdef(ctx->tfpdef()[ctx->tfpdef().size()-i-1]);
            Rec tmp = visitTest(ctx->test()[ctx->test().size()-j-1]).as<Rec>();
            Funcset[nowcntfunc][now] = tmp;
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
        return Rec(Non);
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
        return Rec(Non);
    }
    virtual antlrcpp::Any visitAugassign(Python3Parser::AugassignContext *ctx) override {
        return Rec(Non);
    }
    virtual antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
        if(ctx->break_stmt()) return visitBreak_stmt(ctx->break_stmt());
        if(ctx->continue_stmt()) return visitContinue_stmt(ctx->continue_stmt());
        if(ctx->return_stmt()) return visitReturn_stmt(ctx->return_stmt());
    }
    virtual antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
        Conditon=Break;
        return Rec(Non);
    }
    virtual antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
        Conditon=Continue;
        return Rec(Non);
    }
    virtual antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
        int funct=constfunc;
        Conditon=Return;
        if(ctx->testlist()) {
            vector<Rec> tmp = visitTestlist(ctx->testlist()).as<vector<Rec>>();
            return tmp;
        }
        return Rec(Non);
    }
    virtual antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
        if(ctx->if_stmt()) return visitIf_stmt(ctx->if_stmt());
        if(ctx->while_stmt()) return visitWhile_stmt(ctx->while_stmt());
        if(ctx->funcdef()) return visitFuncdef(ctx->funcdef());
    }
    virtual antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
        //to do
    }
    virtual antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
        //to do
    }
    virtual antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override {
        //debug
        if(ctx->simple_stmt()){
            //debug
            Rec tmp=visitSimple_stmt(ctx->simple_stmt()).as<Rec>();
            if(Conditon==Return) return tmp;
        }else{
            for (unsigned int i = 0; i < ctx->stmt().size(); i++) {
                Rec tmp = visitStmt(ctx->stmt()[i]).as<Rec>();
                if(Conditon==Break||Conditon==Continue) break;
                if(Conditon==Return) return tmp;
            }
        }
        //debug
        return Rec(Non);
    }
    virtual antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override {
        //cerr<<"bug"<<endl;
        return visitOr_test(ctx->or_test());
    }
    virtual antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override {
        const auto tmp= ctx->and_test();
        if((signed)tmp.size()==1)
            return visitAnd_test(tmp[0]);
        for(auto i:tmp){
            Rec T=visitAnd_test(i).as<Rec>();
            if(T.transbool().getbool()) return Rec(bool(true));
        }
        return Rec(bool(false));
    }
    virtual antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override {
        const auto tmp= ctx->not_test();
        if((signed)tmp.size()==1)
            return visitNot_test(tmp[0]);
        for(auto i:tmp){
            Rec T=visitNot_test(i).as<Rec>();
            if(!T.transbool().getbool()) return Rec(bool(false));
        }
        return Rec(bool(true));
    }
    virtual antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override {
        //cerr<<"bug"<<endl;
        if(ctx->comparison())
            return visitComparison(ctx->comparison());
        return Rec(bool(!visitNot_test(ctx->not_test()).as<Rec>().transbool().getbool()));
    }
    virtual antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override {
        auto tmp=ctx->comp_op();
        if(tmp.empty())
            return visitArith_expr(ctx->arith_expr()[0]);
        for(int i=0;i<tmp.size();++i){
            auto op=tmp[i]->getText();
            Rec left=visitArith_expr(ctx->arith_expr()[i]).as<Rec>();
            Rec right=visitArith_expr(ctx->arith_expr()[i+1]).as<Rec>();
            bool flag= false;
            if(op=="<") {
                if (left < right) flag = true;
            } else if(op==">"){
                if(left>right) flag = true;
            } else if(op=="=="){
                if(left==right) flag=true;
            } else if(op==">="){
                if(left>=right) flag=true;
            } else if(op=="<="){
                if(left<=right) flag=true;
            } else{
                if(left!=right) flag=true;
            }
            if(!flag) return Rec(bool(false));
        }
        return Rec(bool(true));
    }
    virtual antlrcpp::Any visitComp_op(Python3Parser::Comp_opContext *ctx) override {
        return ctx;
    }
    virtual antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
        Rec T = visitTerm(ctx->term()[0]).as<Rec>();
        string now = ctx->getText();
        now.erase(0, ctx->term()[0]->getText().length());
        for (unsigned int i = 1; i < ctx->term().size(); i++) {
            Rec tmp = visitTerm(ctx->term()[i]).as<Rec>();
            if (now[0] == '+')
                T += tmp;
             else
                T -= tmp;
            now.erase(0, 1 + ctx->term()[i]->getText().length());
        }
        return T;
    }
    virtual antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override {
        Rec T = visitFactor(ctx->factor()[0]).as<Rec>();
        string now=ctx->getText();
        now.erase(0,ctx->factor()[0]->getText().length());
        for(int i=1;i<ctx->factor().size();i++){
            int ext=1;
            Rec tmp=visitFactor(ctx->factor()[i]).as<Rec>();
            if(now[0]=='*'){
                T*=tmp;
            }else if(now[0]=='%'){
                T%=tmp;
            }else if(now[0]=='/'&&now[1]!='/'){
                T/=tmp;
            }else if(now[0]=='/'&&now[1]=='/'){
                ext++;
                T=ZC(T,tmp);
            }
            now.erase(0,ext+ctx->factor()[i]->getText().length());
        }
        return T;
    }
    virtual antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override {
        if(ctx->factor()){
            string now=ctx->getText();
            if(now[0] == '+') return visitFactor(ctx->factor());
            return -(visitFactor(ctx->factor()).as<Rec>());
        }
        return visitAtom_expr(ctx->atom_expr());
    }
    virtual antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
        if (!ctx->trailer()) return visitAtom(ctx->atom());

        string Text = ctx->atom()->getText();
        if (Text == "print") {
            if (ctx->trailer()->arglist()) {
                const auto Arguments = ctx->trailer()->arglist()->argument();
                for (int i = 0; i < Arguments.size(); ++i) {
                    auto data = visitTest(Arguments[i]->test()).as<Rec>();
                    data.toprint();
                    if ((i + 1) != Arguments.size()) cout << " ";
                }
            }
            cout << endl;
            return Rec(Non);
        } //out is special

        if (ctx->atom()->NAME()) {
            int funct = constfunc = ftolis[ctx->atom()->NAME()->getText()];  //catch the number of a NAME
            Stack[++top] = funct;  // push into a stack
            if (funct <= 4) {   // transfuntion is special
                Rec tmp = visitTrailer(ctx->trailer()).as<Rec>();
                constfunc = Stack[--top];
                return tmp;
            } else {
                int nowdepth = ++depth;
                notin[nowdepth] = notin[nowdepth - 1] + 1;
                for (int i = 0; i < Funcname[funct].size(); ++i) {
                    auto p = Funcname[funct][i];
                    if (Funcset[funct][p].gettype() != Non) AllR[nowdepth][p] = Funcset[funct][p];
                }
                visitTrailer(ctx->trailer());
                notin[nowdepth] = 0;
                Rec tmp = visitSuite(Lis[funct]).as<Rec>();
                AllR[depth].clear();  //clear current depth
                Conditon = Nono;
                depth--;
                constfunc = Stack[--top];
                return tmp;
            }
        }
        return Rec(Non);
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
            return visit(ctx->test());
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
        for(auto i:tmp)
            T.push_back(visitTest(i).as<Rec>());
        return T;
    }
    virtual antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override {
        //debug
        int funct=constfunc,nowdepth=depth;
        Rec tmp=visitArgument(ctx->argument()[0]).as<Rec>();
        if(funct>4&&!ctx->argument()[0]->NAME())
            AllR[nowdepth][Funcname[funct][0]] = tmp;
        for(int i=1;i<ctx->argument().size();++i){
            tmp=visitArgument(ctx->argument()[i]).as<Rec>();
            if(funct>4&&!ctx->argument()[i]->NAME())
                AllR[nowdepth][Funcname[funct][i]]=tmp;
        }
        if(funct==1) tmp=tmp.transint();
        else if(funct==2)tmp=tmp.transdouble();
        else if(funct==3)tmp=tmp.transstr();
        else if(funct==4)tmp=tmp.transbool();
        return tmp;
    }
    virtual antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override {
        Rec tmp;
        int nowdepth=depth;
        if(ctx->NAME()) tmp= AllR[nowdepth][ctx->NAME()->getText()]=visitTest((ctx->test())).as<Rec>();
        else tmp =visitTest(ctx->test()).as<Rec>();
        return tmp;
    }
};


#endif //PYTHON_INTERPRETER_EVALVISITOR_H
