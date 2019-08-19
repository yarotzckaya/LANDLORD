//  landlord.hpp
//  LANDLORD
//
//  Created by Yuliana on 8/15/19.
//  Copyright © 2019 Yuliana. All rights reserved.

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>        // для sort()
#include <numeric>          // для accumulate()

using namespace std;

// глобальные методы

void getLine(string& str);      // получить строку текста
char getChar();                 // получить символ

///////////////////////////////////// класс жильцов /////////////////////////////////////

class Tenant
{
    private:
    
        string name;            // имя жильца
        int aptNumber;          // номер апартаментов, в которых он живет
        // здесь может быть полная информация о жильце - телефон и тд
        
    public:
    
        Tenant(string str, int n)
        {
            name = str;
            aptNumber = n;
        }
        
        ~Tenant();
        
        int getAptNumber();
        
        friend bool operator < (const Tenant&, const Tenant&);      // нужно для использования в множестве
        friend bool operator == (const Tenant&, const Tenant&);      
        friend ostream& operator << (ostream&, const Tenant&);      // нужно для операций ввода-вывода
};

///////////////////////// класс сравнения жильцов (функциональный объект) /////////////////////////////

class compareTenants
{
    public:
    
        bool operator() (Tenant*, Tenant*) const;
};

//////////////////////////////////// класс TenantList //////////////////////////////////////////

class TenantList
{
    private:
    
        set<Tenant*, compareTenants> pointersToTenants;     // указатели на класс жильцов
        set<Tenant*, compareTenants>::iterator pointersIterator;
        
    public:
    
        ~TenantList();                  // удаление жильцов
        
        void insertTenant(Tenant*);     // добавить жильца в список
        int getAptNo(string);           // возвращает номер апартаментов
        void display();                 // выводит список жильцов
};

//////////////////////////////// класс TenantInputScreen //////////////////////////////////

class TenantInputScreen
{
    private:
    
        TenantList* pointerToList;
        string tenantName;
        int aptNo;
        
    public:
    
        TenantInputScreen(TenantList* pointer) : pointerToList(pointer){}
        
        void getTenant();
};

//////////////////////////////////// класс RentRow /////////////////////////////////////////

// одна строка таблицы прибыли: адрес и 12 месячных оплат

class RentRow
{
    private:
    
        int aptNo;
        float rent[12];
        
    public:
    
        RentRow(int);
        
        void setRent(int, float);       // запись платы за месяц
        float getSumOfRow();            // сумма платежей из одной строки
        
        friend bool operator < (const RentRow&, const RentRow&);      // нужно для использования в множестве
        friend bool operator == (const RentRow&, const RentRow&);      
        friend ostream& operator << (ostream&, const RentRow&);      // нужно для операций ввода-вывода
};

//////////////////////// функциональный объект для сравнения объектов RentRows ////////////////////////

class compareRows
{
    public:
    
        bool operator() (RentRow*, RentRow*) const;
};

//////////////////////////////////////// класс RentRecord ////////////////////////////////////////

class RentRecord
{
    private:
    
        set<RentRow*, compareRows> pointerToRows;            // множество указателей на объекты RentRow
        set<RentRow*, compareRows>::iterator iter;
        
    public:
    
        ~RentRecord();
        
        void insertRent(int, int, float);
        void display();
        float getSumOfRents();
};

////////////////////////////////////// класс RentInputScreen //////////////////////////////////

class RentInputScreen
{
    private:
    
        TenantList* pointerToList;
        RentRecord* pointerToRecord;
        string renterName;
        float rentPaid;
        int month;
        int aptNo;
        
    public:
    
        RentInputScreen(TenantList* listPointer, RentRecord* recordPointer)
        {
            pointerToList = listPointer;
            pointerToRecord = recordPointer;
        }
        
        void setRent();     // арендная плата одного жильца за месяц
};

/////////////////////////////////////// класс Expense //////////////////////////////////////////

class Expense
{
    public:
    
        int month, day;
        string category; 
        string payee;
        float amount;
        
        Expense(){}
        
        Expense(int m, int d, string c, string p, float a)
        {
            month = m;
            day = d;
            category = c;
            payee = p;
            amount = a;
        }
        
        friend bool operator < (const Expense&, const Expense&);      // нужно для использования в множестве
        friend bool operator == (const Expense&, const Expense&);      
        friend ostream& operator << (ostream&, const Expense&);      // нужно для операций ввода-вывода

};

///////////////////////////////// функциональный объект сравнения затрат ///////////////////////////

class compareExpences
{
    public:
    
        bool operator () (Expense*, Expense*) const;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class compareCategories
{
    public:
    
        bool operator() (Expense* e1, Expense* e2) const;
};

///////////////////////////////////// класс ExpenseRecord //////////////////////////////////////

class ExpenseRecord
{
    private:
    
        vector<Expense*> pointerToExpences;         // массив указателей на расходы
        vector<Expense*>::iterator vectorIterator;
        
    public:
    
        ~ExpenseRecord();
        
        void insertExpense(Expense*);
        void display();
        float displaySummary();     // нужно для годового отчета
};

/////////////////////////////////// класс ExpenseInputScreen ///////////////////////////////

class ExpenseInputScreen
{
    private:
    
        ExpenseRecord* pointerToRecord;
        
    public:
    
        ExpenseInputScreen(ExpenseRecord*);
        
        void setExpense();
};

////////////////////////////////// класс AnnualReport ///////////////////////////////////

class AnnualReport
{
    private:
    
        RentRecord* pointerToRentRecord;
        ExpenseRecord* pointerToExpenseRecord;
        float expenses, rents;
        
    public:
    
        AnnualReport(RentRecord*, ExpenseRecord*);
        void display();
};

////////////////////////////// класс UserInterface ///////////////////////////////////////////

class UserInterface
{
    private:
    
        TenantList*         pointerToTenantList;
        TenantInputScreen*  pointerToTenantInputScreen;
        RentRecord*         pointerToRentRecord;
        RentInputScreen*    pointerToRentInputScreen;
        ExpenseRecord*      pointerToExpenseRecord;
        ExpenseInputScreen* pointerToExpenseInputScreen;
        AnnualReport*       pointerToAnnualReport;
        char ch;
        
    public:
    
        UserInterface();
        ~UserInterface();
        void interact();
};
