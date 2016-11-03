#include "widget.h"
#include "ui_widget.h"


// 主界面的构造函数
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 初始化数据
    gridNum = 8;            // 这个数据为for循环里的条件判断值
    this->isStart = false;
    PVPFlag = true;

    board = new ChessBoard(this,"../image/board.jpg",gridNum);  // 棋盘对象，new空间时，为其构造函数传递一些参数
    this->ui->verticalLayoutBoard->addWidget(board);            // 将棋盘加入垂直布局中
    init();                                                     // 初始化函数，主要是黑白棋的初始化，按键使能状态设置

    // 信号与槽的关联
    connect(board,SIGNAL(pressPoint(QPoint)),
            this,SLOT(doPressPoint(QPoint)));  // pressPoint()为棋盘发出的信号，doPressPoint()为这边的处理函数
    connect(&timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    connect(&machineTime,SIGNAL(timeout()),this,SLOT(PVEmethod())); // 人机对战用的时间
}

Widget::~Widget()
{
    delete ui;
    delete board;
}

// 初始化函数，主要是黑白棋的初始化，按键使能状态设置
void Widget::init()
{
    // 64个格全部标志为Empty，标志位是在ChessBoard定义的枚举常量，gridNum的值为8
    for(int i=0; i<gridNum; i++)
        for(int j=0; j<gridNum; j++)
            chess[0][i][j] = ChessBoard::Empty;

    // 将棋盘中间的4个空间分别标志位2黑子2白子
    // 引用枚举常量有两种写法：board->Black(对象名->枚举常量)，ChessBoard::Black(类名::枚举常量)
    chess[0][gridNum/2-1][gridNum/2-1]= board->Black;   // a[3][3]
    chess[0][gridNum/2-1][gridNum/2]= board->White;     // a[3][4]
    chess[0][gridNum/2][gridNum/2-1]= board->White;     // a[4][3]
    chess[0][gridNum/2][gridNum/2]= board->Black;       // a[4][4]
    this->board->hide();                                // 开始时隐藏棋盘

    board->setChessManInfo((void *)chess[0]);   // 将棋子的状态通过setChessManInfo()传递过去
    this->board->update();                      // 棋盘更新，作用为了刷图

    // 按键的使能设置
    ui->toolButtonStart->setEnabled(true);
    ui->toolButtonMode->setEnabled(true);
    ui->toolButtonOver->setEnabled(false);
    ui->toolButtonJump->setEnabled(false);
    ui->toolButtonGoback->setEnabled(false);

    this->isStart = false;              // 初始化开始标志，false表示游戏还没有开始
    currentRole = ChessBoard::Black;    // 规则是黑子先下，所以黑子角色优先
    step = 0;       // 初始化数据，保存棋盘的数据
    failFlag = 0;

}

//　棋盘传回来信号后的处理函数
void Widget::doPressPoint(QPoint point)
{
    if(this->PVPFlag == false && this->currentRole == ChessBoard::White)
        return;                         // 如果人机模式，必须是机器下完，人才能下

    // 判断棋子能否吃子之前，把棋盘的状态保存起来
    for(int i=0;i<this->gridNum;i++)
    {
        for(int j=0;j<this->gridNum;j++)
          chess[step+1][i][j] = chess[step][i][j];  // chess[step]还是没有吃子的状态
    }

    // 如果能吃子，num大于0
    // 吃子规则的参数：棋盘数组坐标位置、格数、棋子状态(chess[step+1])、棋子的角色
    if(0 < judgeRule(point.x(),point.y(),gridNum, (void *)chess[step+1],currentRole))
    {
        this->failFlag=0;
        step++;         // 加1，这时step已经改变了，多了1
        changeRole();   // 改变棋子的角色
        board->setChessManInfo((void *)chess[step],point.x(),point.y());
                        // 吃子后标志位改变了，然后开始设置棋子的状态，参数分别为：棋子状态、棋盘数组坐标
        this->board->update();  // 更新棋盘，刷图
        this->showScore();      // 显示分数

    } 
}

