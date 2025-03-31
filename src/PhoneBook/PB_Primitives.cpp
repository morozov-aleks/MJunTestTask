#include "PB_Primitives.hpp"

ATE_INIT_PRIM_STATE(Tm_FilesPhoneBook, ACTIVE);
Tm_FilesPhoneBook::Tm_State *Tm_FilesPhoneBook::On_ACTIVE() {
    ATE_SWITCH_PRIM() {

        ATE_HANDLE_PRIM(Pr_ADD_CONTACT_REQ)
        En_ResultCode result = AddContact(p->m_Contact);
        switch (result) {
            case En_ResultCode::Ok:
                m_pOutput->Put(new Pr_ADD_CONTACT_CONF());
                break;
            default:
                m_pOutput->Put(new Pr_ADD_CONTACT_REJ(result));
                break;
        }
        return m_pState;

        ATE_HANDLE_PRIM(Pr_EDIT_CONTACT_REQ)
        En_ResultCode result = EditContact(p->m_Contact);
        switch (result) {
            case En_ResultCode::Ok:
                m_pOutput->Put(new Pr_EDIT_CONTACT_CONF());
                break;
            default:
                m_pOutput->Put(new Pr_EDIT_CONTACT_REJ(result));
                break;
        }
        return m_pState;

        ATE_HANDLE_PRIM(Pr_REMOVE_CONTACT_REQ)
        En_ResultCode result = RemoveContact(p->m_ContactId);
        switch (result) {
            case En_ResultCode::Ok:
                m_pOutput->Put(new Pr_REMOVE_CONTACT_CONF());
                break;
            default:
                m_pOutput->Put(new Pr_REMOVE_CONTACT_REJ(result));
                break;
        }
        return m_pState;

        ATE_HANDLE_PRIM(Pr_GET_CONTACT_REQ)
        std::pair<En_ResultCode, std::optional<Tm_Contact>> result = GetContact(p->m_ContactId);
        m_pOutput->Put(new Pr_GET_CONTACT_RESP(result));
        return m_pState;

        ATE_HANDLE_PRIM(Pr_GET_ALL_CONTACTS_REQ)
        std::pair<En_ResultCode, std::vector<Tm_Contact>> result = GetAllContacts();
        m_pOutput->Put(new Pr_GET_ALL_CONTACTS_RESP(result));
        return m_pState;

        ATE_DEFAULT_PRIM()
        return m_pState;
    }
}

