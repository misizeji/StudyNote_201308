#include "widget.h"
#include "ui_widget.h"


// ������Ĺ��캯��
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // ��ʼ������
    gridNum = 8;            // �������Ϊforѭ����������ж�ֵ
    this->isStart = false;
    PVPFlag = true;

    board = new ChessBoard(this,"../image/board.jpg",gridNum);  // ���̶���new�ռ�ʱ��Ϊ�乹�캯������һЩ����
    this->ui->verticalLayoutBoard->addWidget(board);            // �����̼��봹ֱ������
    init();                                                     // ��ʼ����������Ҫ�Ǻڰ���ĳ�ʼ��������ʹ��״̬����

    // �ź���۵Ĺ���
    connect(board,SIGNAL(pressPoint(QPoint)),
            this,SLOT(doPressPoint(QPoint)));  // pressPoint()Ϊ���̷������źţ�doPressPoint()Ϊ��ߵĴ�����
    connect(&timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    connect(&machineTime,SIGNAL(timeout()),this,SLOT(PVEmethod())); // �˻���ս�õ�ʱ��
}

Widget::~Widget()
{
    delete ui;
    delete board;
}

// ��ʼ����������Ҫ�Ǻڰ���ĳ�ʼ��������ʹ��״̬����
void Widget::init()
{
    // 64����ȫ����־ΪEmpty����־λ����ChessBoard�����ö�ٳ�����gridNum��ֵΪ8
    for(int i=0; i<gridNum; i++)
        for(int j=0; j<gridNum; j++)
            chess[0][i][j] = ChessBoard::Empty;

    // �������м��4���ռ�ֱ��־λ2����2����
    // ����ö�ٳ���������д����board->Black(������->ö�ٳ���)��ChessBoard::Black(����::ö�ٳ���)
    chess[0][gridNum/2-1][gridNum/2-1]= board->Black;   // a[3][3]
    chess[0][gridNum/2-1][gridNum/2]= board->White;     // a[3][4]
    chess[0][gridNum/2][gridNum/2-1]= board->White;     // a[4][3]
    chess[0][gridNum/2][gridNum/2]= board->Black;       // a[4][4]
    this->board->hide();                                // ��ʼʱ��������

    board->setChessManInfo((void *)chess[0]);   // �����ӵ�״̬ͨ��setChessManInfo()���ݹ�ȥ
    this->board->update();                      // ���̸��£�����Ϊ��ˢͼ

    // ������ʹ������
    ui->toolButtonStart->setEnabled(true);
    ui->toolButtonMode->setEnabled(true);
    ui->toolButtonOver->setEnabled(false);
    ui->toolButtonJump->setEnabled(false);
    ui->toolButtonGoback->setEnabled(false);

    this->isStart = false;              // ��ʼ����ʼ��־��false��ʾ��Ϸ��û�п�ʼ
    currentRole = ChessBoard::Black;    // �����Ǻ������£����Ժ��ӽ�ɫ����
    step = 0;       // ��ʼ�����ݣ��������̵�����
    failFlag = 0;

}

//�����̴������źź�Ĵ�����
void Widget::doPressPoint(QPoint point)
{
    if(this->PVPFlag == false && this->currentRole == ChessBoard::White)
        return;                         // ����˻�ģʽ�������ǻ������꣬�˲�����

    // �ж������ܷ����֮ǰ�������̵�״̬��������
    for(int i=0;i<this->gridNum;i++)
    {
        for(int j=0;j<this->gridNum;j++)
          chess[step+1][i][j] = chess[step][i][j];  // chess[step]����û�г��ӵ�״̬
    }

    // ����ܳ��ӣ�num����0
    // ���ӹ���Ĳ�����������������λ�á�����������״̬(chess[step+1])�����ӵĽ�ɫ
    if(0 < judgeRule(point.x(),point.y(),gridNum, (void *)chess[step+1],currentRole))
    {
        this->failFlag=0;
        step++;         // ��1����ʱstep�Ѿ��ı��ˣ�����1
        changeRole();   // �ı����ӵĽ�ɫ
        board->setChessManInfo((void *)chess[step],point.x(),point.y());
                        // ���Ӻ��־λ�ı��ˣ�Ȼ��ʼ�������ӵ�״̬�������ֱ�Ϊ������״̬��������������
        this->board->update();  // �������̣�ˢͼ
        this->showScore();      // ��ʾ����

    } 
}

