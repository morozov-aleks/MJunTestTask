/* Copyright (C) Protei - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Roman Makarov <makarov_roman@protei.ru>, 08/10/24
 */

#ifndef FILESPHONEBOOK_HPP
#define FILESPHONEBOOK_HPP

#include <jsoncpp/json/json.h>
#include <PhoneBook/PhoneBookInterface.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>


class Tm_FilesPhoneBook: public Tm_PhoneBookInterface {
public:
    Tm_FilesPhoneBook(const std::string& FileName);

    En_ResultCode AddContact(const Tm_Contact& Contact) override;
    En_ResultCode RemoveContact(uint32_t Id) override;
    En_ResultCode EditContact(const Tm_Contact& Contact) override;
    std::pair<En_ResultCode, std::optional<Tm_Contact>> GetContact(uint32_t Id) override;
    std::pair<En_ResultCode, std::vector<Tm_Contact>> GetAllContacts() override;
    uint32_t GetNextContactId() override;

private:
    std::string filename;
    std::string backup_filename;
    Json::Reader f_reader;
    Json::StyledStreamWriter f_writer;
    std::fstream io_file;
    std::vector<Tm_Contact> contacts;
    std::vector<uint32_t> released_ids;
    Json::Value SerializeData();
    bool ReadFileData();
    bool WriteFileData();
    bool FileDataIsOpen() const;
    bool RestoreFileData();
    bool CreateBackup();
};


#endif  //FILESPHONEBOOK_HPP
