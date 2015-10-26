#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

class File
{
public:
    File(int number, string catalog, string name, string suffix, string date, string time, bool atr[], int countOfSectors);
    File* next = NULL;
    File* previous = NULL;

private:
    int number;
    string catalog;
    string name;
    string suffix;
    string date;
    string time;
    bool atr[3];
    int countOfSectors;

public:
    bool checkMemory(int countOfFreeSectors);
    string getName(){return this->name;}
    string getSuffix(){return this->suffix;}
    string getCatalog(){return this->catalog;}
    string getDate(){return this->date;}
    string getTime(){return this->time;}
    bool* getAtr(){return this->atr;}
    int getCountOfSectors(){return this->countOfSectors;}
};

class FilesList
{
private:
    File *ptrAtBegin;
    File *ptrAtEnd;


public:
    void addFile(string catalog, string name, string suffix, string date, string time, bool atr[], int countOfSectors);
    bool deleteFile(string name);
    void getInfoAboutFiles();
    void sortByCatalog();
    void sortByDate();
    void swap(File*, File*);
    bool findAtCatalog(string catalog);
    bool findAtDate(string date);
    bool checkForCopy(string name, int sec);
    void getInfo(string name);
    int size = 0;
};
//------------------------------------------------------------------------




File::File(int number, string catalog, string name, string suffix, string date, string time, bool atr[], int countOfSectors)
{
    this->number = number;
    this->catalog = catalog;
    this->name = name;
    this->suffix = suffix;
    this->date = date;
    this->time = time;
    this->atr[0] = atr[0];
    this->atr[1] = atr[1];
    this->atr[2] = atr[2];
    this->countOfSectors = countOfSectors;
}

