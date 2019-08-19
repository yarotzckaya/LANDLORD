//  landlord.cpp
//  LANDLORD
//
//  Created by Yuliana on 8/15/19.
//  Copyright © 2019 Yuliana. All rights reserved.
//

#include "landlord.hpp"

void getLine(string& str)           // получение строки текста
{
    char temp[21];
    cin.get(temp, 20, '\n');
    cin.ignore(20, '\n');
    str = temp;
}


char getChar()                      // получение символа
{
    char ch = getchar();
    cin.ignore(80, '\n');
    
    return ch;
}

///////////////////////////////// методы класса Tenant //////////////////////////

Tenant::~Tenant(){}

int Tenant::getAptNumber()
{
    return aptNumber;
}

bool operator < (const Tenant& t1, const Tenant& t2)
{
    return t1.name < t2.name;
}

bool operator == (const Tenant& t1, const Tenant& t2)
{
    return t1.name == t2.name;
}

ostream& operator << (ostream& s, const Tenant& t)
{
    s << t.aptNumber << '\t' << t.name << endl;
    return s;
}


//////////////////////////////// методы класса TenantInputScreen ///////////////////////////

void TenantInputScreen::getTenant()
{
    cout << "Введите имя жильца (Кот Матроскин): ";
    getLine(tenantName);
    cout << "Введите номер апартаментов: ";
    cin >> aptNo;
    
    cin.ignore(80, '\n');
    Tenant* tenant = new Tenant(tenantName, aptNo);     // создать жильца
    pointerToList->insertTenant(tenant);                // занести в список жильцов
}

/////////////////////////////////////////////////////////////////////////////////////

bool compareTenants::operator () (Tenant* t1, Tenant* t2) const
{
    return *t1 < *t2;
}

///////////////////// методы класса TenantList ///////////////////////////////////

TenantList::~TenantList()
{       
    while(!pointersToTenants.empty()) {                    // удаление всех жильцов
        pointersIterator = pointersToTenants.begin();
        delete *pointersIterator;
        pointersToTenants.erase(pointersIterator);
    }
}

void TenantList::insertTenant(Tenant* pointer)
{
    pointersToTenants.insert(pointer);      // вставка
}

int TenantList::getAptNo(string name)
{
    int aptNo;
    Tenant dummy(name, 0);
    
    pointersIterator = pointersToTenants.begin();
    
    while(pointersIterator != pointersToTenants.end()) {
        aptNo = (*pointersIterator)->getAptNumber();        //  поиск жильца в списке
        
        if(dummy == **pointersIterator++) {                 // если он есть - возвращаем номер апартаментов
            return aptNo;
        }
    }  
    
    return -1;                                              // если нет - возвращаем -1
}

void TenantList::display()                                  // вывод списка жильцов
{
    cout << "\nApt# \tИмя жильца\n ------------------\n";
    if(pointersToTenants.empty()) {                         // если список жильцов пуст
        cout << "Нет жильцов" << endl;
    } else {
        pointersIterator = pointersToTenants.begin();
        
        while(pointersIterator != pointersToTenants.end()) {
           cout << "    " << **pointersIterator++; 
        }
    }
}


//////////////////////////////////////// методы класса RentRow ////////////////////////////

RentRow::RentRow(int an) : aptNo(an)
{
    fill(&rent[0], &rent[12], 0);
}

void RentRow::setRent(int m, float am)
{
    rent[m] = am;
}

float RentRow::getSumOfRow()        // cумма арендных платежей в строке
{
    return accumulate(&rent[0], &rent[12], 0);
}

bool operator < (const RentRow& t1, const RentRow& t2)
{
    return t1.aptNo < t2.aptNo;
}

bool operator == (const RentRow& t1, const RentRow& t2)
{
    return t1.aptNo == t2.aptNo;
}

ostream& operator << (ostream& s, const RentRow& an)                // распечатать строку арендной платы для одного жильца за 12 месяцев
{
    s << an.aptNo << '\t';              // вывести номер комнаты
    
    for(int i = 0; i < 12; i++) {       // вывести 12 арендных платежей
        if(an.rent[i] == 0) {
            s << " 0 ";
        } else {
            s << an.rent[i] << " ";
        }
    }
    
    s << endl;
    
    return s;
}

///////////////////////////////////////////////////////////////////////////////

bool compareRows::operator () (RentRow* pointer1, RentRow* pointer2) const
{
    return *pointer1 < *pointer2;
}

////////////////////////////// методы класса RentRecord ////////////////////////////

RentRecord::~RentRecord()
{
    while(!pointerToRows.empty()) {     // удалить строки с платежами
        iter = pointerToRows.begin();   // удалить указатели из множества
        delete *iter;   
        pointerToRows.erase(iter);
    }
}

