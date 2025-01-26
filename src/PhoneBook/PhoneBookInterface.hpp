/* Copyright (C) Protei - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Roman Makarov <makarov_roman@protei.ru>, 08/10/24
 */

#ifndef ADDRESSBOOKINTERFACE_HPP
#define ADDRESSBOOKINTERFACE_HPP

#include <cstdint>
#include <vector>
#include <optional>
#include <memory>

struct Tm_Contact {
    uint32_t m_Id = 0;
    std::string m_Name = "";
    std::string m_Number = "";
};

enum class En_ResultCode {
    Ok,
    NotFound,
    BookEmpty,
    JsonError,
    WriteError,
};

class Tm_PhoneBookInterface {
public:
    virtual ~Tm_PhoneBookInterface() = default;

    virtual En_ResultCode AddContact(const Tm_Contact& Contact) = 0;
    virtual En_ResultCode RemoveContact(uint32_t Id) = 0;
    virtual En_ResultCode EditContact(const Tm_Contact& Contact) = 0;

    virtual std::pair<En_ResultCode, std::optional<Tm_Contact>> GetContact(uint32_t Id) = 0;
    virtual std::pair<En_ResultCode, std::vector<Tm_Contact>> GetAllContacts() = 0;
};


#endif // ADDRESSBOOKINTERFACE_HPP
