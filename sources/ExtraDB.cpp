#include "ExtraDB.h"

ExtraDB::ExtraDB() : _disabledUsers(), _bannedUsers()
{
}

void ExtraDB::addDisabledUser(QString userName)
{
    _disabledUsers.push_back(userName);
}

void ExtraDB::addBannedUser(QString userName)
{
    _bannedUsers.push_back(userName);
}

QList<QString> ExtraDB::getDisablesUsersList() const
{
    return _disabledUsers;
}

QList<QString> ExtraDB::getBannedUsersList() const
{
    return _bannedUsers;
}