void RentRecord::insertRent(int aptNo, int month, float amount)
{
    RentRow searchRow(aptNo);       // временная строка с тем же aptNo
    iter = pointerToRows.begin();   // поиск
    
    while(iter != pointerToRows.end()) {
        if(searchRow == **iter) {               // если RentRow найден, 
            (*iter)->setRent(month, amount);    // заносим строку в список
            return;
        } else {
            iter++;
        }        
    }
    RentRow* pointerToRow = new RentRow(aptNo); // если не нашли строку - создаем новую
    pointerToRow->setRent(month, amount);       // заносим в нее платеж
    pointerToRows.insert(pointerToRow);        // заносим строку в вектор
}

void RentRecord::display()
{
    cout << "\nAptNo#   Янв     Фев     Мар     Апр     Май     Июн    Июл    Авг   Сент    Окт     Нояб    Дек" << endl 
            << "-------------------------------------------------------------------------------------------------" << endl;
            
    if(pointerToRows.empty()) {
        cout << "Нет платежей" << endl;
    } else {
        iter = pointerToRows.begin();
        
        while(iter != pointerToRows.end()) {
            cout << "         " << **iter++;
        }
    }
}

float RentRecord::getSumOfRents()           // сумма всех платежей
{
    float sumRents = 0.0;
    iter = pointerToRows.begin();
    
    while(iter != pointerToRows.end()) {
        sumRents += (*iter)->getSumOfRow();
        iter++;
    }
    
    return sumRents;
}

/////////////////////////////////////// методы класса RentInputScreen ////////////////////////////////

void RentInputScreen::setRent()
{
    cout << "Введите имя жильца: ";
    getLine(renterName);
    
    aptNo = pointerToList->getAptNo(renterName);
    
    if(aptNo > 0) {         // если имя найдено
        cout << "Введите сумму платежа: (12.5) ";
        cin >> rentPaid;
        cin.ignore(80, '\n');
        cout << "Введите номер месяца оплаты: (1-12) ";
        cin >> month;
        cin.ignore(80, '\n');
        month--;        // (внутренняя нумерация 0-11)
        pointerToRecord->insertRent(aptNo, month, rentPaid);
    } else {
        cout << "Такого жильца нет" << endl;
    }
}

////////////////////////////////////// методы класса Expense //////////////////////////////////////

bool operator < (const Expense& e1, const Expense& e2)          // сравнивает даты
{
    if(e1.month == e2.month) {        // если тот же месяц - 
        return e1.day < e2.day;       // сравниваем дни
    } else {
        return e1.month < e2.month;   // иначе - сравнить месяцы
    }
}

bool operator == (const Expense& e1, const Expense& e2)
{
    return e1.month == e2.month && e1.day == e2.day;
}

ostream& operator << (ostream& s, const Expense& e)
{
    s << e.month << "/" << e.day << '\t' << e.payee << '\t' << e.amount << '\t' << e.category << endl;
    return s;
}

///////////////////////////////////////////////////////////////////////////////////////////////

bool compareExpences::operator()(Expense* e1, Expense* e2) const
{
    return *e1 < *e2;
}

bool compareCategories::operator()(Expense *e1, Expense *e2) const
{
    return e1->category < e2->category;
}

//////////////////////////////////// методы класса ExpenseRecord ///////////////////////////

ExpenseRecord::~ExpenseRecord()
{
    while(!pointerToExpences.empty()) {
        vectorIterator = pointerToExpences.begin();
        delete *vectorIterator;
        pointerToExpences.erase(vectorIterator);
    }
}

void ExpenseRecord::insertExpense(Expense * pointer)
{
    pointerToExpences.push_back(pointer);
}

void ExpenseRecord::display()
{
    cout << "\nДата     Получатель     Сумма    Категория\n";
    cout << "-----------------------------------------" << endl;
    
    if(pointerToExpences.size() == 0) {
        cout << "Расходов нет" << endl;
    } else {
        sort(pointerToExpences.begin(), pointerToExpences.end(), compareExpences());        // сортировка по дате
        vectorIterator = pointerToExpences.begin();
        
        while(vectorIterator != pointerToExpences.end()) {
            cout << "    " << **vectorIterator++;
        }
    }
}

float ExpenseRecord::displaySummary()           // используется при составлении годового отчета
{
    float totalExpenses = 0;                    // сумма, все категории
    
    if(pointerToExpences.size() == 0) {
        cout << "   Все категории \n";
        return 0;
    }
    
    sort(pointerToExpences.begin(), pointerToExpences.end(), compareCategories());      // сортировать расходы по категориям
    
    // по каждой категории - сумма записей
    
    vectorIterator = pointerToExpences.begin();
    string tempCategory = (*vectorIterator)->category;
    float sum = 0.0;
    
    while(vectorIterator != pointerToExpences.end()) {
        if(tempCategory == (*vectorIterator)->category){       // та же категория
            sum += (*vectorIterator)->amount;
        } else {
            cout << '\t' << tempCategory << '\t' << sum << endl;
            totalExpenses += sum;                           // прибавить предыдущую категорию
            
            tempCategory = (*vectorIterator)->category;
            sum = (*vectorIterator)->amount;                // прибавить конечное значение суммы
        }
        
        vectorIterator++;
    }
    
    totalExpenses += sum;           // прибавить сумму конечной категории
    cout << '\t' << tempCategory << '\t' << sum << endl;
    return totalExpenses;
}

