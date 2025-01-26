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
    io_file.exceptions(io_file.exceptions()|std::ios_base::badbit);
    std::optional<bool> result = RestoreFileData();
    if (result == false)
    {
        std::cout << "Невозможно восстановить файл данных. Программа завершается" << std::endl;
        exit(1);
    }
    ReadFileData();
}

En_ResultCode Tm_FilesPhoneBook::AddContact(const Tm_Contact& Contact)
{
    uint32_t contact_id;
    if (!released_ids.empty())
    {
        contact_id = released_ids.back();
    }
    else
    {
        contact_id = contacts.size();
    }
    bool add_result = WriteFileData();
    if (!add_result)
    {
        return En_ResultCode::WriteError;
    }
    return En_ResultCode::Ok;
}

En_ResultCode Tm_FilesPhoneBook::RemoveContact(uint32_t Id)
{
    Json::Value contacts = ReadFileData();
    io_file.sync();
    if (contacts["contacts"].empty())
    {
        return En_ResultCode::BookEmpty;
    }
    std::optional<Json::ArrayIndex> contact_id = FindContact(contacts, Id);
    if (!contact_id)
    {
        return En_ResultCode::NotFound;
    }
    Json::Value removed;
    bool remove_result = contacts["contacts"].removeIndex(*contact_id, &removed);
    if (!remove_result)
    {
        return En_ResultCode::JsonError;
    }
    contacts["released_ids"].append(Id);
    bool write_result = WriteFileData();
    if (!write_result)
    {
        return En_ResultCode::WriteError;
    }
    return En_ResultCode::Ok;
}

En_ResultCode Tm_FilesPhoneBook::EditContact(const Tm_Contact& Contact)
{
    Json::Value contacts = ReadFileData();
    if (contacts["contacts"].empty())
    {
        return En_ResultCode::BookEmpty;
    }
    std::optional<Json::ArrayIndex> contact_id = FindContact(contacts, Contact.m_Id);
    if (!contact_id)
    {
        return En_ResultCode::NotFound;
    }
    contacts["contacts"][*contact_id]["Name"] = Contact.m_Name;
    contacts["contacts"][*contact_id]["Number"] = Contact.m_Number;
    bool edit_result = WriteFileData();
    if (!edit_result)
    {
        return En_ResultCode::WriteError;
    }
    return En_ResultCode::Ok;
}

std::pair<En_ResultCode, std::optional<Tm_Contact>> Tm_FilesPhoneBook::GetContact(uint32_t Id)
{
    if (contacts.empty()) {
        return {En_ResultCode::BookEmpty, std::nullopt};
    }
    for (Tm_Contact contact: contacts)
    {
        if (contact.m_Id == Id)
        {
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
    io_file.open(filename);
    if (!FileDataIsOpen())
    {
        exit(1);
    }
    Json::Value file_data;
    try
    {
        io_file.seekg(0);
        f_reader.parse(io_file, file_data);
    }
    catch(const std::exception& error)
    {
        std::cout << error.what() << std::endl;
        if (!io_file)
        {
            std::cout << "Ошибка чтения файла контактов" << std::endl;
            exit(1);
        }
    }
    if (!file_data.empty())
    {
        Json::Value readed_contacts = file_data.get("contacts", "");
        Json::Value rel_ids = file_data.get("released_ids", "");
        for (Json::Value contact: readed_contacts)
        {
            contacts.push_back(Tm_Contact{contact["id"].asUInt(), contact["Name"].asString(), contact["Number"].asString()});
        }
        for (Json::Value id: rel_ids)
        {
            released_ids.push_back(id.asUInt());
        }
    }
    io_file.close();
    if (FileDataIsOpen())
    {
        exit(1);
    }
    return true;
}

bool Tm_FilesPhoneBook::WriteFileData()
{
    if (!CreateBackup())
    {
        return false;
    }
    io_file.open(filename, std::ios_base::out|std::ios_base::app);
    if (!io_file.is_open())
    {
        return false;
    }
    Json::Value data = SerializeData();
    try
    {
        f_writer.write(io_file, data);
    }
    catch(const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
        RestoreFileData();
        return false;
    }
    std::filesystem::remove(backup_filename);
    io_file.close();
    return true;
}

std::optional<uint32_t> Tm_FilesPhoneBook::FindContact(Json::Value& contacts, uint32_t Id) const
{
    for (Json::ArrayIndex elem_id = 0; elem_id < contacts["contacts"].size(); ++elem_id)
    {  
        if (Id == contacts["contacts"][elem_id]["id"].asUInt())
        {
            return elem_id;
        }
    }
    return std::nullopt;
}

bool Tm_FilesPhoneBook::FileDataIsOpen() const
{
    if (!io_file.is_open())
    {
        std::cout << "База контактов повреждена!" << std::endl;
        return false;
    }
    return true;
}

Json::Value Tm_FilesPhoneBook::SerializeData()
{
    Json::Value data_array;
    for (Tm_Contact& contact: contacts)
    {
        Json::Value data_contact;
        data_contact["Id"] = contact.m_Id;
        data_contact["Name"] = contact.m_Name;
        data_contact["Number"] = contact.m_Number;
        data_array.append(data_contact);
    }
    Json::Value data_map;
    data_map["contacts"] = data_array;
    return data_map;
}

std::optional<bool> Tm_FilesPhoneBook::RestoreFileData()
{
    if (!std::filesystem::exists(backup_filename))
    {
        return std::nullopt;
    }
    try
    {
        std::filesystem::remove(filename);
        std::filesystem::rename(backup_filename, filename);
        return true;
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        std::cout << error.what() << std::endl;
        return false;
    }
}

bool Tm_FilesPhoneBook::CreateBackup()
{
    try
    {
        std::filesystem::copy_file(filename, backup_filename);
        return true;
    }
    catch(const std::filesystem::filesystem_error& error)
    {
        std::cerr << error.what() << std::endl;
        return false;
    }
}