bool File::checkMemory(int countOfFreeSectors)
{
    if (countOfFreeSectors >= this->countOfSectors)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void FilesList::addFile(string catalog, string name, string suffix, string date, string time, bool atr[], int countOfSectors)
{
    File *newFile = new File(this->size, catalog, name, suffix, date, time, atr, countOfSectors);

    if (size > 0)
    {
        newFile->previous = this->ptrAtEnd;
        this->ptrAtEnd->next = newFile;
        this->ptrAtEnd = newFile;
    }
    else
    {
        this->ptrAtBegin = newFile;
        this->ptrAtEnd = newFile;
    }

    size++;
}

bool FilesList::deleteFile(string name)
{
    File *current;
    current = this->ptrAtBegin;

    while (current != NULL)
    {
        if ( (current->getName() + "." + current->getSuffix()) == name)
        {
            if (current != ptrAtBegin)
            {
                if (current == ptrAtEnd)
                    ptrAtEnd = current->previous;

                current->previous->next = current->next;
            }
            else
            {
                ptrAtBegin = current->next;
            }

            delete current;
            size--;
            return true;
        }
        else
        {
            current = current->next;
        }
    }

    return false;
}

void FilesList::getInfoAboutFiles()
{
    File *current = ptrAtBegin;

    while(current != NULL)
    {
        cout << "Имя файла: " << current->getName() << "." << current->getSuffix() << endl;
        cout << "Каталог: " << current->getCatalog() << endl;
        cout << "Дата создания: " << current->getDate() << endl;
        cout << "Время создания: " << current->getTime() << endl;
        bool *atr = current->getAtr();
        if (atr[0]) {cout << "Только для чтения" << endl;}
        if (atr[1]) {cout << "скрытый" << endl;}
        if (atr[2]) {cout << "системный" << endl;}
        cout << "Размер: " << current->getCountOfSectors() * 512  << " байт" << endl;
        cout << endl;

        current = current->next;
    }
}

void FilesList::swap(File *first, File *second)
{
    bool arg[3] = {0,0,0};
    File *temp = new File(10,"","","","","",arg,0);
    *temp = *first;

    if (first->previous != NULL) {first->previous->next = second;}
    first->next = second->next;
    first->previous = second;

    if (second->next != NULL) {second->next->previous = first;}
    second->next = first;
    second->previous = temp->previous;

    if (second == ptrAtEnd) {ptrAtEnd = first;}
    if (first == ptrAtBegin) {ptrAtBegin = second;}

}

void FilesList::sortByCatalog()
{
    bool needToStop = false;
    File *current;
    while (!needToStop)
    {
       needToStop = true;
       current = ptrAtBegin;
       while(current->next != NULL)
       {
           if (current->next->getCatalog() < current->getCatalog())
           {
               swap(current, current->next);
               needToStop = false;
           }
           else
           {
           current = current->next;
           }

       }
    }
}

void FilesList::sortByDate()
{
    bool needToStop = false;
    File *current;
    string temp1, temp2,hour1, hour2, minute1, minute2, day1,day2,month1,month2,year1,year2;

    while (!needToStop)
    {
       needToStop = true;
       current = ptrAtBegin;
       while(current->next != NULL)
       {
           temp1 = current->getDate();
           temp2 = current->next->getDate();

           day1 = temp1[0] + temp1[1];
           month1 = temp1[3] + temp1[4];
           for (int i = 6; i<10; i++)
           {
               year1 +=temp1[i];
           }

           temp1 = current->getTime();
           hour1 = temp1[0];
           hour1 += temp1[1];
           minute1 = temp1[3];
           minute1 += temp1[4];

           day2 = temp2[0] + temp2[1];
           month2 = temp2[3] + temp2[4];
           for (int i = 6; i<10; i++)
           {
               year2 +=temp2[i];
           }
           temp2 = current->next->getTime();
           hour2 = temp2[0];
           hour2+=temp2[1];
           minute2 = temp2[3];
           minute2 +=temp2[4];


           if (year1 > year2 || month1 > month2 || day1 > day2 || hour1 > hour2 || minute1 > minute2 )
           {
               swap(current, current->next);
               needToStop = false;
           }
           else
           {
           current = current->next;
           }
       }
    }
}

bool FilesList::findAtCatalog(string catalog)
{
    File *current;
    current = ptrAtBegin;
    bool flag = false;
    cout << "Поиск по каталогу " << catalog <<":" << endl;

    while (current != NULL)
    {
        if (current->getCatalog() == catalog)
        {
            flag = true;
            cout << current->getName() + "." + current->getSuffix() << endl;
        }
        current = current->next;
    }

    if (flag)
    {
        return true;
    }
    else
    {
    cout << "Файлы не найдены." << endl;
    return false;
    }
}

bool FilesList::findAtDate(string date)
{
    File *current;
    bool flag = false;
    current = ptrAtBegin;
    cout << "Поиск по дате " << date <<":" << endl;

    while (current != NULL)
    {
        if (current->getDate() == date)
        {
            cout << current->getName() + "." + current->getSuffix() << endl;
            flag = true;
        }
        current = current->next;
    }

    if (flag)
    {
        return true;
    }
    else
    {
    cout << "Файлы не найдены" << endl;
    return false;
    }
}

bool FilesList::checkForCopy(string fullName, int sec)
{
    string suffix, name;
    File *current;
    current = ptrAtBegin;
    for (int i = fullName.size() - 1; i>= 0; i--)
    {
        if (fullName[i] == '.')
        {
           for (int j = i+1; j <fullName.size();j++)
           {
               suffix += fullName[j];
           }
           for (int j = 0; j <i;j++)
           {
               name += fullName[j];
           }
           break;
        }
    }

    while (current != NULL)
    {
        if (current->getName() == name && current->getSuffix() == suffix)
        {
            if (current->checkMemory(sec)){
            return true;
            }
            else
            {
                cout << "Файл не поместится" << endl ;
                return false;
            }
        }
    }

    cout << "Файл не найден" << endl;
    return false;
}

void FilesList::getInfo(string fullName)
{
    bool flag = false;
    string suffix, name1;
    for (int i = fullName.size() - 1; i>= 0; i--)
    {
        if (fullName[i] == '.')
        {
           for (int j = i+1; j <fullName.size();j++)
           {
               suffix += fullName[j];
           }
           for (int j = 0; j <i;j++)
           {
               name1 += fullName[j];
           }
           break;
        }
    }
    File *current;
    current = ptrAtBegin;

    while (current != NULL)
    {
        if (current->getName() == name1 && current->getSuffix() == suffix)
        {
            cout << "Имя файла: " << current->getName() << "." << current->getSuffix() << endl;
            cout << "Каталог: " << current->getCatalog() << endl;
            cout << "Дата создания: " << current->getDate() << endl;
            cout << "Время создания: " << current->getTime() << endl;
            bool *atr = current->getAtr();
            if (atr[0]) {cout << "Только для чтения" << endl;}
            if (atr[1]) {cout << "скрытый" << endl;}
            if (atr[2]) {cout << "системный" << endl;}
            cout << "Размер: " << current->getCountOfSectors() * 512  << " байт" << endl;
            cout << endl;
            flag = true;
            break;
        }
        current = current->next;
    }

    if (!flag)
    {
        cout << "Файл не найден" << endl;
    }
}


//------------------------------------------------------------------------
int main()
{
    FilesList list;
    bool arg[3] = {0,0,0};
    int command, countOfSectors, sec;
    double size;
    bool flag = false;
    string catalog,fullName, name, suffix, date, time;


    cout << "Выберите команду для работы с файловой системой: " << endl;
    cout << "1: Создать файл" << endl;
    cout << "2: Удалить файл" << endl;
    cout << "3: Вывести список файлов" << endl;
    cout << "4: Отсортировать файлы по каталогу" << endl;
    cout << "5: Отсортировать файлы по дате и времени создания" << endl;
    cout << "6: Найти файлы в каталоге" << endl;
    cout << "7: Найти файлы по дате создания" << endl;
    cout << "8: Проверить возможность копирования файла на носитель" << endl;
    cout << "9: Получение информации о файле" << endl;
    cout << "0: Выход" << endl;
    cin >> command;

    while (command)
    {
        if (list.size == 0)
        {
            switch(command)
            {
            case 1:
                cout << endl << "Введите имя файла: "<< endl;
                cin >> fullName;
                for (int i = fullName.size() - 1; i>= 0; i--)
                {
                    if (fullName[i] == '.')
                    {
                       for (int j = i+1; j <fullName.size();j++)
                       {
                           suffix += fullName[j];
                       }
                       for (int j = 0; j <i;j++)
                       {
                           name += fullName[j];
                       }
                       flag = true;
                       break;
                    }
                }
                if (!flag) { name = fullName;}
                cout << "Введите каталог: " << endl;
                cin >> catalog;
                cout << "Введите дату создания: " << endl;
                cin >> date;
                if (date.size() < 10) {
                    cout << "Неверная дата!" << endl;
                    break;
                }
                cout << "Введите время создания: " << endl;
                cin >> time;
                if (time.size() < 5) {
                    cout << "Неверное время! " << endl;
                    break;
                }
                cout << "Введите размер в байтах: " << endl;
                cin >> size;
                countOfSectors = size / 512;
                if (countOfSectors*512 < size) {countOfSectors++;}
                cout << "Файл только для чтения? (1/0)" << endl;
                cin >> arg[0];
                cout << "Файл скрыт? (1/0)" << endl;
                cin >> arg[1];
                cout << "Файл системный? (1/0)" << endl;
                cin >> arg[2];

                list.addFile(catalog, name,suffix,date,time,arg,countOfSectors);
                cout << endl;
                break;
            default:
                cout << "Список файлов пуст!" << endl;
            }
        }
        else
        {
        switch (command)
            {
        case 1:
            cout << endl << "Введите имя файла: "<< endl;
            cin >> fullName;
            for (int i = fullName.size() - 1; i>= 0; i--)
            {
                if (fullName[i] == '.')
                {
                   for (int j = i+1; j <fullName.size();j++)
                   {
                       suffix += fullName[j];
                   }
                   for (int j = 0; j <i;j++)
                   {
                       name += fullName[j];
                   }
                   flag = true;
                   break;
                }
            }
            if (!flag) {name = fullName;}
            cout << "Введите каталог: " << endl;
            cin >> catalog;
            cout << "Введите дату создания: " << endl;
            cin >> date;
            if (date.size() < 10) {
                cout << "Неверная дата!" << endl;
                break;
            }
            cout << "Введите время создания: " << endl;
            cin >> time;
            if (time.size() < 5) {
                cout << "Неверное время! " << endl;
                break;
            }
            cout << "Введите размер в байтах: " << endl;
            cin >> size;
            countOfSectors = size / 512;
            if (countOfSectors*512 < size) {countOfSectors++;}
            cout << "Файл только для чтения? (1/0)" << endl;
            cin >> arg[0];
            cout << "Файл скрыт? (1/0)" << endl;
            cin >> arg[1];
            cout << "Файл системный? (1/0)" << endl;
            cin >> arg[2];

            list.addFile(catalog, name,suffix,date,time,arg,countOfSectors);
            cout << endl;
            break;
        case 2:
            cout << "Введите имя файла: " << endl;
            cin >> fullName;
            for (int i = fullName.size() - 1; i>= 0; i--)
            {
                if (fullName[i] == '.')
                {
                   for (int j = i+1; j <fullName.size();j++)
                   {
                       suffix += fullName[j];
                   }
                   for (int j = 0; j <i;j++)
                   {
                       name += fullName[j];
                   }
                   break;
                }
            }
            if (list.deleteFile(name)) {cout << "Файл удален" << endl;}
            else
            {
                cout << "Файл не найден";
            }
            break;
        case 3:
            list.getInfoAboutFiles();
            break;
        case 4:
            list.sortByCatalog();
            cout << "Файлы отсортированы по каталогу" << endl;
            break;
        case 5:
            list.sortByDate();
            cout << "Файлы отсортированы по дате и времени" << endl;
            break;
        case 6:
            cout << "Введите каталог" << endl;
            cin >> catalog;
            list.findAtCatalog(catalog);
            break;
        case 7:
            cout << "Введите дату" << endl;
            cin >> date;
            list.findAtCatalog(date);
            break;
        case 8:
            cout << "Введите название файла" << endl;
            cin >> name;
            cout << "Введите число секторов на носителе:" << endl;
            cin >> sec;
            if (list.checkForCopy(name, sec))
            {cout << "Файл поместится" << endl;}
            break;
        case 9:
            cout << "Введите название файла" << endl;
            cin >> name;
            cout << endl;
            list.getInfo(name);
            break;
        case 0:
            break;
        }
}

cout << "Нажмите любую клавишу чтобы продолжить" << endl;
        cin.ignore().get();

#ifdef __linux
        system("clear");
#else
        system("cls");
#endif

        cout << "Выберите команду для работы с файловой системой: " << endl;
        cout << "1: Создать файл" << endl;
        cout << "2: Удалить файл" << endl;
        cout << "3: Вывести список файлов" << endl;
        cout << "4: Отсортировать файлы по каталогу" << endl;
        cout << "5: Отсортировать файлы по дате и времени создания" << endl;
        cout << "6: Найти файлы в каталоге" << endl;
        cout << "7: Найти файлы по дате создания" << endl;
        cout << "8: Проверить возможность копирования файла на носитель" << endl;
        cout << "9: Получение информации о файле" << endl;
        cout << "0: Выход" << endl;
        cin >> command;
    }
    return 0;
}


