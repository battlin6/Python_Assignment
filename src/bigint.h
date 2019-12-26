//
// Created by battlin on 2019/12/2.
//

#ifndef PYTHON_INTERPRETER_BIGINT_H
#define PYTHON_INTERPRETER_BIGINT_H
#include <bits/stdc++.h>
using namespace std;


class bigint{
private:
    string num;
    int signal; //1->positive and zero while 0-> negative
public:
    explicit bigint(string);
    explicit bigint(int);
    bigint();
    bool operator >(const bigint&)const;
    bool operator ==(const bigint&)const;
    bool operator <(const bigint&)const;
    bool operator >=(const bigint&)const;
    bool operator <=(const bigint&)const;
    bool operator !=(const bigint&)const;
    bigint &operator =(const bigint&);
    bigint &operator =(const int &);
    bigint &operator =(const string &);
    bigint operator -()const;
    bigint operator +(const bigint&)const;
    bigint operator -(const bigint&)const;
    bigint operator *(const bigint&)const;
    bigint operator %(const bigint&)const;
    bigint &operator +=(const bigint&);
    bigint &operator -=(const bigint&);
    bigint &operator *=(const bigint&);
    bigint &operator %=(const bigint&);
    string getstring();
    friend ostream& operator<<(ostream & os, const bigint &);
    friend bigint Zdiv(const bigint &, const bigint &);
    friend bigint Zdive(bigint &,const bigint &);
    friend int Toint(bigint &);
};



#endif
