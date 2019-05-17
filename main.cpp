#include <iostream>
#include <algorithm>
#include <signal.h>
#include <stdexcept>
#include <ctime>

#include "sp_connect.h"
#include "values.h"

using std::string;
using std::cout;
using std::endl;

/**
* \brief Парсинг командной строки.
* @param begin --- итератор на начало списка параметров;
* @param end --- итератор на конец списка параметров;
* @param option --- параметр для поиска;
* \return Параметр или исключение.
*/
char* getCmdOption(char ** begin, char ** end, const string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    throw std::runtime_error("Параметр "+option+" не найден");
}

/**
* \brief Логирование информации в поток out.
* @param message --- сообщение для логирования;
* @param isRepeatable --- флаг для игнорирования одинаковых сообщений;
*/
void log(string const & message, bool isRepeatable = false)
{
    static string mess;
    if (isRepeatable && message == mess)
        return;
    mess = message;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    if (ltm->tm_mday < 10)
        cout << "0";
    cout << ltm->tm_mday << ".";
    if (ltm->tm_mon + 1 < 10)
        cout << "0";
    cout << ltm->tm_mon + 1 << ".";
    cout << ltm->tm_year + 1900 << " ";
    if (ltm->tm_hour + 1 < 10)
        cout << "0";
    cout  << ltm->tm_hour + 1 << ":";
    if (ltm->tm_min < 10)
        cout << "0";
    cout << ltm->tm_min << ":";
    if (ltm->tm_sec < 10)
        cout << "0";
    cout << ltm->tm_sec << " "
         << message << endl;

}

static bool isRunning = true;

void stopRunning(int sig)
{
    isRunning = false;
    exit(sig);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, stopRunning);
    signal(SIGQUIT, stopRunning);

    log("Запущена система управления конфигурацией kgm_conf");
    if (argc != 7)
    {
        cout<< "    Использование:"<<endl;
        cout<< argv[0] <<" -h хост -p порт -f файл" << endl;
        cout<< "    Например:"<<endl;
        cout<< argv[0] <<" -h 192.168.0.2 -p 4803 -f spv" << endl;
        return -1;
    }

    string host;
    string port;
    const char * filename = 0;

    try {
        host     = getCmdOption(argv, argv + argc, "-h");
        port     = getCmdOption(argv, argv + argc, "-p");
        filename = getCmdOption(argv, argv + argc, "-f");

        Values::loadFromFile(filename);
    } catch (std::exception &ex) {
        log(ex.what());
        return -1;
    }

    string spr_name = port + "@" + host;

    int service_type=DROP_RECV;
    char sender[MAX_GROUP_NAME];
    int num_groups = 0;
    char target_groups[10][MAX_GROUP_NAME];
    short mess_type;
    int endian_mismatch;
    int size=1024*128;
    char * buf = new char[1024*128*10];
    while(isRunning)
    {
        while (Sp_Connect::id == 0)
        {
            try {
                Sp_Connect::Connect(spr_name.c_str());
                log("Соединение со spread установлено");
            } catch(std::exception &ex) {
                log(ex.what(),true);
                usleep(1000000); // sleep 1 sec
            }
        }

        while(SP_poll(Sp_Connect::id) > 0)
        {
            int ret=SP_receive(Sp_Connect::id,&service_type,sender,10,&num_groups,
                           target_groups,&mess_type,&endian_mismatch,size,buf);
            if(ret==BUFFER_TOO_SHORT) /*пришло более мегабайта данных*/
                std::cerr << "Буфер переполнен"<< endl;
            else if (ret < 0)
            {
                log("Потеряно соединение со spread");
                Sp_Connect::Disconnect();
            }
            else
            {
                buf[ret]='\0';
                if(!strncmp(buf,"get",3))
                {
                    log("Данные запрошены");
                    Values::send();
                }
                else if(!strncmp(buf,"set",3))
                {
                    log("Данные получены");
                    Values::set(buf+3);
                }
            }
        }
        usleep(1000000); // sleep 1 sec
    }
    cout<<"Система управления конфигурацией kgm_conf завершена"<<endl;
    return 0;
}
