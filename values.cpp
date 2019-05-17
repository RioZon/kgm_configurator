#include "values.h"
#include "sp_connect.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>


std::map<int,double> Values::dictionary;
const char * Values::filename;

int Values::loadFromFile(const char * filename)
{
    dictionary.clear();
    Values::filename = filename;
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Ошибка чтения файла");
    }

    int num = 0;
    double coeff = 0;
    std::string strLine;
    while(getline(file,strLine))
    {
        if (sscanf(strLine.c_str(),"braker%d %*s %lf",&num,&coeff) == 2)
        {
            dictionary[num] = coeff;
        }
    }
    file.close();
    return 0;
}

int Values::send()
{
    std::stringstream sstream;
    for (std::map<int,double>::iterator it = dictionary.begin(); it != dictionary.end(); ++it)
    {
        sstream << it->first << " "<< it->second << " ";
    }
    std::string str = sstream.str();
    int res = SP_multicast(Sp_Connect::id,UNRELIABLE_MESS,"kgm_conf",
                 0,str.length(),str.c_str());
    return res;
}

int Values::set(const char * buf)
{
    dictionary.clear();
    std::stringstream ss;
    ss << buf;
    int num;
    double val;
    while (!ss.eof())
    {
        ss >> num;
        ss >> val;
        dictionary[num] = val;
        std::cout<<num<<" = "<<val<<std::endl;
    }
    saveToFile();
    return 0;
}

int Values::saveToFile()
{
    std::ofstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Ошибка записи в файл");
    }
    file << "[scaleparkvelocities]\n";
    for (std::map<int,double>::iterator it = dictionary.begin(); it != dictionary.end(); ++it)
    {
        file << "braker"<<it->first<<"    =    "<<it->second<<"\n";
    }
    file.close();
    return 0;
}
