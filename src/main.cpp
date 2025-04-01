#include <iostream>
#include <ATE/Architecture/PrimSyncDynaQueue.h>
#include <PhoneBook/FilesPhoneBook.hpp>
#include <PhoneBook/PB_Actor.hpp>

std::shared_ptr<Tm_PB_Actor> CreatePhoneBookActor(const std::string& FileName, Tm_PrimOutput* pClientOutput)
{
    auto book = std::make_shared<Tm_PB_Actor>(FileName);
    book->Link(pClientOutput);
    return book;
}


bool stream_failed()
{
    if (!std::cin) {
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(INT64_MAX, '\n');
        }
        return true;
    }
    return false;
}

void input(std::optional<uint32_t>& variable)
{
    try {
        std::cin >> *variable;
        std::cin.ignore();
    } catch (const std::ios_base::failure& error) {
        std::cout << "Проблема с потоком ввода: " << error.what() << std::endl << "Программа завершается" << std::endl;
        exit(1);
    }
    if (stream_failed()) {
        std::cout << "Некорректный ввод!" << std::endl;
        variable = std::nullopt;
    }
}

void input(std::string& variable)
{
    try {
        std::getline(std::cin, variable);
    } catch (const std::ios_base::failure& error) {
        std::cout << "Проблема с потоком ввода: " << error.what() << std::endl << "Программа завершается" << std::endl;
        exit(1);
    }
    if (stream_failed()) {
        std::cout << "Некорректный ввод!" << std::endl;
    }
}

void get_choice_prompt()
{
    std::cout << "Введите цифру, соответвующую пункту меню:\n"
                 "1. Вывести все записи.\n"
                 "2. Вывести определенную запись.\n"
                 "3. Создать новую запись.\n"
                 "4. Отредактировать запись.\n"
                 "5. Удалить запись.\n"
                 "6. Выйти из программы.\n";
}

void get_contact_prompt(const std::string& field)
{
    std::cout << "Введите " << field << " контакта." << std::endl;
}

void book_empty_output()
{
    std::cout << "Книга контактов пуста." << std::endl;
}

void contact_not_found_output()
{
    std::cout << "Контакт не найден." << std::endl;
}

void contact_field_cant_be_empty(const std::string& field)
{
    std::cout << field << " не может быть пустым!" << std::endl;
}

void unexpected_error_output()
{
    std::cout << "Неожиданная ошибка, попробуйте снова." << std::endl;
}

void repeat_input_output()
{
    std::cout << "Попробуйте ввести снова." << std::endl;
}

void write_error_output()
{
    std::cout << "Проверьте целостность файла данных." << std::endl;
}

