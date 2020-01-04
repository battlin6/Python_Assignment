//
// Created by battlin on 2019/12/6.
//

#ifndef PYTHON_INTERPRETER_REC_H
#define PYTHON_INTERPRETER_REC_H

#include <bits/stdc++.h>

#include <utility>
#include "bigint.h"

using namespace std;

enum type{Int,Double,Str,Bool,Non}; //def type

class Rec{
private:
    type Type;
    bool vbool;
    bigint vint;
    string vstring;
    double vdouble;
    static void qiangzhi(Rec &a,Rec &b){
        if(a.Type==b.Type) return;
        if(a.Type==Bool&&b.Type==Int) a=a.transint();
        if(b.Type==Bool&&a.Type==Int) b=b.transint();
        if(a.Type==Bool&&b.Type==Double) a=a.transdouble();
        if(b.Type==Bool&&a.Type==Double) b=b.transdouble();
        if(a.Type==Int&&b.Type==Double) a=a.transdouble();
        if(b.Type==Int&&a.Type==Double) b=b.transdouble();
        if(a.Type==Non&&b.Type==Int) a=a.transint();
        if(b.Type==Non&&a.Type==Int) b=b.transint();
        if(a.Type==Non&&b.Type==Double) a=a.transdouble();
        if(b.Type==Non&&a.Type==Double) b=b.transdouble();
        if(a.Type==Non&&b.Type==Bool) a=a.transbool();
        if(b.Type==Non&&a.Type==Bool) b=b.transbool();
    }
public:
    bool getbool(){
        return vbool;
    }
    string getstring(){
        return vstring;
    }
    bigint getint(){
        return vint;
    }
    double getdouble(){
        return vdouble;
    }
    type gettype (){
        return Type;
    }

    Rec(){
        vstring="";
        vdouble=0;
        vbool=false;
        vint=0;
        Type=Non;
    }
    explicit Rec (const type & t){
        vstring="";
        vdouble=0;
        vbool=false;
        vint=0;
        Type=t;
    }
    explicit Rec (const bigint& b){
        vstring="";
        vdouble=0;
        vbool=false;
        vint=b;
        Type=Int;
    }
    explicit Rec (const int& b){
        Type=Int;
        vstring="";
        vdouble=0;
        vbool=false;
        vint=bigint(b);
    }
    explicit Rec (const double& d){
        Type=Double;
        vstring="";
        vdouble=d;
        vbool=false;
        vint=0;
    }
    explicit Rec (const string &s){
        vstring=s;
        vdouble=0;
        vbool=false;
        vint=0;
        Type=Str;
    }
    explicit Rec (const bool &b){
        vstring="";
        vdouble=0;
        vbool=b;
        vint=0;
        Type=Bool;
    }
    explicit Rec (const bigint& b,type T){
        vstring="";
        vdouble=0;
        vbool=false;
        vint=b;
        Type=T;
    }
    explicit Rec (const int& b,type T){
        Type=T;
        vstring="";
        vdouble=0;
        vbool=false;
        vint=bigint(b);
    }
    explicit Rec (const double& d,type T){
        Type=T;
        vstring="";
        vdouble=d;
        vbool=false;
        vint=0;
    }
    explicit Rec (const string &s,type T){
        vstring=s;
        vdouble=0;
        vbool=false;
        vint=0;
        Type=T;
    }
    explicit Rec (const bool &b,type T){
        vstring="";
        vdouble=0;
        vbool=b;
        vint=0;
        Type=T;
    }

    Rec transint() {
        Rec tmp(Int);
        if (Type == Int) return *this;
        else if (Type == Double) {
            int x = int(vdouble);
            //cout<<x<<endl;
            bigint T(x);
            tmp.vint = T;
            return tmp;
        } else if (Type == Bool) {
            if (vbool) tmp.vint = 1;
            else tmp.vint = 0;
            return tmp;
        } else if (Type == Str) {
            bigint T(vstring);
            tmp.vint = T;
            return tmp;
        }
    }
    Rec transdouble(){     //In this part I cannot be sure that if Type==Str how to trans to double may be it is invalid
        Rec tmp(Double);
        if(Type==Double) return *this;
        else if(Type==Int){
             int x=Toint(vint);
             tmp.vdouble=double(x);
             return tmp;
        }
        else if(Type==Str){
            bigint T(vstring);
            int x=Toint(T);
            tmp.vdouble=double(x);
            return tmp;
        }
        else if(Type==Bool){
            if(vbool) tmp.vdouble=double(1);
            else tmp.vdouble=double(0);
            return tmp;
        }
    }
    Rec transstr(){
        Rec tmp(Str);
        if(Type==Str) return *this;
        else if(Type==Int){
            tmp.vstring=vint.getstring();
            if(vint.getsignal()==0){
                string T="-"+tmp.vstring;
                tmp.vstring = T;
            }
            return tmp;
        }
        else if(Type==Double){
            tmp.vstring=to_string(vdouble );
            return tmp;
        }
        else if(Type==Bool){
            if(vbool)tmp.vstring="True";
            else tmp.vstring="False";
        }
    }
    Rec transbool(){
        Rec tmp(Bool);
        if(Type==Bool) return *this;
        else if(Type==Int){
            tmp.vbool = !(vint == bigint(0));
            return tmp;
        }
        else if(Type==Double){
            tmp.vbool = vdouble != 0;
            return tmp;
        }
        else if(Type==Str){
            tmp.vbool = !vstring.empty();
            return tmp;
        }
    }

