//
// Created by battlin on 2019/12/3.
//

#include "bigint.h"

#include <utility>
using namespace std;
bigint::bigint(string c) {
    if (c == "-0") {
        *this = 0;
    } else if (c[0] == '-') {
            string tmps;
            for (int i = 1; i < c.size(); ++i) {
                tmps += c[i];
            }
            num = tmps;
            signal=0;
            }
    else {
        num=std::move(c);
        signal=1;
    }
}
bigint::bigint(int x) {
    if (x == 0) {
        signal = 1;
        num = "0";
    } else {
        if (x > 0) {
            signal = 1;
        }
        if (x < 0) {
            signal = 0;
            x=-x;
        }
        while (x) {
        int now = x % 10;
        num += char('0' + now);
        x /= 10;
        }
    reverse(num.begin(), num.end());
    }
}
bigint::bigint(){
    num="";
    signal=1;
}
bool bigint::operator>(const bigint &b)const {
    if(signal==1&&b.signal==0)
        return true;
    else if(signal==1&&b.signal==1){
        if(num.size()>b.num.size())
            return true;
        if(num.size()==b.num.size()&&num>b.num)
            return true;
    }
    else if(signal==0&&b.signal==0){
        if(num.size()<b.num.size())
            return true;
        if(num.size()==b.num.size()&&num<b.num)
            return true;
    } else
        return false;
}
bool bigint::operator==(const bigint &b) const {
    return num==b.num&&signal==b.signal;
}
bool bigint::operator<(const bigint &b) const {
    return !(*this==b||*this>b);
}
bool bigint::operator>=(const bigint &b) const {
    return !(*this<b);
}
bool bigint::operator<=(const bigint &b) const {
    return !(*this>b);
}
bool bigint::operator!=(const bigint &b) const {
    return !(*this==b);
}
bigint &bigint::operator=(const bigint &b){
    if(this==&b) return *this;
    num=b.num;
    signal=b.signal;
    return *this;
}
bigint &bigint::operator=(const int &i) {
    bigint tmp(i);
    *this=tmp;
    return *this;
}
bigint &bigint::operator=(const string &s) {
    bigint tmp(s);
    *this=tmp;
    return *this;
}
bigint bigint::operator-()const{
    if(num=="0") return *this;
    bigint tmp=*this;
    tmp.signal^=1;
    return tmp;
}
bigint bigint::operator+(const bigint &b)const{
    bigint tmp;
    if(signal==1&&b.signal==1){
        if(num.size()>b.num.size()){
            for(int i=0;i<num.size();++i){
                tmp.num+=num[num.size()-i-1];
            }
            for(int i=0;i<b.num.size();++i) {
                tmp.num[i] += b.num[b.num.size()-i-1]-'0';
            }
        }
        else{
            for(int i=0;i<b.num.size();++i){
                tmp.num+=b.num[b.num.size()-i-1];
            }
            for(int i=0;i<num.size();++i){
                tmp.num[i]+=num[num.size()-i-1]-'0';
            }
        }
        for(int i=0;i<tmp.num.size()-1;++i){
            if(tmp.num[i]>=char('0'+10)){
                tmp.num[i]-=10;
                tmp.num[i+1]++;
            }
        }
        if(tmp.num[tmp.num.size()-1]>=char('0'+10)){
            tmp.num[tmp.num.size()-1]-=10;
            tmp.num+='1';
        }
        reverse(tmp.num.begin(),tmp.num.end());
        return tmp;
    } else if(signal==0&&b.signal==0){
        if(num.size()>b.num.size()){
            for(int i=0;i<num.size();++i){
                tmp.num+=num[num.size()-i-1];
            }
            for(int i=0;i<b.num.size();++i) {
                tmp.num[i] += b.num[b.num.size()-i-1]-'0';
            }
        }
        else{
            for(int i=0;i<b.num.size();++i){
                tmp.num+=b.num[b.num.size()-i-1];
            }
            for(int i=0;i<num.size();++i){
                tmp.num[i]+=num[num.size()-i-1]-'0';
            }
        }
        for(int i=0;i<tmp.num.size()-1;++i){
            if(tmp.num[i]>=char('0'+10)){
                tmp.num[i]-=10;
                tmp.num[i+1]++;
            }
        }
        if(tmp.num[tmp.num.size()-1]>=char('0'+10)){
            tmp.num[tmp.num.size()-1]-=10;
            tmp.num+='1';
        }
        reverse(tmp.num.begin(),tmp.num.end());
        tmp.signal=0;
        return tmp;
    } else if(signal==1&&b.signal==0){
        tmp=b;
        tmp.signal=1;
        return *this-tmp;
    } else{
        tmp=*this;
        tmp.signal=1;
        return b-tmp;
    }
}
bigint bigint::operator-(const bigint &b)const{
    bigint tmp;
    if(signal==1&&b.signal==1){
        if(*this==b){
            tmp.num="0";
            return tmp;
        } else if(*this>b){
            for(int i=0;i<num.size();++i){
                tmp.num+=num[num.size()-1-i];
            }
            for(int i=0;i<b.num.size();++i){
                tmp.num[i]=tmp.num[i]-b.num[b.num.size()-1-i]+'0';
            }
            for(int i=0;i<tmp.num.size();++i){
                if(tmp.num[i]<'0'){
                    tmp.num[i]+=10;
                    tmp.num[i+1]-=1;
                }
            }
            int j=0;
            for(int i=0;i<tmp.num.size();++i){
                if(tmp.num[tmp.num.size()-i-1]!='0'){
                    j=tmp.num.size()-i-1;
                    break;
                }
            }
            bigint T;
            for(int i=0;i<=j;++i){
                T.num+=tmp.num[j-i];
            }
            return T;
        } else{
            for(int i=0;i<b.num.size();++i){
                tmp.num+=b.num[b.num.size()-1-i];
            }
            for(int i=0;i<num.size();++i){
                tmp.num[i]=tmp.num[i]-num[num.size()-1-i]+'0';
            }
            for(int i=0;i<tmp.num.size();++i){
                if(tmp.num[i]<'0'){
                    tmp.num[i]+=10;
                    tmp.num[i+1]-=1;
                }
            }
            int j=0;
            for(int i=0;i<tmp.num.size();++i){
                if(tmp.num[tmp.num.size()-i-1]!='0'){
                    j=tmp.num.size()-i-1;
                    break;
                }
            }
            bigint T;
            for(int i=0;i<=j;++i){
                T.num+=tmp.num[j-i];
            }
            T.signal=0;
            return T;
        }
    } else if(signal==0&&b.signal==0){
        if(*this==b){
            tmp.num="0";
            return tmp;
        } else if(*this<b){
            for(int i=0;i<num.size();++i){
                tmp.num+=num[num.size()-1-i];
            }
            for(int i=0;i<b.num.size();++i){
                tmp.num[i]=tmp.num[i]-b.num[b.num.size()-1-i]+'0';
            }
            for(int i=0;i<tmp.num.size();++i){
                if(tmp.num[i]<'0'){
                    tmp.num[i]+=10;
                    tmp.num[i+1]-=1;
                }
            }
            int j=0;
            for(int i=0;i<tmp.num.size();++i){
                if(tmp.num[tmp.num.size()-i-1]!='0'){
                    j=tmp.num.size()-i-1;
                    break;
                }
            }
            bigint T;
            for(int i=0;i<=j;++i){
                T.num+=tmp.num[j-i];
            }
            T.signal=0;
            return T;
        } else{
            for(int i=0;i<b.num.size();++i){
                tmp.num+=b.num[b.num.size()-1-i];
            }
            for(int i=0;i<num.size();++i){
                tmp.num[i]=tmp.num[i]-num[num.size()-1-i]+'0';
            }
            for(int i=0;i<tmp.num.size();++i){
                if(tmp.num[i]<'0'){
                    tmp.num[i]+=10;
                    tmp.num[i+1]-=1;
                }
            }
            int j=0;
            for(int i=0;i<tmp.num.size();++i){
                if(tmp.num[tmp.num.size()-i-1]!='0'){
                    j=tmp.num.size()-i-1;
                    break;
                }
            }
            bigint T;
            for(int i=0;i<=j;++i){
                T.num+=tmp.num[j-i];
            }
            T.signal=0;
            return T;
        }
    } else if(signal==1&&b.signal==0){
        tmp=b;
        tmp.signal=1;
        return *this+tmp;
    } else{
        tmp=b;
        tmp.signal=0;
        return *this+tmp;
    }
}
bigint bigint::operator*(const bigint &b)const{
    bigint tmp;
    for(int i=0;i<int(num.size())+int(b.num.size());++i)
        tmp.num+="0";
    for(int i=0;i<num.size();++i){
        for(int j=0;j<b.num.size();++j){
            tmp.num[i+j]+=int(num[num.size()-1-i]-'0')*int(b.num[b.num.size()-1-j]-'0');
            if(tmp.num[i+j]>'9'){
                tmp.num[i+j]-=10;
                tmp.num[i+j+1]+=1;
            }
           // cout<<int(num[num.size()-1-i]-'0')<<" "<<int(b.num[b.num.size()-1-j]-'0')<<" "<<tmp.num[i+j]<<endl;
        }
    }
    //cout<<tmp.num<<endl;
    for(int i=0;i<tmp.num.size();++i){
        while(tmp.num[i]>'9'){
            tmp.num[i]-=10;
            tmp.num[i+1]+=1;
        }
      //  cout<<tmp.num[i]<<" "<<bool(tmp.num[i]>'9')<<endl;
    }
    int j=0;
    for(int i=0;i<tmp.num.size();++i){
        if(tmp.num[tmp.num.size()-i-1]!='0'){
            j=tmp.num.size()-i-1;
            break;
        }
    }
    bigint T;
    for(int i=0;i<=j;++i){
        T.num+=tmp.num[j-i];
    }
    if(T.num=="0") return T;
    else if(signal!=b.signal){
        T.signal^=1;
        return T;
    }else return T;
}
ostream & operator<<(ostream &os, const bigint &b) {
    if(b.signal==0)
        os<<'-';
    os << b.num;
}
bigint Zdiv(const bigint &a,const bigint &b){
    bigint tmp,tmp1=a,tmp2=b;
    tmp1.signal=1,tmp2.signal=1;
    if(tmp1<tmp2){
        bigint T("0");
        return T;
    } else{
        bigint test,TT;
        for(int i=0;i<tmp2.num.size();++i){
            test.num+=a.num[i];
        }
        for(int k=0;k<int(a.num.size())-int(tmp2.num.size())+1;++k){
            int ans=0;
            while(test>=tmp2){
                test=test-tmp2;
                ans++;
            }
            TT.num+=char(ans+'0');
            if(test.num=="0") test.num=a.num[k+tmp2.num.size()];
            else if(k!=int(a.num.size())-int(tmp2.num.size()))
                test.num+=a.num[k+tmp2.num.size()];
        }
        reverse(TT.num.begin(),TT.num.end());
        int j=0;
        for(int i=0;i<TT.num.size();++i){
            if(TT.num[TT.num.size()-i-1]!='0'){
                j=TT.num.size()-i-1;
                break;
            }
        }
        bigint T;
        for(int i=0;i<=j;++i){
            T.num+=TT.num[j-i];
        }
        if(a.signal==b.signal){
            return T;
        }
        if(a.signal!=b.signal){
            T.signal^=1;
            if(b*T!=a) {
                bigint cnt("1");
                T = T - cnt;
            }
            return T;
        }
    }
}
bigint Zdive(bigint &a,const bigint &b) {
    a = Zdiv(a,b);
    return a;
}
bigint bigint::operator%(const bigint &b)const{
    return *this-Zdiv(*this,b)*b;
}
bigint &bigint::operator+=(const bigint &b) {
    *this=*this+b;
    return *this;
}
bigint &bigint::operator-=(const bigint &b) {
    *this=*this-b;
    return *this;
}
bigint &bigint::operator*=(const bigint &b) {
    *this=*this*b;
    return *this;
}
bigint &bigint::operator%=(const bigint &b) {
    *this=*this%b;
    return *this;
}
int Toint(bigint &b){
    int x=0;
    for(int i=0;i<b.num.size();++i){
        x*=10;
        x+=(b.num[b.num.size()-1-i]-'0');
    }
    if(b.signal==0) x=-x;
    return x;
}

string bigint::getstring(){
    return num;
}




