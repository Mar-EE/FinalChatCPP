#ifndef EXTRADB_H
#define EXTRADB_H

#endif // EXTRADB_H
#pragma once
#include <QList>

class ExtraDB
{
    QList<QString> _disabledUsers;
    QList<QString> _bannedUsers;

public:
    ExtraDB();
    void addDisabledUser(QString userName);
    void addBannedUser(QString userName);
    QList<QString> getDisablesUsersList() const;
    QList<QString> getBannedUsersList() const;
};