// 人机对战函数
void Widget::PVEmethod()
{
    this->machineTime.stop();   // 机器的时间先停止
    int num = 0;
    int max = 0;
    int px;
    int py;

    // 判断棋盘上哪些位置是能够吃子的
    for(int i= 0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(chess[step][i][j] == ChessBoard::Empty)  // 为棋盘空时，才能下子
            {   // 判断能不能吃子，只是判断，不改变棋盘状态，注意false参数
                num = judgeRule(i,j,gridNum,(void*)chess[step],currentRole,false);
                if(num > max)   // 挑一个吃子最多的空格，并把坐标保存起来
                {
                    max = num;  // 改变max的值
                    px = i;
                    py = j;
                }
            }
        }
    }

    if(max == 0)    // 到这一步max为0，说明方格里都不能吃子，这是就改变棋子的角色
    {
        this->changeRole(); // 改变棋子的角色
        return ;            // 中断此函数
    }

    // 下面为机器真正的吃子
    // 判断棋子能否吃子之前，把棋盘的状态保存起来
    for(int i=0;i<this->gridNum;i++)
    {
        for(int j=0;j<this->gridNum;j++)
          chess[step+1][i][j] = chess[step][i][j];  // chess[step]还是没有吃子的状态
    }
    // 如果能吃子，num大于0
    // 吃子规则的参数：棋盘数组坐标位置(这个坐标肯定能吃子)、格数、棋子状态(chess[step+1])、棋子的角色
    if(0 < judgeRule(px,py,gridNum, (void *)chess[step+1],currentRole))
    {
        failFlag = 0;
        step++;         // 加1，这时step已经改变了，多了1
        changeRole();   // 改变棋子的角色
        board->setChessManInfo((void *)chess[step],px,py);  // 设置棋子状态
        this->board->update();  // 更新状态
         this->showScore();      // 显示分数
    }
}

// 吃子规则的参数：棋盘数组坐标位置(x y)、横着或竖着的格数(gridNum)、
// 棋子状态(chess)、棋子的角色(注意其变量的写法)、eatChess默认为true
int Widget::judgeRule(int x, int y, int gridNum, void *chess, ChessBoard::GridState currentRole,bool eatChess)
{
    // 棋盘的八个方向
    int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    int temp_x = x, temp_y = y;             // 临时保存棋盘数组坐标位置
    int i = 0, eatNum = 0;                  // 初始化数据
    typedef int (*p) [gridNum];             // 自定义类型
    p chessBoard = p(chess);                // 类型转换

    if(chessBoard[temp_x][temp_y] != ChessBoard::Empty)  // 如果此方格内已有棋子，返回;
        return 0;

    // 棋盘的8个方向
    for(i = 0 ; i <8; i++)
    {
        temp_x += dir[i][0]; temp_y += dir[i][1];	// 准备判断相邻棋子
        // 如果没有出界，且相邻棋子是对方棋子，才有吃子的可能．
        if((temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            && (chessBoard[temp_x][temp_y] != currentRole) && (chessBoard[temp_x][temp_y] != ChessBoard::Empty) )
        {
            temp_x += dir[i][0]; temp_y += dir[i][1];	            // 继续判断下一个，向前走一步
            while(temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            {
                if(chessBoard[temp_x][temp_y] == ChessBoard::Empty) // 遇到空位跳出
                        break;
                if(chessBoard[temp_x][temp_y] == currentRole)       // 找到自己的棋子，代表可以吃子
                {
                    if(eatChess == true)                            // 确定吃子
                    {

                        chessBoard[x][y] = currentRole;          // 开始点标志为自己的棋子
                        temp_x -= dir[i][0]; temp_y -= dir[i][1];// 后退一步
                        while((temp_x != x )||(temp_y != y))	 // 只要没有回到开始的位置就执行
                        {
                            chessBoard[temp_x][temp_y] = currentRole;   // 标志为自己的棋子
                            temp_x -= dir[i][0]; temp_y -= dir[i][1];   // 继续后退一步
                            eatNum ++;  // 累计
                        }
                    }
                    else    // 不吃子，只是判断这个位置能不能吃子
                    {

                        temp_x -= dir[i][0]; temp_y -= dir[i][1];   // 后退一步
                        while((temp_x != x )||(temp_y != y))        // 只计算可以吃子的个数
                        {
                            temp_x -= dir[i][0]; temp_y -= dir[i][1];// 继续后退一步
                            eatNum ++;
                        }
                    }
                    break;  // 跳出循环
                }// 没有找到自己的棋子，就向前走一步
                temp_x += dir[i][0]; temp_y += dir[i][1];   // 向前走一步
            }
        }// 如果这个方向不能吃子，就换一个方向
        temp_x = x; temp_y = y;
    }

    return eatNum;              // 返回能吃子的个数
}

// 显示分数函数
void Widget::showScore()
{
   int blackNum=0;
   int whiteNum=0;
   bool isOver = true;
   // 棋盘的棋子遍历一遍，累计黑白棋各自的个数
    for(int i=0;i<gridNum;i++)
    {
        for(int j=0;j<gridNum;j++)
        {
            if(chess[step][i][j] == ChessBoard::White)      // 白子
              whiteNum++;
            else if(chess[step][i][j] == ChessBoard::Black) // 黑子
               blackNum++;

            if(0 < judgeRule(i,j,gridNum, (void *)chess[step],ChessBoard::Black, false) ||
               0 < judgeRule(i,j,gridNum, (void *)chess[step],ChessBoard::White, false)){
                isOver = false;
            }
        }
    }
    // 分别显示到对应的LCD上
    ui->lcdNumberBlack->display(blackNum);
    ui->lcdNumberWhite->display(whiteNum);

    if(false == isOver){
        return;
    }

    // 判断输赢
    if(blackNum > whiteNum){
        QMessageBox::information(this,"win","black win");
    }else if(blackNum < whiteNum){
        QMessageBox::information(this,"win","white win");
    }else{
        QMessageBox::information(this,"win","the same");
    }
    this->on_toolButtonOver_clicked();  // 游戏结束函数


}

// 画图事件，这里主要是画主窗口的背景图片
void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap("../image/background.jpg"));
}