    void toprint()const{
        if(Type==Int) cout<<vint;
        else if(Type == Str) cout << vstring;
        else if(Type==Bool) cout<<(vbool ? "True" : "False");
        else if(Type==Non) cout<<"None";
        else printf("%0.6f",vdouble);
    }

    Rec operator+(Rec b){
        Rec a=*this;
        qiangzhi(a,b);
        if(a.Type==Int) return Rec(a.vint+b.vint);
        if(a.Type==Str) return Rec(a.vstring+b.vstring);
        if(a.Type==Double) return Rec(a.vdouble+b.vdouble);
    }
    Rec operator-(Rec b){
        Rec a=*this;
        qiangzhi(a,b);
        if(a.Type==Int) return Rec(a.vint-b.vint);
        if(a.Type==Double) return Rec(a.vdouble-b.vdouble);
    }
    Rec operator*(Rec b){
        Rec a=*this;
        qiangzhi(a,b);
        if(a.Type==Int) return Rec(a.vint*b.vint);
        if(a.Type==Double) return Rec(a.vdouble*b.vdouble);
        if(a.Type==Str){
            Rec tmp("",Str);
            for(bigint i(1);i<=b.vint;i+=bigint(1)){
                tmp.vstring+=a.vstring;
            }
            return tmp;
        }
    }
    Rec operator/(Rec b){
        Rec a=*this;
        qiangzhi(a,b);
        if(a.Type==Int){
            a=a.transdouble();
            b=b.transdouble();
        }
        if(a.Type==Double) return Rec(a.vdouble/b.vdouble);
    }
    friend Rec ZC(Rec a,Rec b){
        qiangzhi(a,b);
        return Rec(Zdiv(a.vint,b.vint));
    }
    Rec operator%(Rec b){
        Rec a=*this;
        qiangzhi(a,b);
        return Rec(a.vint%b.vint);
    }
    Rec &operator+=(const Rec &b){
        *this=*this+b;
        return *this;
    }
    Rec &operator-=(const Rec &b){
        *this=*this-b;
        return *this;
    }
    Rec &operator/=(const Rec &b){
        *this=*this/b;
        return *this;
    }
    Rec &operator*=(const Rec &b){
        *this=*this*b;
        return *this;
    }
    Rec &operator%=(const Rec &b){
        *this=*this%b;
        return *this;
    }
    friend Rec ZCE(Rec &a,const Rec &b){
        a=ZC(a,b);
        return a;
    }
    bool operator==(Rec b){
        Rec a=*this;
        qiangzhi(a,b);
        if(a.Type==Int&&b.Type==Int&&a.vint==b.vint) return true;
        if(a.Type==Str&&b.Type==Str&&a.vstring==b.vstring) return true;
        if(a.Type==Bool&&b.Type==Bool&&a.vbool==b.vbool) return true;
        if(a.Type==Non&&b.Type==Non) return true;
        if(a.Type==Double&&b.Type==Double&&a.vdouble==b.vdouble) return true;
        return false;
    }
    bool operator!=(const Rec&b){
        return !(*this==b);
    }
    bool operator>(Rec b){
        Rec a=*this;
        qiangzhi(a,b);
        if(a.Type==Int&&b.Type==Int&&a.vint>b.vint) return true;
        if(a.Type==Str&&b.Type==Str&&a.vstring>b.vstring) return true;
        if(a.Type==Bool&&b.Type==Bool&&a.vbool>b.vbool) return true;
        if(a.Type==Double&&b.Type==Double&&a.vdouble>b.vdouble) return true;
        return false;
    }
    bool operator<(const Rec&b){
        return !((*this==b)||(*this>b));
    }
    bool operator>=(const Rec& b){
        return !(*this<b);
    }
    bool operator<=(const Rec&b){
        return !(*this>b);
    }
    Rec operator -(){
        if(Type==Int) return Rec(-vint);
        if(Type==Double) return Rec(-vdouble);
    }
    bool operator&&(Rec b){
        return transbool().vbool&&b.transbool().vbool;
    }
    bool operator||(Rec b){
        return transbool().vbool||b.transbool().vbool;
    }
    bool operator!(){
        return !transbool().vbool;
    }
};

#endif //PYTHON_INTERPRETER_REC_H
