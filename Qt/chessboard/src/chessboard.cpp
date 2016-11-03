#include "chessboard.h"

//==================================================================
//功能介绍：构造函数，主要是初始化一些数据
//入口参数： fileName为棋盘背景图的路径
//         num为横着或竖着的格数，棋盘网格数8*8，所以默认参数为8
//出口参数：无
//===================================================================
ChessBoard::ChessBoard(QWidget *parent, QString fileName, int num)
    : QWidget(parent)
{
    // 棋盘背景图片的路径
    backImageFileName = fileName;

    // 棋盘横着(竖着)的格数，为8
    gridNum = num;

    // 棋子状态开始时是没有设置，chessManInfo为指针变量
    chessManInfo = NULL;

    //棋盘一个小方格的宽度(或高度),这里宽度和高度相等
    grid = 25;

    //初始化棋盘的起点坐标
    startx = 0;
    starty = 0;

    // (px,py)为当前下子的坐标
    px = -1;
    py = -1;
}

//==================================================================
//功能介绍：析构函数，这里没做处理
//===================================================================
ChessBoard::~ChessBoard()
{
}

//==================================================================
//功能介绍：当主窗口大小发生改变时自动被调用，不需要(也不能)人为调用
//        重点是棋盘起点坐标的确定，可以自己定一种规则，怎么让棋盘设计得好看怎么来
//===================================================================
void ChessBoard::resizeEvent(QResizeEvent *)
{
    int w = width();    // 当前窗口的宽
    int h = height();   // 当前窗口的高

    // 当宽大于高时
    if(w>=h) {
        grid = h/(gridNum+2); // 棋盘一个方格宽度(或高度)大小为：主窗口的宽/10，分为10个方格
        starty = grid;        // 棋盘起点Y坐标为棋盘一个方格宽度(或高度)大小
        startx = (w-h)/2+grid;// 棋盘起点X坐标
    }
    else {  //当高大于宽时，操作和上面差不多，把宽和高换一下位置
        grid = w /(gridNum+2);
        startx = grid;
        starty = (h-w)/2+grid;
    }
}



//==================================================================
//功能介绍：绘图事件，当窗口状态改变时会自动被调用，想人为调用需要调用update()函数
//        画图事件，主要功能是画棋盘背景图、棋盘、棋子
//===================================================================
void ChessBoard::paintEvent(QPaintEvent *)
{
    // 画图对象(相当于创建一个画家)
    // this调用当然这个窗口用这个画家
    QPainter painter(this);

    // 画棋盘背景图片，通过棋盘的大小来确定棋盘的背景
    // 起点坐标为棋盘坐标后退半个方格，宽比棋盘的宽多一个方格，高比棋盘的高多一个方格
    painter.drawPixmap(startx-grid/2,starty-grid/2,
                       grid*gridNum+grid,grid*gridNum+grid,
                       QPixmap(backImageFileName));

    // 棋盘线宽的设置，根据棋盘方格的大小作相应的改变
    int lineWidth = 0;
    if(grid < 30)
        lineWidth = 2;
    else if((grid >= 30)&&(grid <50))
        lineWidth = 3;
    else if(grid >= 50)
        lineWidth = 5;

    // myPen为类中的成员，在类中定义
    myPen.setWidth(lineWidth);  // 通过画笔设置线宽
    myPen.setColor(Qt::black);  // 设置画笔颜色
    painter.setPen(myPen);      // 将笔交给画家

    // 画棋盘
    for(int i=0;i<=gridNum;i++){
            painter.drawLine(startx,starty+i*grid,
                             startx+grid*gridNum,starty+i*grid);    // 画横线
            painter.drawLine(startx+i*grid,starty,
                             startx+grid*i,starty+grid*gridNum);    // 画竖线
        }

    //  通过typedef自定义一个类型(二维数组指针)
    //  因为chessManInfo本来是(void *)类型的
    //  p(chessManInfo)[i][j]这样就能把(void *)强制转换为二维数组指针类型
    typedef int (*p)[gridNum];

    // 画棋子
    // 当棋子已经设置了状态后
    if(NULL != chessManInfo)
    {
        for(int i=0; i<gridNum; i++)
        {
            for(int j=0; j<gridNum; j++)
            {
                // 注意线宽的处理
                // 宽度和高度减2*线宽时为了在棋子的周围画一个正方形
                if(p(chessManInfo)[i][j] == Black)
                {
                    // 画黑子
                    painter.drawPixmap(startx+lineWidth+grid*i,starty+lineWidth+grid*j,
                                       grid-2*lineWidth,grid-2*lineWidth,QPixmap("../image/black.png"));
                }
                else if((p(chessManInfo)[i][j]) == White)
                {
                    // 画白子
                    painter.drawPixmap(startx+lineWidth+grid*i,starty+lineWidth+grid*j,
                                       grid-2*lineWidth,grid-2*lineWidth,QPixmap("../image/white.png"));
                }
            }
        }
    }

    // 下棋后，在棋子周围画一个绿色的正方形
    // 提醒当前下子的位置
    // (px,py)为当前下子的坐标必须在(0, 0)到(7, 7)的
    if(px >=0 && py >= 0 && px<gridNum && py<gridNum)
    {
        myPen.setColor(Qt::green);  // 设置画笔的颜色，绿色
        painter.setPen(myPen);      // 把画笔交给画家
        painter.drawRect(startx+lineWidth+grid*px,starty+lineWidth+grid*py,
                         grid-2*lineWidth,grid-2*lineWidth);    // 画矩形
    }
}


//==================================================================
//功能介绍：鼠标点击事件，当鼠标点击会自动被调用，不需要(也不能)人为调用
//         此事件主要为了确定下子的位置
//         当鼠标点击时，给主窗口发射一个信号
//===================================================================
void ChessBoard::mousePressEvent(QMouseEvent *e)
{
    // 获取点击点x,y的坐标
    int x = e->x();
    int y = e->y();

    // 要保证点击点在棋盘范围里面
    if((x > startx) && (x < (startx + grid*gridNum))&&
       (y > starty)&&(y < (starty + grid*gridNum)))
    {
        // 将坐标值转换为相对棋盘的数组下标
        // 类似于a[i][j]的i和j
        x = (x - startx)/grid;
        y = (y - starty)/grid;

        // 给主窗口发射一个信号，参数为点击棋盘里的坐标
        emit pressPoint(QPoint(x,y));
    }
}

//==================================================================
//功能介绍：获取格子的宽度或高度
//===================================================================
int ChessBoard::getGrid()
{
    return grid;
}

//==================================================================
//功能介绍：设置画笔，参数为设置好的画笔
//===================================================================
void ChessBoard::setPen(const QPen & pen)
{
    myPen = pen;
}

//==================================================================
// 设置棋盘状态，manInfo为数组的首地址，(px,py)为当前下子的坐标
//===================================================================
void ChessBoard::setChessManInfo(void *manInfo,int px,int py )
{
    // 棋盘下棋的坐标
    this->px = px;
    this->py = py;

    // 棋盘状态指针，方便接收从主窗口传过来的棋盘状态
    chessManInfo = manInfo; // 保存设置好的棋子状态
}
