#ifndef VALUES_H
#define VALUES_H

#include <map>


class Values
{
public:
    static int loadFromFile(const char *filename); // прочесть из файла
    static int send(); // отправить в АРМ ШНСГ
    static int set(const char *buf);  // сохранить из посылки АРМа в файл

private:
    static std::map<int,double> dictionary;
    static int saveToFile();
    static const char * filename;
};

#endif // VALUES_H
