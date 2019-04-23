#ifndef EIGHTDIGIT_H
#define EIGHTDIGIT_H

#include <QPushButton>
#include <QThread>
#include <QTimer>
#include <QWidget>
#include <random>

using namespace std;

namespace Ui {
class EightDigit;
}


class EightDigit : public QWidget
{
    Q_OBJECT

public:
    explicit EightDigit(QWidget *parent = 0);
    ~EightDigit();
public slots:
    void moveSpace();
    void updateProgress();
private slots:
    void on_generate_btn_clicked();
    void on_AI_solution_btn_clicked();
private:
    Ui::EightDigit *ui;

    //被移动的格子的数字
    int num;
    //更新数据
    void updateData(int spaceX, int spaceY);
protected:
    //界面九宫格对应的数字
    int digit[9];
    //数字对应的按钮，空格对应0
    QPushButton* button[9];
    //计时器
    QTimer* timer;
    //按钮的大小
    int button_size;
    //速度
    int speed;
    //之前的x，y
    int preX,preY;
    //步骤(x,y)
    int step[32][2];
    //步骤当前下标
    int step_i;
    //步骤的长度
    int step_size;
    //随机
    random_device rd;

    /**
     * @brief 根据x和y获得在数组中的位置
     * @param x x坐标
     * @param y y坐标
     * @return 在数组中的位置
     */
    int getIDByXY(int x,int y);
    /**
     * @brief 将下标为i的按钮，移动到(x,y)
     * @param i 下标
     * @param x 移动的x
     * @param y 移动的y
     */
    void moveButtonToPoint(int i,int x,int y);

    /**
     * @brief 获得格子在九宫格的位置
     * @param i 下标
     * @param x 用来返回的x
     * @param y 用来返回的y
     */
    void getPosById(int i,int &x,int &y);
    /**
     * @brief 判断是否移动到了对应位置
     * @return true/false
     */
    bool moveReached();
    /**
     * @brief 判断是否移动完了8数码
     * @return true/false
     */
    bool solved();

    /**
     * @brief 获得棋盘的逆序数的奇偶性
     * @return 0或者1
     */
    int getInv();

    /**
     * @brief 根据哈希值生成局面
     * @param hash 哈希值
     */
    void reverseCantor(int hash);
};

#endif // EIGHTDIGIT_H
