#include "chessbox.h"
#include <QDebug>

//==========================================================//
//�ڰ����㷨����:                                             //
//����: int h ������ int v ������ void *chess ����             //
//     int player ��������                                   //
//����ֵ: int  ������                                         //
//==========================================================//
int myJudgeRule(int h, int v, void *chess, int player)
{
    //��������
    int dir[8][2]={ {1,0},{1,-1},{0,-1},{-1,-1},
                    {-1,0},{-1,1},{0,1},{1,1} };
    int eat = 0;
    int ch = h;
    int cv = v;
    int nh = 0, nv = 0;

    //ǿ������ת��
    typedef int (*p)[8];
    p chChess = p(chess);

    //����λ�ò�Ϊ�գ���������
    if(chChess[h][v] != EMPTY)
    {
        return 0;
    }

    //8������ѭ���ж�
    for(int i=0;i<8;i++)
    {
        h += dir[i][0];
        v += dir[i][1];

        //�˷�������һ���Ӳ�Ϊ�Է������򻻷��򣬲�ȷ���ж�λ�ò��������̷�Χ
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
            if(chChess[h][v] == EMPTY)  //��������λ��Ϊ�գ����˳��˷���ѭ��
            {
                break;
            }
            if(v<0 || h<0 || v>7 || h >7) //�����߽磬���ܳ���
            {
                break;
            }
            if(chChess[h][v] == player) //���ֱ�������
            {
                chChess[ch][cv] = player;   //����
                do
                {
                    h -= dir[i][0];
                    v -= dir[i][1];

                    chChess[h][v] = player; //����
                    eat++;
                    nh = h-dir[i][0];
                    nv = v-dir[i][1];
                }while(chChess[nh][nv] != player);
                break;
            }
         }
         //�ص�����λ�ã��������������ж�
         h = ch;
         v = cv;
       }
    return eat;
}
