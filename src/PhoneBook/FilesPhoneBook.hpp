/* Copyright (C) Protei - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Roman Makarov <makarov_roman@protei.ru>, 08/10/24
 */

#ifndef FILESPHONEBOOK_HPP
#define FILESPHONEBOOK_HPP

#include <ATE/Concept/Primitive.h>
#include <ATE/Concept/PrimFSM.h>
#include <ATE/Concept/PrimOutput.h>
#include <jsoncpp/json/json.h>
#include <PhoneBookInterface.hpp>
#include <PB_Primitives.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>


class Tm_FilesPhoneBook: public Tm_PrimFSM, public virtual Tm_PrimOutput, public Tm_PhoneBookInterface {
public:
    Tm_FilesPhoneBook(const std::string& FileName): filename{FileName}, backup_filename{filename + ".backup"}, m_pOutput(0)
    {
        m_pState = &st_ACTIVE;
        StartTraceMode("PhoneBook", 1, TTRACE_ID());
    };

    bool Init() override;
    En_ResultCode AddContact(Tm_Contact& Contact) override;
    En_ResultCode RemoveContact(uint32_t Id) override;
    En_ResultCode EditContact(const Tm_Contact& Contact) override;
    std::pair<En_ResultCode, std::optional<Tm_Contact>> GetContact(uint32_t Id) override;
    std::pair<En_ResultCode, std::vector<Tm_Contact>> GetAllContacts() override;
    void Link(Tm_PrimOutput* pOutput) { m_pOutput = pOutput; }

private:
    enum { ST_ACTIVE };
    Tm_PrimOutput* m_pOutput;
    const std::string filename;
    const std::string backup_filename;
    std::fstream io_file;
    std::vector<Tm_Contact> contacts;
    std::vector<uint32_t> released_ids;
    Json::Value SerializeData();
    bool ReadFileData();
    bool WriteFileData();
    bool RestoreFileData();
    bool CreateBackup();

protected:
    Tm_State* On_ACTIVE();
    ATE_DECLARE_PRIM_STATE(ACTIVE);
};


#endif  //FILESPHONEBOOK_HPP
