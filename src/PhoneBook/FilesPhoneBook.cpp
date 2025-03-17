/* Copyright (C) Protei - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Roman Makarov <makarov_roman@protei.ru>, 08/10/24
 */

#include "FilesPhoneBook.hpp"

Tm_FilesPhoneBook::Tm_FilesPhoneBook(const std::string& FileName)
{
}

bool Tm_FilesPhoneBook::Init()
{
    if (!RestoreFileData()) {
        return false;
    }
    if (!std::filesystem::exists(filename)) {
        try {
            io_file.open(filename, std::ios_base::out);
            if (!io_file.is_open()) {
                return false;
            }
            io_file.close();
        } catch (const std::exception& error) {
            std::cout << "Ошибка создания файла данных.";
            return false;
        }
    }
    if (!ReadFileData()) {
        return false;
    }
    return true;
}

En_ResultCode Tm_FilesPhoneBook::AddContact(Tm_Contact& Contact)
{
    uint32_t contact_id;
    if (!released_ids.empty()) {
        contact_id = released_ids.back();
    } else {
        contact_id = contacts.size();
    }
    Contact.m_Id = contact_id;
    contacts.push_back(Contact);
    if (!released_ids.empty() && Contact.m_Id == released_ids.back()) {
        released_ids.pop_back();
    }
    if (!WriteFileData()) {
        RestoreFileData();
        ReadFileData();
        return En_ResultCode::WriteError;
    }
    return En_ResultCode::Ok;
}

En_ResultCode Tm_FilesPhoneBook::RemoveContact(uint32_t Id)
{
    if (contacts.empty()) {
        return En_ResultCode::BookEmpty;
    }
    for (uint32_t contact_id = 0; contact_id < contacts.size(); ++contact_id) {
        if (contacts[contact_id].m_Id == Id) {
            contacts.erase(contacts.begin() + contact_id);
            released_ids.push_back(Id);
            if (!WriteFileData()) {
                RestoreFileData();
                ReadFileData();
                return En_ResultCode::WriteError;
            }
            return En_ResultCode::Ok;
        }
    }
    return En_ResultCode::NotFound;
}

En_ResultCode Tm_FilesPhoneBook::EditContact(const Tm_Contact& Contact)
{
    if (contacts.empty()) {
        return En_ResultCode::BookEmpty;
    }
    for (Tm_Contact& contact: contacts) {
        if (contact.m_Id == Contact.m_Id) {
            contact.m_Name = Contact.m_Name;
            contact.m_Number = Contact.m_Number;
            if (!WriteFileData()) {
                RestoreFileData();
                ReadFileData();
                return En_ResultCode::WriteError;
            }
            return En_ResultCode::Ok;
        }
    }
    return En_ResultCode::NotFound;
}

std::pair<En_ResultCode, std::optional<Tm_Contact>> Tm_FilesPhoneBook::GetContact(uint32_t Id)
{
    if (contacts.empty()) {
        return {En_ResultCode::BookEmpty, std::nullopt};
    }
    for (const Tm_Contact contact: contacts) {
        if (contact.m_Id == Id) {
            return {En_ResultCode::Ok, contact};
        }
    }
    return {En_ResultCode::NotFound, std::nullopt};
}

std::pair<En_ResultCode, std::vector<Tm_Contact>> Tm_FilesPhoneBook::GetAllContacts()
{
    if (contacts.empty()) {
        return {En_ResultCode::BookEmpty, contacts};
    }
    return {En_ResultCode::Ok, contacts};
}

bool Tm_FilesPhoneBook::ReadFileData()
{
    Json::Value file_data;
    try {
        io_file.open(filename, std::ios_base::in);
        if (!io_file.is_open()) {
            return false;
        }
        io_file.seekg(0);
        Json::Reader f_reader;
        f_reader.parse(io_file, file_data);
    } catch (const std::exception& error) {
        std::cout << "Ошибка чтения из файла данных." << std::endl;
        io_file.close();
        return false;
    }
    if (!file_data.empty()) {
        try
        {
            Json::Value readed_contacts = file_data.get("contacts", "");
            Json::Value rel_ids = file_data.get("released_ids", "");
            for (const Json::Value& contact: readed_contacts) {
                contacts.push_back(
                    Tm_Contact{contact["Id"].asUInt(), contact["Name"].asString(), contact["Number"].asString()});
            }
            for (const Json::Value id: rel_ids) {
                released_ids.push_back(id.asUInt());
            }
        }
        catch(const std::exception& error)
        {
           std::cout << "Ошибка формата контактов." << std::endl;
           io_file.close();
           return false;
        }
    }
    io_file.close();
    return true;
}

bool Tm_FilesPhoneBook::WriteFileData()
{
    if (!CreateBackup()) {
        return false;
    }
    try {
        io_file.open(filename, std::ios_base::out);
        if(!io_file.is_open()) {
            return false;
        }
        Json::Value data = SerializeData();
        Json::StyledStreamWriter f_writer;
        f_writer.write(io_file, data);
        std::filesystem::remove(backup_filename);
        io_file.close();
    } catch (const std::exception& error) {
        std::cout << "Ошибка записи в файл данных." << std::endl;
        std::filesystem::remove(backup_filename);
        io_file.close();
        return false;
    }
    return true;
}

Json::Value Tm_FilesPhoneBook::SerializeData()
{
    Json::Value data_array;
    for (const Tm_Contact& contact: contacts) {
        Json::Value data_contact;
        data_contact["Id"] = contact.m_Id;
        data_contact["Name"] = contact.m_Name;
        data_contact["Number"] = contact.m_Number;
        data_array.append(data_contact);
    }
    Json::Value data_contacts;
    data_contacts["contacts"] = data_array;
    Json::Value data_released_ids;
    for (const uint32_t& id: released_ids) {
        data_released_ids.append(id);
    }
    data_contacts["released_ids"] = data_released_ids;
    return data_contacts;
}

bool Tm_FilesPhoneBook::RestoreFileData()
{
    if (!std::filesystem::exists(backup_filename)) {
        return true;
    }
    try {
        std::filesystem::remove(filename);
        std::filesystem::rename(backup_filename, filename);
        return true;
    } catch (const std::filesystem::filesystem_error& error) {
        std::cout << "Ошибка восстановления из резервного файла данных." << std::endl;
        return false;
    }
}

bool Tm_FilesPhoneBook::CreateBackup()
{
    try {
        std::filesystem::copy_file(filename, backup_filename);
        return true;
    } catch (const std::filesystem::filesystem_error& error) {
        std::cout << "Ошибка создания резервного файла данных." << std::endl;
        return false;
    }
}
