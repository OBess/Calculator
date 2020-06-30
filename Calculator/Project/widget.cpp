#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    setFixedSize(280, 426);
    setWindowIcon(QIcon(":/icon.ico"));

    ui->setupUi(this);
    ui->FieldAction->setMaxLength(limit);
    ui->FieldAction->setValidator( new QRegExpValidator( QRegExp( "[0-9+*/-().]{0,}" ) ) );
}

Widget::~Widget()
{
    delete ui;
}

//--*Check*--
bool Widget::isOperation(QChar c){
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}
bool Widget::noFirstZero(std::string str){
    return *(str.end() - 1) == '0' && str.length() == 1;
}
bool Widget::lastNoneZero(std::string str){
    if(str.length() == 0) return true;
    for (int i = str.length() - 1; i >= 0; i--) {
        if(isOperation(str[i])) return false;
        if(str[i] == '.') return true;
        if(str[i] != '0') return true;
    }
    return false;
}
bool Widget::noneElseDotInNumber(std::string str){
    if(*(str.end() - 1) == 46 || isOperation(*(str.end() - 1))) return false;
    for(auto it = str.rbegin(); it != str.rend(); it++){
        if(isOperation(*it)) break;
        if(*it == '.') return false;
    }
    return true;
}
bool Widget::matchFromOneToNine(std::string str){
    std::regex re("[1-9]");
    return std::regex_search(str, re);
}
bool Widget::matchDigit(std::string str){
    std::regex re("[0-9]");
    return std::regex_search(str, re);
}
bool Widget::matchBrackets(std::string str){
    std::regex re("[()]");
    return std::regex_search(str, re);
}
bool Widget::matchCorrectBrackets(std::string str){
    return std::count_if(str.begin(), str.end(), [](char n){return n == '(';}) == std::count_if(str.begin(), str.end(), [](char n){return n == ')';});
}
bool Widget::hasOperation(std::string str){
    std::regex re("[+*/-]");
    return std::regex_search(str, re);
}
//--/*Check/*--

