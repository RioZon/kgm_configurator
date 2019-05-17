#ifndef SP_CONNECT_H
#define SP_CONNECT_H

#include <sp.h>

/**
* \brief Предоставляет интерфейс работы с соединением spread.
**/
class Sp_Connect
{
public:
    static int Connect(const char *param_con="4803");
    static int Disconnect();
    static mailbox id;  ///< Идентификатор соединения со сприд.
    static char priv_gr[MAX_GROUP_NAME];  ///< Имя приватной группы.
};

#endif // SP_CONNECT_H
