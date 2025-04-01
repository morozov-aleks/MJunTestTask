#ifndef PB_PRITIMIVES_HPP
#define PB_PRITIMIVES_HPP

#include <ATE/Architecture/DirectIO_Machine.h>
#include <ATE/Concept/Primitive.h>
#include <ATE/Concept/PrimFSM.h>
#include <ATE/Concept/PrimOutput.h>
#include "FilesPhoneBook.hpp"


class Tm_PB_Actor: private Tm_FilesPhoneBook,
                   public Tm_PrimFSM,
                   public virtual Tm_PrimOutput,
                   public Tm_DirectIO_Machine<Tm_Primitive *> {
public:
	Tm_PB_Actor(const std::string& FileName): Tm_FilesPhoneBook{FileName}, m_pOutput(0) {
    m_pState = &st_ACTIVE;
    StartTraceMode("PhoneBook", 1, TTRACE_ID());
};
void Link(Tm_PrimOutput* pOutput) { m_pOutput = pOutput; }
bool Init_Actor() { return Init(); }

private:
    enum { ST_ACTIVE };
    Tm_PrimOutput* m_pOutput;

protected:
    Tm_State* On_ACTIVE();
    ATE_DECLARE_PRIM_STATE(ACTIVE);
};

class Gr1_PHONEBOOK_PRIMS {
public:
    enum { GROUP_ID = 1 };
    enum { ADD_CONTACT_REQ,
           ADD_CONTACT_CONF,
           EDIT_CONTACT_REQ,
           EDIT_CONTACT_CONF,
           REMOVE_CONTACT_REQ,
           REMOVE_CONTACT_CONF,
           GET_CONTACT_REQ,
           GET_CONTACT_RESP,
           GET_ALL_CONTACTS_REQ,
           GET_ALL_CONTACTS_RESP
    };
};

class Gr2_PHONEBOOK_REJ_PRIMS {
public:
    enum { GROUP_ID = 2 };
    enum { ADD_CONTACT_REJ,
    	   EDIT_CONTACT_REJ,
           REMOVE_CONTACT_REJ
    };
};

class Pr_ADD_CONTACT_REQ: public Gr1_PHONEBOOK_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_PRIMS, ADD_CONTACT_REQ);
    Pr_ADD_CONTACT_REQ(Tm_Contact& Contact) : m_Contact(Contact) {};
    Tm_Contact m_Contact;

    void PrintAdditionalDebugInfo(std::ostream& stream) const {
        stream << "Params=" << "{" << "Contact_Name=" << m_Contact.m_Name << ", "
               << "Contact_Number=" << m_Contact.m_Number << "}";
    };
};

class Pr_ADD_CONTACT_CONF: public Gr1_PHONEBOOK_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_PRIMS, ADD_CONTACT_CONF);
};

class Pr_ADD_CONTACT_REJ: public Gr2_PHONEBOOK_REJ_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr2_PHONEBOOK_REJ_PRIMS, ADD_CONTACT_REJ);
    Pr_ADD_CONTACT_REJ(En_ResultCode& code) : m_Result(code) {};
    En_ResultCode m_Result;
};

class Pr_EDIT_CONTACT_REQ: public Gr1_PHONEBOOK_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_PRIMS, EDIT_CONTACT_REQ);
    Pr_EDIT_CONTACT_REQ(const Tm_Contact& Contact) : m_Contact(Contact) {};
    const Tm_Contact m_Contact;

    void PrintAdditionalDebugInfo(std::ostream& stream) const {
        stream << "Params=" << "{" << "Contact_Id=" << m_Contact.m_Id << ", "
               << "Contact_Name=" << m_Contact.m_Name << ", "
               << "Contact_Number=" << m_Contact.m_Number << "}";
    };
};

class Pr_EDIT_CONTACT_CONF: public Gr1_PHONEBOOK_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_PRIMS, EDIT_CONTACT_CONF);
};

class Pr_EDIT_CONTACT_REJ: public Gr2_PHONEBOOK_REJ_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr2_PHONEBOOK_REJ_PRIMS, EDIT_CONTACT_REJ);
    Pr_EDIT_CONTACT_REJ(En_ResultCode& code) : m_Result(code) {};
    En_ResultCode m_Result;
};

class Pr_REMOVE_CONTACT_REQ: public Gr1_PHONEBOOK_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_PRIMS, REMOVE_CONTACT_REQ);
    Pr_REMOVE_CONTACT_REQ(uint32_t& Id) : m_ContactId(Id) {};
    uint32_t m_ContactId;

    void PrintAdditionalDebugInfo(std::ostream& stream) const {
        stream << "Params=" << "{" << "Contact_Id=" << m_ContactId << "}";
    };
};

class Pr_REMOVE_CONTACT_CONF: public Gr1_PHONEBOOK_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_PRIMS, REMOVE_CONTACT_CONF);
};

class Pr_REMOVE_CONTACT_REJ: public Gr2_PHONEBOOK_REJ_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr2_PHONEBOOK_REJ_PRIMS, REMOVE_CONTACT_REJ);
    Pr_REMOVE_CONTACT_REJ(En_ResultCode& code) : m_Result(code) {};
    En_ResultCode m_Result;
};

class Pr_GET_CONTACT_REQ: public Gr1_PHONEBOOK_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_PRIMS, GET_CONTACT_REQ);
    Pr_GET_CONTACT_REQ(uint32_t& Id) : m_ContactId(Id) {};
    uint32_t m_ContactId;

    void PrintAdditionalDebugInfo(std::ostream& stream) const {
        stream << "Params=" << "{" << "Contact_Id=" << m_ContactId << "}";
    };
};

class Pr_GET_CONTACT_RESP: public Gr1_PHONEBOOK_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_PRIMS, GET_CONTACT_RESP);
    Pr_GET_CONTACT_RESP(std::pair<En_ResultCode, std::optional<Tm_Contact>>& result) : m_Result(result) {};
    std::pair<En_ResultCode, std::optional<Tm_Contact>> m_Result;
};

class Pr_GET_ALL_CONTACTS_REQ: public Gr1_PHONEBOOK_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_PRIMS, GET_ALL_CONTACTS_REQ);
};

class Pr_GET_ALL_CONTACTS_RESP: public Gr1_PHONEBOOK_PRIMS, public Tm_Primitive {
public:
    ATE_DECLARE_PRIM_ID(Gr1_PHONEBOOK_PRIMS, GET_ALL_CONTACTS_RESP);
    Pr_GET_ALL_CONTACTS_RESP(std::pair<En_ResultCode, std::vector<Tm_Contact>>& result) : m_Result(result) {};
    std::pair<En_ResultCode, std::vector<Tm_Contact>> m_Result;
};


#endif  //PB_PRITIMIVES_HPP
