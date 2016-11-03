#include "chessbox.h"
#include <QDebug>

//==========================================================//
//黑白棋算法函数:                                             //
//参数: int h 网格行 int v 网格列 void *chess 棋盘             //
//     int player 落子种类                                   //
//返回值: int  吃子数                                         //
//==========================================================//
int myJudgeRule(int h, int v, void *chess, int player)
{
    //方向数组
    int dir[8][2]={ {1,0},{1,-1},{0,-1},{-1,-1},
                    {-1,0},{-1,1},{0,1},{1,1} };
    int eat = 0;
    int ch = h;
    int cv = v;
    int nh = 0, nv = 0;

    //强制类型转换
    typedef int (*p)[8];
    p chChess = p(chess);

    //落子位置不为空，才能落子
    if(chChess[h][v] != EMPTY)
    {
        return 0;
    }

    //8个方向循环判断
    for(int i=0;i<8;i++)
    {
        h += dir[i][0];
        v += dir[i][1];

        //此方向上下一棋子不为对方棋子则换方向，并确保判断位置不超出棋盘范围
        if((chChess[h][v] != !player) || v<0 || h<0 || v>7 || h >7)
        {
            h = ch;
            v = cv;
            continue;
        }

        while(1)
        {
            h += dir[i][0];
            v += dir[i][1];
            if(chChess[h][v] == EMPTY)  //落子相邻位置为空，则退出此方向循环
            {
                break;
            }
            if(v<0 || h<0 || v>7 || h >7) //超出边界，不能吃子
            {
                break;
            }
            if(chChess[h][v] == player) //发现本方棋子
            {
                chChess[ch][cv] = player;   //落子
                do
                {
                    h -= dir[i][0];
                    v -= dir[i][1];

                    chChess[h][v] = player; //吃子
                    eat++;
                    nh = h-dir[i][0];
                    nv = v-dir[i][1];
                }while(chChess[nh][nv] != player);
                break;
            }
         }
         //回到落子位置，换个方向重新判断
         h = ch;
         v = cv;
       }
    return eat;
}
