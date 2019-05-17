/**	\file sp_connect.cpp
*	\brief �������� �����, ��������������� ��������� ������ � ������� spread.
*/

#include "sp_connect.h"
#include <stdexcept>

mailbox Sp_Connect::id=0;
char Sp_Connect::priv_gr[MAX_GROUP_NAME];


/**
* \brief ���������� �� �������.
* @param param_con --- ������ ���������� � ������� ����@����;
* \return ��������� ���������� �� ������� (�������� ������ � ������ ���������� ����������).
*/
int Sp_Connect::Connect(const char *param_con)
{
    Disconnect();
    char us_name[12];
    sprintf(us_name,"%d",std::rand()%10000000);
    if( SP_connect(param_con,us_name,0,0,&id,priv_gr) == ACCEPT_SESSION)
    {
        if (SP_join(id,"kgm_conf"))
        {
            throw std::runtime_error("������ ����������� � ������ kgm_conf");
        }
    }
    else
    {
        throw std::runtime_error("������ ���������� �� spread");
    }
    return 0;
}

/**
* \brief ������������ �� ������ �����.
* \return 0.
*/
int Sp_Connect::Disconnect()
{
    if(id)
    {
        SP_disconnect(id);
        id=0;
    }
    return 0;
}
