#include <iostream>
#include <PhoneBook/FilesPhoneBook.hpp>

std::shared_ptr<Tm_PhoneBookInterface> CreatePhoneBook(const std::string& FileName)
{
    return std::make_shared<Tm_FilesPhoneBook>(FileName);
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
        std::cout << "Проблема с потоком ввода" << std::endl
                  << error.what() << std::endl
                  << "Программа завершается" << std::endl;
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

void get_contact_prompt(const std::string field)
{
    std::cout << "Введите " << field << " контакта" << std::endl;
}

void book_empty_output()
{
    std::cout << "Книга контактов пуста" << std::endl;
}

void contact_not_found_output()
{
    std::cout << "Контакт не найден" << std::endl;
}

void contact_field_cant_be_empty(const std::string& field)
{
    std::cout << field << " не может быть пустым!" << std::endl;
}

void unexpected_error_output()
{
    std::cout << "Неожиданная ошибка, попробуйте снова" << std::endl;
}

void repeat_input_output()
{
    std::cout << "Попробуйте ввести снова" << std::endl;
}

void write_error_output()
{
    std::cout << "Ошибка записи данных в базу" << std::endl;
}

int main()
{
    auto PhoneBook = CreatePhoneBook("./book.json");
    std::optional<uint32_t> choice;
    std::cin.exceptions(std::cin.exceptions() | std::ios_base::badbit);
    while (true) {
        get_choice_prompt();
        input(choice);
        switch (*choice) {
        case 1: {
            std::pair<En_ResultCode, std::vector<Tm_Contact>> result = PhoneBook->GetAllContacts();
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
            std::pair<En_ResultCode, std::optional<Tm_Contact>> result = PhoneBook->GetContact(*id);
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
            contact.m_Id = PhoneBook->GetNextContactId();
            contact.m_Name = name;
            contact.m_Number = number;
            En_ResultCode result = PhoneBook->AddContact(contact);
            switch (result) {
            case En_ResultCode::Ok:
                std::cout << "Контакт добавлен" << std::endl;
                break;
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
            En_ResultCode result = PhoneBook->EditContact(contact);
            switch (result) {
            case En_ResultCode::BookEmpty:
                book_empty_output();
                break;
            case En_ResultCode::NotFound:
                contact_not_found_output();
                break;
            case En_ResultCode::Ok:
                std::cout << "Контакт изменен" << std::endl;
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
            En_ResultCode result = PhoneBook->RemoveContact(*id);
            switch (result) {
            case En_ResultCode::BookEmpty:
                book_empty_output();
                break;
            case En_ResultCode::NotFound:
                contact_not_found_output();
                break;
            case En_ResultCode::Ok:
                std::cout << "Контакт удален" << std::endl;
                break;
            case En_ResultCode::WriteError:
                write_error_output();
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
