#ifndef PB_PRITIMIVES_HPP
#define PB_PRITIMIVES_HPP

#include <ATE/Concept/Primitive.h>
#include "PhoneBookInterface.hpp"

class Gr1_PHONEBOOK_ADD_COMMANDS {
public:
    enum { GROUP_ID = 1 };
    enum { ADD_CONTACT_REQ, ADD_CONTACT_CONF, ADD_CONTACT_REJ };
};

class Gr2_PHONEBOOK_EDIT_COMMANDS {
public:
    enum { GROUP_ID = 2 };
    enum { EDIT_CONTACT_REQ, EDIT_CONTACT_CONF, EDIT_CONTACT_REJ };
};

class Gr3_PHONEBOOK_REMOVE_COMMANDS {
public:
    enum { GROUP_ID = 3 };
    enum { REMOVE_CONTACT_REQ, REMOVE_CONTACT_CONF, REMOVE_CONTACT_REJ };
};

class Gr4_PHONEBOOK_GET_COMMANDS {
public:
    enum { GROUP_ID = 4 };
    enum { GET_CONTACT_REQ, GET_CONTACT_RESP, GET_ALL_CONTACTS_REQ, GET_ALL_CONTACTS_RESP };
};

class Gr5_UNKNOWN_PRIM {
public:
    enum { GROUP_ID = 5 };
    enum { UNKNOWN_PRIM };
};

class Pr_ADD_CONTACT_REQ: public Gr1_PHONEBOOK_ADD_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_ADD_COMMANDS, ADD_CONTACT_REQ);
    Pr_ADD_CONTACT_REQ(Tm_Contact& Contact) : m_Contact(Contact) {};
    Tm_Contact& m_Contact;
};

class Pr_ADD_CONTACT_CONF: public Gr1_PHONEBOOK_ADD_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_ADD_COMMANDS, ADD_CONTACT_CONF);
    Pr_ADD_CONTACT_CONF(En_ResultCode code) : m_Result(code) {};
    En_ResultCode m_Result;
};

class Pr_ADD_CONTACT_REJ: public Gr1_PHONEBOOK_ADD_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_ADD_COMMANDS, ADD_CONTACT_REJ);
    Pr_ADD_CONTACT_REJ(En_ResultCode code) : m_Result(code) {};
    En_ResultCode m_Result;
};

class Pr_EDIT_CONTACT_REQ: public Gr2_PHONEBOOK_EDIT_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr2_PHONEBOOK_EDIT_COMMANDS, EDIT_CONTACT_REQ);
    Pr_EDIT_CONTACT_REQ(const Tm_Contact& Contact) : m_Contact(Contact) {};
    const Tm_Contact& m_Contact;
};

class Pr_EDIT_CONTACT_CONF: public Gr2_PHONEBOOK_EDIT_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr2_PHONEBOOK_EDIT_COMMANDS, EDIT_CONTACT_CONF);
    Pr_EDIT_CONTACT_CONF(En_ResultCode code) : m_Result(code) {};
    En_ResultCode m_Result;
};

class Pr_EDIT_CONTACT_REJ: public Gr2_PHONEBOOK_EDIT_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr2_PHONEBOOK_EDIT_COMMANDS, EDIT_CONTACT_REJ);
    Pr_EDIT_CONTACT_REJ(En_ResultCode code) : m_Result(code) {};
    En_ResultCode m_Result;
};

class Pr_REMOVE_CONTACT_REQ: public Gr3_PHONEBOOK_REMOVE_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr3_PHONEBOOK_REMOVE_COMMANDS, REMOVE_CONTACT_REQ);
    Pr_REMOVE_CONTACT_REQ(uint32_t Id) : m_ContactId(Id) {};
    uint32_t m_ContactId;
};

class Pr_REMOVE_CONTACT_CONF: public Gr3_PHONEBOOK_REMOVE_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr3_PHONEBOOK_REMOVE_COMMANDS, REMOVE_CONTACT_CONF);
    Pr_REMOVE_CONTACT_CONF(En_ResultCode code) : m_Result(code) {};
    En_ResultCode m_Result;
};

class Pr_REMOVE_CONTACT_REJ: public Gr3_PHONEBOOK_REMOVE_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr3_PHONEBOOK_REMOVE_COMMANDS, REMOVE_CONTACT_REJ);
    Pr_REMOVE_CONTACT_REJ(En_ResultCode code) : m_Result(code) {};
    En_ResultCode m_Result;
};

class Pr_GET_CONTACT_REQ: public Gr4_PHONEBOOK_GET_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr4_PHONEBOOK_GET_COMMANDS, GET_CONTACT_REQ);
    Pr_GET_CONTACT_REQ(uint32_t Id) : m_ContactId(Id) {};
    uint32_t m_ContactId;
};

class Pr_GET_CONTACT_RESP: public Gr4_PHONEBOOK_GET_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr4_PHONEBOOK_GET_COMMANDS, GET_CONTACT_RESP);
    Pr_GET_CONTACT_RESP(std::pair<En_ResultCode, std::optional<Tm_Contact>> result) : m_Result(result) {};
    std::pair<En_ResultCode, std::optional<Tm_Contact>> m_Result;
};

class Pr_GET_ALL_CONTACTS_REQ: public Gr4_PHONEBOOK_GET_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr4_PHONEBOOK_GET_COMMANDS, GET_ALL_CONTACTS_REQ);
    Pr_GET_ALL_CONTACTS_REQ();
};

class Pr_GET_ALL_CONTACTS_RESP: public Gr4_PHONEBOOK_GET_COMMANDS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr4_PHONEBOOK_GET_COMMANDS, GET_ALL_CONTACTS_RESP);
    Pr_GET_ALL_CONTACTS_RESP(std::pair<En_ResultCode, std::vector<Tm_Contact>> result) : m_Result(result) {};
    std::pair<En_ResultCode, std::vector<Tm_Contact>> m_Result;
};

class Pr_UNKNOWN_PRIM: public Gr5_UNKNOWN_PRIM, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr5_UNKNOWN_PRIM, UNKNOWN_PRIM);
    Pr_UNKNOWN_PRIM();
};

#endif  //PB_PRITIMIVES_HPP
