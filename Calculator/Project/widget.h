#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QLineEdit>
#include <QLabel>
#include <regex>
#include <string>
#include <algorithm>
#include <QRegExpValidator>
#include <QKeyEvent>
#include <QIcon>
#include <QClipboard>

#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void keyPressEvent(QKeyEvent*);

private slots:
    void on_pushButton_18_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_19_clicked();

    void on_cBTN_clicked();

    void on_resBTN_clicked();

    void on_pros_cosBTN_clicked();

    void on_bracketsBTN_clicked();

private:
    //--*Variable*--
    const int limit = 26;
    //--/*Variable/*--

    //--*Objects*--
     Ui::Widget *ui;
     QClipboard *clipboard;
    //--/*Objects/*--

    //--*Check*--
    bool isOperation(QChar);
    bool noFirstZero(std::string);
    bool lastNoneZero(std::string);
    bool noneElseDotInNumber(std::string);
    bool matchFromOneToNine(std::string);
    bool matchDigit(std::string);
    bool matchBrackets(std::string);
    bool matchCorrectBrackets(std::string);
    bool hasOperation(std::string);
    //--/*Check/*--

    //--*ButtonClickAcion*--
    void btnAddCharToLabel(std::string, QLineEdit*);
    void btnAddZeroToLabel(std::string, QLineEdit*);
    void btnAddBracketsToLabel(QLineEdit*, std::string);
    void btnAddDotToLabel(QLineEdit*, std::string);
    void btnAddCosToLabel(std::string, QLineEdit*);
    void btnAddOperationToLabel(std::string, QLineEdit*);
    void btnSolve(QLineEdit*, QLabel*);
    //--/*ButtonClickAcion/*--

    //--*Logic*--
    long double actionOperaion(char, long double, long double);
    long double distribution(std::string&);
    //--/*Logic/*--
};
#endif // WIDGET_H
