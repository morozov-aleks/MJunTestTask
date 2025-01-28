/* Copyright (C) Protei - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Roman Makarov <makarov_roman@protei.ru>, 08/10/24
 */

#include "FilesPhoneBook.hpp"

Tm_FilesPhoneBook::Tm_FilesPhoneBook(const std::string& FileName)
{
    filename = FileName;
    backup_filename = FileName + ".backup";
    io_file.exceptions(io_file.exceptions() | std::ios_base::badbit);
    bool result = RestoreFileData();
    if (result == false) {
        std::cout << "Невозможно восстановить файл данных. Программа завершается" << std::endl;
        exit(1);
    }
    if (!std::filesystem::exists(filename)) {
        try {
            io_file.open(filename, std::ios_base::out);
            if (!FileDataIsOpen()) {
                std::cout << "Невозможно создать файл контактов" << std::endl;
                exit(1);
            }
            io_file.close();
        } catch (const std::exception& error) {
            std::cout << "Ошибка при создании файла контактов: ";
            std::cout << error.what() << std::endl;
            exit(1);
        }
    }
    ReadFileData();
}

En_ResultCode Tm_FilesPhoneBook::AddContact(const Tm_Contact& Contact)
{
    contacts.push_back(Contact);
    if (!released_ids.empty() && Contact.m_Id == released_ids.back()) {
        released_ids.pop_back();
    }
    bool add_result = WriteFileData();
    if (!add_result) {
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
            bool result = WriteFileData();
            if (!result) {
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
            bool add_result = WriteFileData();
            if (!add_result) {
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
    for (Tm_Contact contact: contacts) {
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

uint32_t Tm_FilesPhoneBook::GetNextContactId()
{
    uint32_t contact_id;
    if (!released_ids.empty()) {
        contact_id = released_ids.back();
    } else {
        contact_id = contacts.size();
    }
    return contact_id;
}

bool Tm_FilesPhoneBook::ReadFileData()
{
    Json::Value file_data;
    try {
        io_file.open(filename, std::ios_base::in);
        if (!FileDataIsOpen()) {
            std::cout << "Невозможно открыть файл контактов" << std::endl;
            exit(1);
        }
        io_file.seekg(0);
        f_reader.parse(io_file, file_data);
    } catch (const std::exception& error) {
        if (!io_file) {
            std::cout << "Ошибка чтения файла контактов: ";
            std::cout << error.what() << std::endl;
            exit(1);
        }
    }
    if (!file_data.empty()) {
        Json::Value readed_contacts = file_data.get("contacts", "");
        Json::Value rel_ids = file_data.get("released_ids", "");
        for (Json::Value contact: readed_contacts) {
            contacts.push_back(
                Tm_Contact{contact["Id"].asUInt(), contact["Name"].asString(), contact["Number"].asString()});
        }
        for (Json::Value id: rel_ids) {
            released_ids.push_back(id.asUInt());
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
        if (!io_file.is_open()) {
            RestoreFileData();
            return false;
        }
        Json::Value data = SerializeData();
        f_writer.write(io_file, data);
    } catch (const std::exception& error) {
        std::cout << error.what() << std::endl;
        RestoreFileData();
        return false;
    }
    std::filesystem::remove(backup_filename);
    io_file.close();
    return true;
}

bool Tm_FilesPhoneBook::FileDataIsOpen() const
{
    if (!io_file.is_open()) {
        std::cout << "База контактов повреждена!" << std::endl;
        return false;
    }
    return true;
}

Json::Value Tm_FilesPhoneBook::SerializeData()
{
    Json::Value data_array;
    for (Tm_Contact& contact: contacts) {
        Json::Value data_contact;
        data_contact["Id"] = contact.m_Id;
        data_contact["Name"] = contact.m_Name;
        data_contact["Number"] = contact.m_Number;
        data_array.append(data_contact);
    }
    Json::Value data_contacts;
    data_contacts["contacts"] = data_array;
    Json::Value data_released_ids;
    for (uint32_t& id: released_ids) {
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
        ReadFileData();
        return true;
    } catch (const std::filesystem::filesystem_error& error) {
        std::cout << "Невозможно восстановить резервный файл";
        std::cout << error.what() << std::endl;
        return false;
    }
}

bool Tm_FilesPhoneBook::CreateBackup()
{
    try {
        std::filesystem::copy_file(filename, backup_filename);
        return true;
    } catch (const std::filesystem::filesystem_error& error) {
        std::cout << "Невозможно создать резервный файл";
        std::cout << error.what() << std::endl;
        return false;
    }
}
