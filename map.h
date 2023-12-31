#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;
const int HEIGHT_MAP = 126;//������ ����� ������
const int WIDTH_MAP = 140;//������ ����� ������ 
String TileMap[HEIGHT_MAP] = {





	"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",//1
	"0                               0 0                        0                                              0                                0",//2
	"0                               0 0QQQ                     0                                              0                                0",//3
	"0                               000 QQ                     0000000000000000000000000000000000000000000000 0                                0",//4
	"0                               777 QQ                     0                                            0 0                                0",//5
	"0                               777 QQ      q              0                                            0 0                                0",//5
	"0                               777 QQ                     0                                            0 0                                0",//5
	"0                               777 QQ                     0                                            0 0                                0",//5
	"0                               777 QQ                     0            t                               0 0                    t           0",//5
	"0                m              777 QQ                 q   0                                            0 0                                0",//5
	"0                               000 QQ                     0                                            0 0                                0",//6
	"0                               0 0QQQ                     0                                            0 0                                0",//7
	"0                               0 0                        0                                            0 0                                0",//7
	"0                               0 0                        0                                 t          0 0                                0",//7
	"0                               0 0                        0                                            0 0                                0",//7
	"0                               0 0          Q             0                                            0 0                                0",//7
	"0                               0 0                        0                   E                        0 0                R               0",//7
	"0                               0 0                        0                                          EE000RR                              0",//7
	"0                               0 0                        0                                          E  7  R                              0",//8
	"000000000000077777000000000000000 0                        0                                          E  7  R                              0",//9
	"0           0     0             0 0                        0                                          E  7  R                              0",//10
	"00000000000007777700000000000000000         q              0                 t                        E  7  R                              0",//11
	"0         0          W          0 0                        0                                          EE000RR                              0",//12
	"0         0WWWWWWWWWWW          0 0                        0                                            0 0                                0",//13
	"0         0                     0 0                        0                                            0 0                                0",//13
	"0         0                     0 0            QQQQQQQQQ   0                                            0 0                                0",//13
	"0         0                     0 0            Q       Q   0                                            0 0                                0",//14
	"0         0                     0000000000000000077777000000                               EEEEEEEEEEEEE0 0            t                   0",//15
	"0         0                     0               0     0    0                               E            0 0                                0",//16
	"0         0                     0000000000000000077777000000000000000000000000000000000000000077777000000 0                                0",//17
	"0         0                                   W          0   t                         t  0  0     0    0 0                                0",//18
	"0         0                                   WWWWWWWWWWW0                                000077777000000 0                                0",//19
	"0         0                                              0                t               T 7      0    0 0                                0",//20
	"0         0                   W                          0                                T 7      0    0 0000000000000000000000000000000000",//21
	"0         0                                              0      t                         T 7      000000                                  0",//22
	"0         0                                              0                                T 7      0    0                                  0",//23
	"0         0                                              0                                T0007777700000000000000000000000000000000000000000",//24
	"0         0                                              0              t                  000      Y                                      0",//25
	"0         0                                              0                                 0YYYYYYYYY                                      0",//26
	"0         0               WWWWWWWWWW                     0                                 0                                               0",//27
	"0         0               W        W                     0                        t        0                                               0",//28
	"0  ttt    000000000000000000777770000000000000000000000000 t           T                   0                                    t          0",//29
	"0                                             000 T                                        0                                               0",//30
	"0                                              7  T                                        0                        Y                      0",//31
	"0                                              7  T                                        0       t                                       0",//32
	"0                                              7  T           t        t     t      t      0                                               0",//33
	"0                                              7  T                                        0                                               0",//33
	"0                                              7  T              TTTTTTTTT                 0                     YYYYYYYYYYY               0",//34
	"0                                             000 T  t           T       T              t  0                     Y         Y               0",//35
	"0                                             0 00000000000000000007777700000000000000000000000000000000000000000000777777000000000000000000",//36
	"0                                             0 0                 0     0                  0                                               0",//37
	"00000000000000000000000000000000000000000000000000000000000000000007777700000000000000000000                                               0",//38
	"0                                            0  0                I       I                 0                                               0",//39
	"0                                            0  0                IIIIIIIII                 0                                               0",//40
	"0                                          OO0  0II                                        0          t                    t               0",//40
	"0                                          O 0000 I                                        0                                               0",//41
	"0                                          O 7  7 I                                        0                     U                         0",//42
	"0                 O                        O 7  7 I              I                         0                                               0",//43
	"0                                          O 7  7 I       t                                0                                               0",//44
	"0                                          O 7  7 I                                        0          t                    t               0",//44
	"0                                          O 7  7 I                                        0                                               0",//44
	"0                                          O 0000 I                             t          0                                               0",//45
	"0                                          OO0  0II               t                        0                                               0",//46
	"0                                            0  0                                          0                00000000000000077777000000000000",//47
	"0                                            0  0                                          0                0                              0",//48
	"0                                            0  0                                          0                0                              0",//49
	"0000000007777700000000000000000000000000000000000000000000000000000000000000000000000000000000000007777000000                              0",//50
	"0                     0                                                               7                     0                              0",//51
	"0                     0                                                               7                     0                              0",//52
	"0                     0                                                               7                     00000000000000000000000000000000",//53
	"0                     0         S                                                     7                                                    0",//54
	"0                     0                                                               7                                                    0",//55
	"0         t           7                 00000000000000000000000000000000000000000000000        t                                           0",//56
	"0                     7                 0                                             0                                                    0",//57
	"0          A          7                 0                                             0                      F                             0",//58
	"0                     7                 0                                             7                                                    0",//58
	"0                     7                 0                                             7                 t                    t             0",//59
	"0                     0                 0               D                             7                                                    0",//60
	"0                     0                 0                                             7                                                    0",//60
	"0            t        0                 0                            t                7                                                    0",//61
	"0                     0                 0                                             0                                                    0",//61
	"0                     0000000000000000000           t                                 000000000000000000000000000000000000000000000000000000",//62
	"0                                 0     0                                             0                                                    0",//63
	"0                                 0     0                                             0                                                    0",//64
	"00000000000000000000000077777000000     0                                             0                                                    0",//65
	"0                                 0     0                                             0                                                    0",//66//10
	"0                                 00000000000000000000000000777770000000000000000000000                                                    0",//67
	"0                                 0                                                   0                                                    0",//68
	"0                                 0                                                   0                                                    0",//68
	"0                                 0                                                   0                        J                           0",//69
	"0                     t           0                                                   7                                                    0",//70
	"0                                 0                        H                          7                                                    0",//71
	"0             G                   0                                                   7                                                    0",//72
	"0                                 0                                                   7                                                    0",//73
	"0                                 0                                                   7                                                    0",//73
	"0                                 0                                                   0                                                    0",//74
	"0                       t         0                                                   0                                                    0",//75
	"0                                 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000777770000000000000000",//76
	"0                                 0                 0                                             0                                        0",//77
	"0                                 0                 0                                             0                                        0",//78
	"0                                 0                 0                                             0                                        0",//79
	"0                                 0                 0                                             7                                        0",//80
	"00000000777770000000000000000000000                 0                                             7                                        0",//81
	"0      0777770      0                               0                                             7                    K                   0",//82
	"000000007777700000000                               0                    L                        7                                        0",//83
	"0                   0                               0                                             7                                        0",//84
	"0                   0                  Z            0         t                                   0                     t                  0",//84
	"0                   0                               0                                             0                                        0",//84
	"0                   0                               0                                             0                                        0",//84
	"0                   0                               0                                             0                                        0",//85
	"0    t       t      0                               0                                             000000000000000000000000000000000000000000",//86
	"0                   0                               0                                             0                                        0",//87
	"0                   0                               0                                             0                                        0",//88
	"0                   0000000000000000000000000000000000000000000000000077777000000000000000000000000                                        0",//89
	"0                                                0 0                                           0  0                                        0",//90
	"0                                                0 0                                           0  0                                        0",//90
	"0                                     t          000                                           0  0                                        0",//90
	"0                                                777                                           0000                 V                      0",//91
	"0                                                777                    C                      7777                                        0",//92
	"0                    X                           777                                           7777                                        0",//92
	"0                                                777                                           7777                                        0",//92
	"0                                                777                                           7777                                        0",//93
	"0                                     t          000                                           0000                                        0",//90
	"0                                                0 0                                           0  0                                        0",//90
	"0                                                0 0                                           0  0                                        0",//94
	"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",//116

};