// �˻���ս����
void Widget::PVEmethod()
{
    this->machineTime.stop();   // ������ʱ����ֹͣ
    int num = 0;
    int max = 0;
    int px;
    int py;

    // �ж���������Щλ�����ܹ����ӵ�
    for(int i= 0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(chess[step][i][j] == ChessBoard::Empty)  // Ϊ���̿�ʱ����������
            {   // �ж��ܲ��ܳ��ӣ�ֻ���жϣ����ı�����״̬��ע��false����
                num = judgeRule(i,j,gridNum,(void*)chess[step],currentRole,false);
                if(num > max)   // ��һ���������Ŀո񣬲������걣������
                {
                    max = num;  // �ı�max��ֵ
                    px = i;
                    py = j;
                }
            }
        }
    }

    if(max == 0)    // ����һ��maxΪ0��˵�������ﶼ���ܳ��ӣ����Ǿ͸ı����ӵĽ�ɫ
    {
        this->changeRole(); // �ı����ӵĽ�ɫ
        return ;            // �жϴ˺���
    }

    // ����Ϊ���������ĳ���
    // �ж������ܷ����֮ǰ�������̵�״̬��������
    for(int i=0;i<this->gridNum;i++)
    {
        for(int j=0;j<this->gridNum;j++)
          chess[step+1][i][j] = chess[step][i][j];  // chess[step]����û�г��ӵ�״̬
    }
    // ����ܳ��ӣ�num����0
    // ���ӹ���Ĳ�����������������λ��(�������϶��ܳ���)������������״̬(chess[step+1])�����ӵĽ�ɫ
    if(0 < judgeRule(px,py,gridNum, (void *)chess[step+1],currentRole))
    {
        failFlag = 0;
        step++;         // ��1����ʱstep�Ѿ��ı��ˣ�����1
        changeRole();   // �ı����ӵĽ�ɫ
        board->setChessManInfo((void *)chess[step],px,py);  // ��������״̬
        this->board->update();  // ����״̬
         this->showScore();      // ��ʾ����
    }
}

// ���ӹ���Ĳ�����������������λ��(x y)�����Ż����ŵĸ���(gridNum)��
// ����״̬(chess)�����ӵĽ�ɫ(ע���������д��)��eatChessĬ��Ϊtrue
int Widget::judgeRule(int x, int y, int gridNum, void *chess, ChessBoard::GridState currentRole,bool eatChess)
{
    // ���̵İ˸�����
    int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    int temp_x = x, temp_y = y;             // ��ʱ����������������λ��
    int i = 0, eatNum = 0;                  // ��ʼ������
    typedef int (*p) [gridNum];             // �Զ�������
    p chessBoard = p(chess);                // ����ת��

    if(chessBoard[temp_x][temp_y] != ChessBoard::Empty)  // ����˷������������ӣ�����;
        return 0;

    // ���̵�8������
    for(i = 0 ; i <8; i++)
    {
        temp_x += dir[i][0]; temp_y += dir[i][1];	// ׼���ж���������
        // ���û�г��磬�����������ǶԷ����ӣ����г��ӵĿ��ܣ�
        if((temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            && (chessBoard[temp_x][temp_y] != currentRole) && (chessBoard[temp_x][temp_y] != ChessBoard::Empty) )
        {
            temp_x += dir[i][0]; temp_y += dir[i][1];	            // �����ж���һ������ǰ��һ��
            while(temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            {
                if(chessBoard[temp_x][temp_y] == ChessBoard::Empty) // ������λ����
                        break;
                if(chessBoard[temp_x][temp_y] == currentRole)       // �ҵ��Լ������ӣ�������Գ���
                {
                    if(eatChess == true)                            // ȷ������
                    {

                        chessBoard[x][y] = currentRole;          // ��ʼ���־Ϊ�Լ�������
                        temp_x -= dir[i][0]; temp_y -= dir[i][1];// ����һ��
                        while((temp_x != x )||(temp_y != y))	 // ֻҪû�лص���ʼ��λ�þ�ִ��
                        {
                            chessBoard[temp_x][temp_y] = currentRole;   // ��־Ϊ�Լ�������
                            temp_x -= dir[i][0]; temp_y -= dir[i][1];   // ��������һ��
                            eatNum ++;  // �ۼ�
                        }
                    }
                    else    // �����ӣ�ֻ���ж����λ���ܲ��ܳ���
                    {

                        temp_x -= dir[i][0]; temp_y -= dir[i][1];   // ����һ��
                        while((temp_x != x )||(temp_y != y))        // ֻ������Գ��ӵĸ���
                        {
                            temp_x -= dir[i][0]; temp_y -= dir[i][1];// ��������һ��
                            eatNum ++;
                        }
                    }
                    break;  // ����ѭ��
                }// û���ҵ��Լ������ӣ�����ǰ��һ��
                temp_x += dir[i][0]; temp_y += dir[i][1];   // ��ǰ��һ��
            }
        }// �����������ܳ��ӣ��ͻ�һ������
        temp_x = x; temp_y = y;
    }

    return eatNum;              // �����ܳ��ӵĸ���
}

// ��ʾ��������
void Widget::showScore()
{
   int blackNum=0;
   int whiteNum=0;
   bool isOver = true;
   // ���̵����ӱ���һ�飬�ۼƺڰ�����Եĸ���
    for(int i=0;i<gridNum;i++)
    {
        for(int j=0;j<gridNum;j++)
        {
            if(chess[step][i][j] == ChessBoard::White)      // ����
              whiteNum++;
            else if(chess[step][i][j] == ChessBoard::Black) // ����
               blackNum++;

            if(0 < judgeRule(i,j,gridNum, (void *)chess[step],ChessBoard::Black, false) ||
               0 < judgeRule(i,j,gridNum, (void *)chess[step],ChessBoard::White, false)){
                isOver = false;
            }
        }
    }
    // �ֱ���ʾ����Ӧ��LCD��
    ui->lcdNumberBlack->display(blackNum);
    ui->lcdNumberWhite->display(whiteNum);

    if(false == isOver){
        return;
    }

    // �ж���Ӯ
    if(blackNum > whiteNum){
        QMessageBox::information(this,"win","black win");
    }else if(blackNum < whiteNum){
        QMessageBox::information(this,"win","white win");
    }else{
        QMessageBox::information(this,"win","the same");
    }
    this->on_toolButtonOver_clicked();  // ��Ϸ��������


}

// ��ͼ�¼���������Ҫ�ǻ������ڵı���ͼƬ
void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap("../image/background.jpg"));
}