//--*ButtonClickAcion*--
void Widget::btnAddCharToLabel(std::string c, QLineEdit* ql){
    if(noFirstZero(ql->text().toStdString())) ql->setText("");
    if(*(ql->text().end() - 1) == ')')
        ql->setText(ql->text() + '*' + c.c_str());
    else
        ql->setText(ql->text() + c.c_str());
}
void Widget::btnAddZeroToLabel(std::string c, QLineEdit* ql){
    if(lastNoneZero(ql->text().toStdString())) btnAddCharToLabel(c, ql);
}
void Widget::btnAddBracketsToLabel(QLineEdit* ql, std::string c = "()"){
    if(*(ql->text().end() - 1) != '.' && lastNoneZero(ql->text().toStdString())){
        if(ql->text().length() == 0){
            ql->setText(ql->text() + c.c_str()[0]);
        }else if(matchDigit(ql->text().toStdString().substr(ql->text().length() - 1)) && !matchCorrectBrackets(ql->text().toStdString())){
            ql->setText(ql->text() + c.c_str()[1]);
        }else if(matchDigit(ql->text().toStdString().substr(ql->text().length() - 1)) && matchCorrectBrackets(ql->text().toStdString())){
            ql->setText(ql->text() + '*' + c.c_str()[0]);
        }else if(isOperation(*(ql->text().toStdString().end() - 1))){
            ql->setText(ql->text() + c.c_str()[0]);
        }else if(*(ql->text().toStdString().end() - 1) == ')' && matchCorrectBrackets(ql->text().toStdString())){
            ql->setText(ql->text() + '*' + c.c_str()[0]);
        }else if(*(ql->text().toStdString().end() - 1) == ')' && !matchCorrectBrackets(ql->text().toStdString())){
            ql->setText(ql->text() + c.c_str()[1]);
        }else{
            ql->setText(ql->text() + c.c_str()[0]);
        }
    }
}
void Widget::btnAddDotToLabel(QLineEdit* ql, std::string c = "."){
    if(ql->text().length() == 0) { ql->setText("0."); return;}
    if(isOperation(*(ql->text().end() - 1))) { ql->setText(ql->text() + "0."); return;}
    if(noneElseDotInNumber(ql->text().toStdString()))
        ql->setText(ql->text() + c.c_str());
}
void Widget::btnAddCosToLabel(std::string c, QLineEdit* ql){
    if(ql->text().length() == 0 || (isOperation(*(ql->text().end() - 1)) && *(ql->text().end() - 1) != '-' && *(ql->text().end() - 2) != '(')){
        ql->setText(ql->text() + c.c_str());
        return;
    }else if(*(ql->text().end() - 1) == ')'){
        ql->setText(ql->text() + "*" + c.c_str());
        return;
    }
    else{
        for(int i = ql->text().length() - 1; i >= 0; i--){
            if(isOperation(ql->text()[i])){
                if(i != 0 && ql->text()[i] == '-' && ql->text()[i - 1] == '('){
                    std::string left = ql->text().toStdString().substr(0, i - 1);
                    std::string right = ql->text().toStdString().substr(i + 1, ql->text().length() - i + 1);
                    ql->setText(QString(left.c_str()) + QString(right.c_str()));
                }else{
                    std::string left = ql->text().toStdString().substr(0, i + 1);
                    std::string right = ql->text().toStdString().substr(i + 1, ql->text().length() - i);
                    ql->setText(QString(left.c_str()) + c.c_str() + QString(right.c_str()));
                }
                return;
            }
        }
        ql->setText(QString(c.c_str() + ql->text()));
    }
}
void Widget::btnAddOperationToLabel(std::string c, QLineEdit* ql) {
    if(*(ql->text().toStdString().end() - 1) == '.') return;
    if(lastNoneZero(ql->text().toStdString())){
        if(noFirstZero(ql->text().toStdString())) ql->setText("");
        if(!ql->text().toStdString().empty()){
            if(!isOperation(*(ql->text().toStdString().end() - 1))){
                ql->setText(ql->text() + c.c_str());
            }
            else {
                std::string str = ql->text().toStdString();
                ql->setText(QString(str.substr(0, str.length() - 1).c_str()) + c.c_str());
            }
        }
    }
}
void Widget::btnSolve(QLineEdit* qlF, QLabel* qlRes){
    if(!isOperation(*(qlF->text().end() - 1)) && qlF->text().length() != 0 && matchFromOneToNine(qlF->text().toStdString())
            && matchCorrectBrackets(qlF->text().toStdString())){
        std::stringstream ss1;
        std::string str = qlF->text().toStdString();
        double res = distribution(str);
        ss1 << res;
        qlRes->setText(QString("= ") + ss1.str().c_str());
    }
}
//--/*ButtonClickAcion/*--

