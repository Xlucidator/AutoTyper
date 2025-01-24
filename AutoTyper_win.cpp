#include <windows.h>
#include <cstdlib>
#include <bits/stdc++.h>

using namespace std;

#define key_down(key) ((GetAsyncKeyState(key)&0x8000)?1:0)
#define ctrl(ch) down(17),an(ch),up(17)
#define down(ch) keybd_event(ch,0,0,0)
#define up(ch) keybd_event(ch,0,2,0)
#define an(ch) down(ch),up(ch)

void hide(){
    HWND hwnd;
    if(hwnd=::FindWindow("ConsoleWindowClass",NULL))
        ::ShowWindow(hwnd,SW_HIDE); 
}

int key_num[130]={0,0,0,0,0,0,0,0,0,0,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,1049,11222,1051,1052,1053,1055,10222,11057,1048,1056,1107,188,109,190,111,48,49,50,51,52,53,54,55,56,57,1186,186,11188,187,1190,1191,1050,1065,1066,1067,1068,1069,1070,1071,1072,1073,1074,1075,1076,1077,1078,1079,1080,1081,1082,1083,1084,1085,1086,1087,1088,1089,1090,10219,220,221,1054,1189,192,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,0,1220,1221,1192};

void work(char ch){
    int num=key_num[ch]%1000,is_shift=key_num[ch]%10000/1000,is_del=key_num[ch]/10000;
    if(ch=='{'){
        down(160),an(219),up(160);
        return;
    }
    if(is_shift) down(160);
    an(num);
    if(is_shift) up(160);
    if(is_del) an(46);
}

void str_work(string str){
    for(int i=0;i<str.size();i++) work(char(str[i]));
}

int main(){
    hide();
    while(1) {
        freopen("target.cpp","r",stdin);
        while(!key_down(27)) if(key_down(45)&&key_down(8)) return 0;
        Sleep(3000);
        char ch;
        while(ch=getchar()){
            if(ch==-1) break;
            if(key_down(45)&&key_down(8)) return 0;
            work(ch);
            Sleep(0.5);
        }
        for(int i=1;i<=100;i++) an(46);
        an(116);
        str_work("\n\n//Go to https://www.luogu.com.cn/paste/x9pf2cnw to know more!");
    }
    return 0;
}