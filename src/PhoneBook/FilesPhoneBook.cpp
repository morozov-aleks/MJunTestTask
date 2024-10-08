/* Copyright (C) Protei - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Roman Makarov <makarov_roman@protei.ru>, 08/10/24
 */

#include "FilesPhoneBook.hpp"

Tm_FilesPhoneBook::Tm_FilesPhoneBook(const std::string& FileName)
{
    //TODO
}

En_ResultCode Tm_FilesPhoneBook::AddContact(const Tm_Contact& Contact)
{
    //TODO
}

En_ResultCode Tm_FilesPhoneBook::RemoveContact(uint32_t Id)
{
    //TODO
}

En_ResultCode Tm_FilesPhoneBook::EditContact(const Tm_Contact& Contact)
{
    //TODO
}

std::pair<En_ResultCode, std::optional<Tm_Contact>> Tm_FilesPhoneBook::GetContact(uint32_t Id)
{
    //TODO
}

std::pair<En_ResultCode, std::vector<Tm_Contact>> Tm_FilesPhoneBook::GetAllContacts()
{
    //TODO
}