// ��ʼ��ť�Ĳ���
void Widget::on_toolButtonStart_clicked()
{
   if(this->isStart == false)   // �����Ϸû�п�ʼ
    {   // ��Ӧ������ʹ������
        ui->toolButtonStart->setEnabled(false);
        ui->toolButtonMode->setEnabled(false);
        ui->toolButtonOver->setEnabled(true);
        ui->toolButtonJump->setEnabled(true);
        ui->toolButtonGoback->setEnabled(true);
        this->isStart = true;   // ��־��Ϸ�Ѿ���ʼ��
        this->times=30;         // ʱ���ʼ��Ϊ30s
        ui->lcdNumber->display(times);  // LED��ʾʱ��
        this->timer.start(1000);        // ��ÿ��1S����ʱ��
        this->board->show();            // ��ʾ����
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

// ʱ��Ĵ�������ÿ1Sִ��һ��
void Widget::timerSlot()
{
    this->times--;          // ÿ�μ�1
    if(this->times == -1)   // ʱ�䵽�͸ı����ӵĽ�ɫ
        changeRole();       // �ı����ӵĽ�ɫ
    ui->lcdNumber->display(this->times);    // ʱ����ʾ��LCD��
}

// �ı����ӵĽ�ɫ
void Widget::changeRole()
{
    this->times=30;     // ʱ������Ϊ30S
    // �����ǰΪ���ӣ��ͻ�Ϊ����
    if(this->currentRole == ChessBoard::White)
        this->currentRole = ChessBoard::Black;
    else
        this->currentRole = ChessBoard::White;
    // ���µ�ʱ����ʾ��LCD��
    ui->lcdNumber->display(this->times);

    // ������˻���սģʽ�����ҵ���������
    if(this->PVPFlag == false && this->currentRole == ChessBoard::White)
    {
        this->machineTime.start(1000);  // ����������ʱ��
    }
}

// ģʽת��
void Widget::on_toolButtonMode_clicked()
{
    // �����ǰ�����ˣ����º�ͱ�Ϊ�˻���ս�����ı���Ӧ������
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

// ���尴��
void Widget::on_toolButtonGoback_clicked()
{   // ��������˶�ս�������Ѿ�������
    if(this->PVPFlag == true && this->step >= 1)
    {
        this->changeRole(); // �ı����ӵĽ�ɫ
        step--;             // ��һ��
    }
    else if(this->PVPFlag == false && this->step >= 2 ) // ������˻���ս
        step = step -2;                                 // ������
    this->board->setChessManInfo((void *)chess[step]);  // �ı����̱�־λ
    this->board->update();  // ��������
}

// �ı����ӽ�ɫ��ť
void Widget::on_toolButtonJump_clicked()
{
    if(this->PVPFlag == false && this->currentRole == ChessBoard::White)
        return;                         // ����˻�ģʽ�������ǻ���, ���ܰ�

    this->changeRole();
}

// ��Ϸ������ť
void Widget::on_toolButtonOver_clicked()
{
    this->init();   // ��ʼ����������
    // LCD�����ݶ���ʾ0
    ui->lcdNumber->display(0);
    ui->lcdNumberBlack->display(0);
    ui->lcdNumberWhite->display(0);
    // ʱ��ֹͣ
    this->timer.stop();
    this->machineTime.stop();
}
