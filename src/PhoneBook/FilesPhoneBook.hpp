/* Copyright (C) Protei - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Roman Makarov <makarov_roman@protei.ru>, 08/10/24
 */

#ifndef FILESPHONEBOOK_HPP
#define FILESPHONEBOOK_HPP

#include <PhoneBook/PhoneBookInterface.hpp>
#include <string>

class Tm_FilesPhoneBook: public Tm_PhoneBookInterface {
public:
    Tm_FilesPhoneBook(const std::string& FileName);

    En_ResultCode AddContact(const Tm_Contact& Contact) override;
    En_ResultCode RemoveContact(uint32_t Id) override;
    En_ResultCode EditContact(const Tm_Contact& Contact) override;
    std::pair<En_ResultCode, std::optional<Tm_Contact>> GetContact(uint32_t Id) override;
    std::pair<En_ResultCode, std::vector<Tm_Contact>> GetAllContacts() override;

    //TODO
};


#endif  //FILESPHONEBOOK_HPP
