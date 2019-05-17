#ifndef VALUES_H
#define VALUES_H

#include <map>


class Values
{
public:
    static int loadFromFile(const char *filename); // �������� �� �����
    static int send(); // ��������� � ��� ����
    static int set(const char *buf);  // ��������� �� ������� ���� � ����

private:
    static std::map<int,double> dictionary;
    static int saveToFile();
    static const char * filename;
};

#endif // VALUES_H