//////////////////////////////////// методы класса ExpenseInputScreen ///////////////////////////////

ExpenseInputScreen::ExpenseInputScreen(ExpenseRecord* per) : pointerToRecord(per) {}

void ExpenseInputScreen::setExpense()
{
    int month, day;
    string category, payee;
    float amount;
    
    cout << "Введите месяц: (1-12) ";
    cin >> month;
    cin.ignore(80, '\n');
    cout << "Введите день: (1-31) ";
    cin >> day;
    cin.ignore(80, '\n');
    cout << "Введите категорию расходов: (Ремонт, Налоги) ";
    getLine(category);
    cout << "Введите получателя: (ЭлектроСбыт) ";
    getLine(payee);
    cout << "Введите сумму (39.5) ";
    cin >> amount;
    cin.ignore(80, '\n');
    
    Expense* pointer = new Expense(month, day, category, payee, amount);
    pointerToRecord->insertExpense(pointer);
}

///////////////////////////////////// методы класса AnnualReport ////////////////////////////////////

AnnualReport::AnnualReport(RentRecord* pointerToRentRec, ExpenseRecord* pointerToExpenseRec)
{
    pointerToRentRecord = pointerToRentRec;
    pointerToExpenseRecord = pointerToExpenseRec;
}

void AnnualReport::display()
{
    cout << "Годовой отчет" << endl;
    cout << "-------------------" << endl;
    cout << "Доходы:     " << "\nАрендная плата    " << endl;
    rents = pointerToRentRecord->getSumOfRents();
    cout << "       " << rents << endl;
    cout << "Расходы    " << endl;
    expenses = pointerToExpenseRecord->displaySummary();
    cout << "\nБаланс   " << rents - expenses << endl;
}

////////////////////////////////////// методы класса UserInterface //////////////////////////////////

UserInterface::UserInterface()
{
    // это жизненно важно для программы
    pointerToTenantList     = new TenantList;
    pointerToRentRecord     = new RentRecord;
    pointerToExpenseRecord  = new ExpenseRecord;
}

UserInterface::~UserInterface()
{
    delete pointerToTenantList;
    delete pointerToRentRecord;
    delete pointerToExpenseRecord;
}

void UserInterface::interact()
{
    while(true) {
        cout << "Для ввода данных нажмите i, \n d - для ввода отчета, \n q - для выхода: ";
        ch = getChar();
        
        if(ch == 'i') {
            cout << " t - для добавления жильца, r - для записи арендной платы, e - для записи расходов: ";
            ch = getChar();
            
            switch(ch) {
                // экраны ввода существуют только во время их использования
                case 't':
                    pointerToTenantInputScreen = new TenantInputScreen(pointerToTenantList);
                    pointerToTenantInputScreen->getTenant();
                    delete pointerToTenantInputScreen;
                    break;
                case 'r':
                    pointerToRentInputScreen = new RentInputScreen(pointerToTenantList, pointerToRentRecord);
                    pointerToRentInputScreen->setRent();
                    delete pointerToRentInputScreen;
                    break;
                case 'e':
                    pointerToExpenseInputScreen = new ExpenseInputScreen(pointerToExpenseRecord);
                    pointerToExpenseInputScreen->setExpense();
                    delete pointerToExpenseInputScreen;
                    break;
                default:
                    cout << "Неизвестная функция " << endl;
                    break;
            }
        } else if(ch == 'd') {
            cout << "t - для вывода жильцов, r - для вывода арендной платы, е - для вывода расходов, а - для вывода годового отчета: ";
            ch = getChar();
            
            switch(ch) {
                case 't':
                    pointerToTenantList->display();
                    break;
                case 'r':
                    pointerToRentRecord->display();
                    break;
                case 'e':
                   pointerToExpenseRecord->display();
                    break;
                case 'a':
                    pointerToAnnualReport = new AnnualReport(pointerToRentRecord, pointerToExpenseRecord);
                    pointerToAnnualReport->display();
                    delete pointerToAnnualReport;
                    break;
                default:
                    cout << "Неизвестная функция " << endl;
                    break;
            }
        } else if(ch == 'q') {
            return;
        } else {
            cout << "Неизвестная функция" << endl;
        }
    }
}