// 开始按钮的操作
void Widget::on_toolButtonStart_clicked()
{
   if(this->isStart == false)   // 如果游戏没有开始
    {   // 相应按键的使能设置
        ui->toolButtonStart->setEnabled(false);
        ui->toolButtonMode->setEnabled(false);
        ui->toolButtonOver->setEnabled(true);
        ui->toolButtonJump->setEnabled(true);
        ui->toolButtonGoback->setEnabled(true);
        this->isStart = true;   // 标志游戏已经开始了
        this->times=30;         // 时间初始化为30s
        ui->lcdNumber->display(times);  // LED显示时间
        this->timer.start(1000);        // 以每次1S启动时间
        this->board->show();            // 显示棋盘
    }
   else
   {
       ui->toolButtonStart->setEnabled(true);
       ui->toolButtonMode->setEnabled(true);
       ui->toolButtonOver->setEnabled(false);
       ui->toolButtonJump->setEnabled(false);
       ui->toolButtonGoback->setEnabled(false);
       this->isStart = false;
       this->times=0;
       ui->lcdNumber->display(times);
       this->timer.stop();
   }

}

// 时间的处理函数，每1S执行一次
void Widget::timerSlot()
{
    this->times--;          // 每次减1
    if(this->times == -1)   // 时间到就改变棋子的角色
        changeRole();       // 改变棋子的角色
    ui->lcdNumber->display(this->times);    // 时间显示到LCD上
}

// 改变棋子的角色
void Widget::changeRole()
{
    this->times=30;     // 时间设置为30S
    // 如果当前为白子，就换为黑子
    if(this->currentRole == ChessBoard::White)
        this->currentRole = ChessBoard::Black;
    else
        this->currentRole = ChessBoard::White;
    // 将新的时间显示到LCD上
    ui->lcdNumber->display(this->times);

    // 如果是人机对战模式，并且到白子下了
    if(this->PVPFlag == false && this->currentRole == ChessBoard::White)
    {
        this->machineTime.start(1000);  // 启动机器的时间
    }
}

// 模式转换
void Widget::on_toolButtonMode_clicked()
{
    // 如果当前是人人，按下后就变为人机对战，并改变相应的文字
    if(this->PVPFlag == true)
    {
        this->PVPFlag = false;
        ui->toolButtonMode->setText("PVE");
    }
    else
    {
        this->PVPFlag = true;
        ui->toolButtonMode->setText("PVP");
    }
}

// 悔棋按键
void Widget::on_toolButtonGoback_clicked()
{   // 如果是人人对战，并且已经下子了
    if(this->PVPFlag == true && this->step >= 1)
    {
        this->changeRole(); // 改变棋子的角色
        step--;             // 推一步
    }
    else if(this->PVPFlag == false && this->step >= 2 ) // 如果是人机对战
        step = step -2;                                 // 退两步
    this->board->setChessManInfo((void *)chess[step]);  // 改变棋盘标志位
    this->board->update();  // 更新棋盘
}

// 改变棋子角色按钮
void Widget::on_toolButtonJump_clicked()
{
    if(this->PVPFlag == false && this->currentRole == ChessBoard::White)
        return;                         // 如果人机模式，必须是机器, 不能按

    this->changeRole();
}

// 游戏结束按钮
void Widget::on_toolButtonOver_clicked()
{
    this->init();   // 初始化棋盘数据
    // LCD的数据都显示0
    ui->lcdNumber->display(0);
    ui->lcdNumberBlack->display(0);
    ui->lcdNumberWhite->display(0);
    // 时间停止
    this->timer.stop();
    this->machineTime.stop();
}