//--*Logic*--
long double Widget::actionOperaion(char oper, long double a, long double b){
    switch (oper) {
    case '+':
        return a + b;
    case '-':
        return a - b;
     case '*':
        return a * b;
     case '/':
        return a / b;
     case '%':
        return a * (b / 100);
    default:
        return 0;
    }
}
long double Widget::distribution(std::string& str){
    char operation[][2] = {{'*', '/'}, {'%', ' '}, {'+', '-'}};
    int level = 0;
    double res = 0.0f;
    try {
        from:
        if(matchBrackets(str))
        {
            int countOpenBrackects = 0;
            int countCloseBrackects = 0;
            for(size_t i = 0; i < str.length(); i++){
                if(str[i] == '('){
                    for(int j = i; j < str.length(); j++){
                        if(str[j] == '(') countOpenBrackects++;
                        if(str[j] == ')') countCloseBrackects++;
                        if(countOpenBrackects == countCloseBrackects){
                            std::string disStr = str.substr(i + 1, j - i - 1);
                            str = str.substr(0, i) + std::to_string(distribution(disStr)) + str.substr(j + 1, str.length() - j);
                            goto from;
                        }
                    }
                }
            }
        }
        while (str.length() > 0) {
            int index = str.find(operation[level][0]);
            if(operation[level][1] != ' ' && (str.find(operation[level][1]) < index || index == -1)) index = str.find(operation[level][1]);
            if((index == -1 || index == 0) && level == sizeof(operation)/sizeof(char)/2-1){
                if(hasOperation(str.substr(1, str.length() - 2))){
                    str[0] = '_';
                    continue;
                }
                res = std::stof(str);
                break;
            }
            if(index == -1) { ++level; continue; }
            int start = index, end = index;
            for(int i = index - 1; i >= 0; i--){
                if(isOperation(str[i])){
                    if(i != 0 && (str[i - 1] == '(' || isOperation(str[i - 1]))) start = i - 1;
                    if(i == 0) start = 0;
                    break;
                }
                start = i;
            }
            for(int i = index + 1; i < str.length(); i++){
                if(isOperation(str[i]) && index != i - 1) break;
                end = i;
            }
            std::string left = str.substr(start, index - start);
            std::string right = str.substr(index + 1, end - index);
            if(left[0] == '_') left[0] = '-';
            res = actionOperaion(str[index], std::stod(left), std::stod(right));
            str = str.substr(0, start) + std::to_string(res) + str.substr(end + 1);
        }
    } catch (std::exception e) {
        return 0;
    }
    return res;
}
void Widget::keyPressEvent(QKeyEvent* e){
    if(e->text() == "\r"){
        on_resBTN_clicked();
    }
}
//--/*Logic/*--

void Widget::on_pushButton_18_clicked()
{
    btnAddZeroToLabel("0", ui->FieldAction);
}

void Widget::on_pushButton_13_clicked()
{
    btnAddCharToLabel("1", ui->FieldAction);
}

void Widget::on_pushButton_14_clicked()
{
    btnAddCharToLabel("2", ui->FieldAction);
}

void Widget::on_pushButton_15_clicked()
{
    btnAddCharToLabel("3", ui->FieldAction);
}

void Widget::on_pushButton_9_clicked()
{
    btnAddCharToLabel("4", ui->FieldAction);
}

void Widget::on_pushButton_10_clicked()
{
    btnAddCharToLabel("5", ui->FieldAction);
}

void Widget::on_pushButton_11_clicked()
{
    btnAddCharToLabel("6", ui->FieldAction);
}

void Widget::on_pushButton_5_clicked()
{
    btnAddCharToLabel("7", ui->FieldAction);
}

void Widget::on_pushButton_6_clicked()
{
    btnAddCharToLabel("8", ui->FieldAction);
}

void Widget::on_pushButton_7_clicked()
{
    btnAddCharToLabel("9", ui->FieldAction);
}

void Widget::on_pushButton_16_clicked()
{
    btnAddOperationToLabel("+", ui->FieldAction);
}

void Widget::on_pushButton_12_clicked()
{
    btnAddOperationToLabel("-", ui->FieldAction);
}

void Widget::on_pushButton_8_clicked()
{
    btnAddOperationToLabel("*", ui->FieldAction);
}

void Widget::on_pushButton_clicked()
{
    btnAddOperationToLabel("/", ui->FieldAction);
}

void Widget::on_pushButton_2_clicked()
{
    btnAddOperationToLabel("%", ui->FieldAction);
}

void Widget::on_pushButton_20_clicked()
{
    btnAddDotToLabel(ui->FieldAction);
}

void Widget::on_pushButton_19_clicked()
{
    if(ui->FieldAction->text().length() > 0)
        ui->FieldAction->setText(ui->FieldAction->text().remove(ui->FieldAction->text().length() - 1, 1));
}

void Widget::on_cBTN_clicked()
{
    ui->FieldAction->setText("");
    ui->ResultLabel->setText("=");
}

void Widget::on_resBTN_clicked()
{
    btnSolve(ui->FieldAction, ui->ResultLabel);
}

void Widget::on_pros_cosBTN_clicked()
{
    btnAddCosToLabel("(-", ui->FieldAction);
}

void Widget::on_bracketsBTN_clicked()
{
    btnAddBracketsToLabel(ui->FieldAction);
}
