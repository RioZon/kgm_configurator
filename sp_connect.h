#ifndef SP_CONNECT_H
#define SP_CONNECT_H

#include <sp.h>

/**
* \brief ������������� ��������� ������ � ����������� spread.
**/
class Sp_Connect
{
public:
    static int Connect(const char *param_con="4803");
    static int Disconnect();
    static mailbox id;  ///< ������������� ���������� �� �����.
    static char priv_gr[MAX_GROUP_NAME];  ///< ��� ��������� ������.
};

#endif // SP_CONNECT_H