int main()
{
    std::cin.exceptions(std::cin.exceptions() | std::ios_base::badbit);
    new Tm_Trace("./Trace.cfg");
    Tm_PrimSyncDynaQueue BackChannel;
    auto PhoneBookActor = CreatePhoneBookActor("./book.json", &BackChannel);
    if (!PhoneBookActor->Init_Actor()) {
        std::cout << "Ошибка инициализации. Проверьте целостность данных." << std::endl;
        return 1;
    }

    std::optional<uint32_t> choice;
    while (true) {
        get_choice_prompt();
        input(choice);
        switch (*choice) {
        case 1: {
            std::pair<En_ResultCode, std::vector<Tm_Contact>> result;
            PhoneBookActor->Put(new Pr_GET_ALL_CONTACTS_REQ());
            if (auto Resp = dynamic_cast<Pr_GET_ALL_CONTACTS_RESP*>(BackChannel.Get())) {
                result = std::move(Resp->m_Result);
            }
            switch (result.first) {
            case En_ResultCode::Ok:
                std::cout << "Книга контактов содержит:" << std::endl;
                for (Tm_Contact contact: result.second) {
                    std::cout << "Id пользователя: " << contact.m_Id << " " << "Имя пользователя: " << contact.m_Name
                              << " " << "Номер пользователя: " << contact.m_Number << std::endl;
                }
                break;
            case En_ResultCode::BookEmpty:
                book_empty_output();
                break;
            default:
                unexpected_error_output();
                break;
            }
            break;
        }
        case 2: {
            std::optional<uint32_t> id = 0;
            get_contact_prompt("Id");
            input(id);
            if (!id) {
                repeat_input_output();
                continue;
            }
            std::pair<En_ResultCode, std::optional<Tm_Contact>> result;
            PhoneBookActor->Put(new Pr_GET_CONTACT_REQ(*id));
            if (auto Resp = dynamic_cast<Pr_GET_CONTACT_RESP*>(BackChannel.Get())) {
                result = std::move(Resp->m_Result);
            }
            switch (result.first) {
            case En_ResultCode::Ok:
                std::cout << "Найден контакт:" << std::endl;
                std::cout << "Id пользователя: " << result.second->m_Id << " "
                          << "Имя пользователя: " << result.second->m_Name << " "
                          << "Номер пользователя: " << result.second->m_Number << std::endl;
                break;
            case En_ResultCode::NotFound:
                contact_not_found_output();
                break;
            case En_ResultCode::BookEmpty:
                book_empty_output();
                break;
            default:
                unexpected_error_output();
                break;
            }
        } break;
        case 3: {
            std::string name, number;
            get_contact_prompt("Имя");
            input(name);
            if (name.empty()) {
                contact_field_cant_be_empty("Имя контакта");
                repeat_input_output();
                continue;
            }
            get_contact_prompt("Номер");
            input(number);
            if (number.empty()) {
                contact_field_cant_be_empty("Номер контакта");
                repeat_input_output();
                continue;
            }
            Tm_Contact contact;
            contact.m_Name = name;
            contact.m_Number = number;
            En_ResultCode result;
            PhoneBookActor->Put(new Pr_ADD_CONTACT_REQ(contact));
            if (dynamic_cast<Pr_ADD_CONTACT_CONF*>(BackChannel.Get())) {
                std::cout << "Контакт добавлен" << std::endl;
                continue;
            }
            if (auto Rej = dynamic_cast<Pr_ADD_CONTACT_REJ*>(BackChannel.Get())) {
                result = std::move(Rej->m_Result);
            }
            switch (result) {
            case En_ResultCode::WriteError:
                write_error_output();
                break;
            default:
                unexpected_error_output();
                break;
            }
        } break;
        case 4: {
            std::optional<uint32_t> id = 0;
            get_contact_prompt("Id");
            input(id);
            if (!id) {
                repeat_input_output();
                continue;
            }
            std::string name, number;
            get_contact_prompt("новое имя");
            input(name);
            if (name.empty()) {
                contact_field_cant_be_empty("Имя контакта");
                repeat_input_output();
                continue;
            }
            get_contact_prompt("новый номер");
            input(number);
            if (number.empty()) {
                contact_field_cant_be_empty("Номер контакта");
                repeat_input_output();
                continue;
            }
            Tm_Contact contact;
            contact.m_Id = *id;
            contact.m_Name = name;
            contact.m_Number = number;
            En_ResultCode result;
            PhoneBookActor->Put(new Pr_EDIT_CONTACT_REQ(contact));
            if (dynamic_cast<Pr_EDIT_CONTACT_CONF*>(BackChannel.Get())) {
                std::cout << "Контакт изменен" << std::endl;
                continue;
            }
            if (auto Rej = dynamic_cast<Pr_EDIT_CONTACT_REJ*>(BackChannel.Get())) {
                result = std::move(Rej->m_Result);
            }
            switch (result) {
            case En_ResultCode::BookEmpty:
                book_empty_output();
                break;
            case En_ResultCode::NotFound:
                contact_not_found_output();
                break;
            case En_ResultCode::WriteError:
                write_error_output();
                break;
            default:
                unexpected_error_output();
                break;
            }
        } break;
        case 5: {
            std::optional<uint32_t> id = 0;
            get_contact_prompt("Id");
            input(id);
            if (!id) {
                repeat_input_output();
                continue;
            }
            En_ResultCode result;
            PhoneBookActor->Put(new Pr_REMOVE_CONTACT_REQ(*id));
            if (dynamic_cast<Pr_REMOVE_CONTACT_CONF*>(BackChannel.Get())) {
                std::cout << "Контакт удален" << std::endl;
                continue;
            }
            if (auto Rej = dynamic_cast<Pr_EDIT_CONTACT_REJ*>(BackChannel.Get())) {
                result = std::move(Rej->m_Result);
            }
            switch (result) {
            case En_ResultCode::BookEmpty:
                book_empty_output();
                break;
            case En_ResultCode::NotFound:
                contact_not_found_output();
                break;
            case En_ResultCode::WriteError:
                write_error_output();
            default:
                unexpected_error_output();
                break;
            }
        } break;
        case 6:
            std::cout << "Программа завершается" << std::endl;
            return 0;
        default:
            repeat_input_output();
            break;
        }
    }
    return 0;
}
