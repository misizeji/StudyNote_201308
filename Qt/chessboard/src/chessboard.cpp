#include "chessboard.h"

//==================================================================
//���ܽ��ܣ����캯������Ҫ�ǳ�ʼ��һЩ����
//��ڲ����� fileNameΪ���̱���ͼ��·��
//         numΪ���Ż����ŵĸ���������������8*8������Ĭ�ϲ���Ϊ8
//���ڲ�������
//===================================================================
ChessBoard::ChessBoard(QWidget *parent, QString fileName, int num)
    : QWidget(parent)
{
    // ���̱���ͼƬ��·��
    backImageFileName = fileName;

    // ���̺���(����)�ĸ�����Ϊ8
    gridNum = num;

    // ����״̬��ʼʱ��û�����ã�chessManInfoΪָ�����
    chessManInfo = NULL;

    //����һ��С����Ŀ��(��߶�),�����Ⱥ͸߶����
    grid = 25;

    //��ʼ�����̵��������
    startx = 0;
    starty = 0;

    // (px,py)Ϊ��ǰ���ӵ�����
    px = -1;
    py = -1;
}

//==================================================================
//���ܽ��ܣ���������������û������
//===================================================================
ChessBoard::~ChessBoard()
{
}

//==================================================================
//���ܽ��ܣ��������ڴ�С�����ı�ʱ�Զ������ã�����Ҫ(Ҳ����)��Ϊ����
//        �ص���������������ȷ���������Լ���һ�ֹ�����ô��������Ƶúÿ���ô��
//===================================================================
void ChessBoard::resizeEvent(QResizeEvent *)
{
    int w = width();    // ��ǰ���ڵĿ�
    int h = height();   // ��ǰ���ڵĸ�

    // ������ڸ�ʱ
    if(w>=h) {
        grid = h/(gridNum+2); // ����һ��������(��߶�)��СΪ�������ڵĿ�/10����Ϊ10������
        starty = grid;        // �������Y����Ϊ����һ��������(��߶�)��С
        startx = (w-h)/2+grid;// �������X����
    }
    else {  //���ߴ��ڿ�ʱ�������������࣬�ѿ�͸߻�һ��λ��
        grid = w /(gridNum+2);
        startx = grid;
        starty = (h-w)/2+grid;
    }
}



//==================================================================
//���ܽ��ܣ���ͼ�¼���������״̬�ı�ʱ���Զ������ã�����Ϊ������Ҫ����update()����
//        ��ͼ�¼�����Ҫ�����ǻ����̱���ͼ�����̡�����
//===================================================================
void ChessBoard::paintEvent(QPaintEvent *)
{
    // ��ͼ����(�൱�ڴ���һ������)
    // this���õ�Ȼ����������������
    QPainter painter(this);

    // �����̱���ͼƬ��ͨ�����̵Ĵ�С��ȷ�����̵ı���
    // �������Ϊ����������˰�����񣬿�����̵Ŀ��һ�����񣬸߱����̵ĸ߶�һ������
    painter.drawPixmap(startx-grid/2,starty-grid/2,
                       grid*gridNum+grid,grid*gridNum+grid,
                       QPixmap(backImageFileName));

    // �����߿�����ã��������̷���Ĵ�С����Ӧ�ĸı�
    int lineWidth = 0;
    if(grid < 30)
        lineWidth = 2;
    else if((grid >= 30)&&(grid <50))
        lineWidth = 3;
    else if(grid >= 50)
        lineWidth = 5;

    // myPenΪ���еĳ�Ա�������ж���
    myPen.setWidth(lineWidth);  // ͨ�����������߿�
    myPen.setColor(Qt::black);  // ���û�����ɫ
    painter.setPen(myPen);      // ���ʽ�������

    // ������
    for(int i=0;i<=gridNum;i++){
            painter.drawLine(startx,starty+i*grid,
                             startx+grid*gridNum,starty+i*grid);    // ������
            painter.drawLine(startx+i*grid,starty,
                             startx+grid*i,starty+grid*gridNum);    // ������
        }

    //  ͨ��typedef�Զ���һ������(��ά����ָ��)
    //  ��ΪchessManInfo������(void *)���͵�
    //  p(chessManInfo)[i][j]�������ܰ�(void *)ǿ��ת��Ϊ��ά����ָ������
    typedef int (*p)[gridNum];

    // ������
    // �������Ѿ�������״̬��
    if(NULL != chessManInfo)
    {
        for(int i=0; i<gridNum; i++)
        {
            for(int j=0; j<gridNum; j++)
            {
                // ע���߿�Ĵ���
                // ��Ⱥ͸߶ȼ�2*�߿�ʱΪ�������ӵ���Χ��һ��������
                if(p(chessManInfo)[i][j] == Black)
                {
                    // ������
                    painter.drawPixmap(startx+lineWidth+grid*i,starty+lineWidth+grid*j,
                                       grid-2*lineWidth,grid-2*lineWidth,QPixmap("../image/black.png"));
                }
                else if((p(chessManInfo)[i][j]) == White)
                {
                    // ������
                    painter.drawPixmap(startx+lineWidth+grid*i,starty+lineWidth+grid*j,
                                       grid-2*lineWidth,grid-2*lineWidth,QPixmap("../image/white.png"));
                }
            }
        }
    }

    // �������������Χ��һ����ɫ��������
    // ���ѵ�ǰ���ӵ�λ��
    // (px,py)Ϊ��ǰ���ӵ����������(0, 0)��(7, 7)��
    if(px >=0 && py >= 0 && px<gridNum && py<gridNum)
    {
        myPen.setColor(Qt::green);  // ���û��ʵ���ɫ����ɫ
        painter.setPen(myPen);      // �ѻ��ʽ�������
        painter.drawRect(startx+lineWidth+grid*px,starty+lineWidth+grid*py,
                         grid-2*lineWidth,grid-2*lineWidth);    // ������
    }
}


//==================================================================
//���ܽ��ܣ�������¼�������������Զ������ã�����Ҫ(Ҳ����)��Ϊ����
//         ���¼���ҪΪ��ȷ�����ӵ�λ��
//         �������ʱ���������ڷ���һ���ź�
//===================================================================
void ChessBoard::mousePressEvent(QMouseEvent *e)
{
    // ��ȡ�����x,y������
    int x = e->x();
    int y = e->y();

    // Ҫ��֤����������̷�Χ����
    if((x > startx) && (x < (startx + grid*gridNum))&&
       (y > starty)&&(y < (starty + grid*gridNum)))
    {
        // ������ֵת��Ϊ������̵������±�
        // ������a[i][j]��i��j
        x = (x - startx)/grid;
        y = (y - starty)/grid;

        // �������ڷ���һ���źţ�����Ϊ��������������
        emit pressPoint(QPoint(x,y));
    }
}

//==================================================================
//���ܽ��ܣ���ȡ���ӵĿ�Ȼ�߶�
//===================================================================
int ChessBoard::getGrid()
{
    return grid;
}

//==================================================================
//���ܽ��ܣ����û��ʣ�����Ϊ���úõĻ���
//===================================================================
void ChessBoard::setPen(const QPen & pen)
{
    myPen = pen;
}

//==================================================================
// ��������״̬��manInfoΪ������׵�ַ��(px,py)Ϊ��ǰ���ӵ�����
//===================================================================
void ChessBoard::setChessManInfo(void *manInfo,int px,int py )
{
    // �������������
    this->px = px;
    this->py = py;

    // ����״ָ̬�룬������մ������ڴ�����������״̬
    chessManInfo = manInfo; // �������úõ�����״̬
}
