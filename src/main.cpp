#include <iostream>
#include <PhoneBook/FilesPhoneBook.hpp>

std::shared_ptr<Tm_PhoneBookInterface> CreatePhoneBook(const std::string &FileName)
{
    return std::make_shared<Tm_FilesPhoneBook>(FileName);
}


bool stream_failed()
{
    if (!std::cin)
    {
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(INT64_MAX, '\n');
        }
        return true;
    }
    return false;
}

void input(std::optional<uint32_t>& variable)
{
    try
    {
        std::cin >> *variable;
    }
    catch (const std::ios_base::failure& error)
    {
        std::cout << "Проблема с потоком ввода" << std::endl 
                  << error.what() << std::endl
                  << "Программа завершается" << std::endl;
        exit(1);
    }
    if (stream_failed())
    {
        std::cout << "Некорректный ввод!" << std::endl;
        variable = std::nullopt;
    }
}

void input(std::string& variable)
{
    try
    {
        std::getline(std::cin, variable);
    }
    catch (const std::ios_base::failure& error)
    {
        std::cout << "Проблема с потоком ввода" << std::endl 
                  << error.what() << std::endl
                  << "Программа завершается" << std::endl;
        exit(1);
    }
    if (stream_failed())
    {
        std::cout << "Некорректный ввод!" << std::endl;
    }
}

void get_choice(std::optional<uint32_t>& variable)
{
        std::cout << "Введите цифру, соответвующую пункту меню:\n"
                     "1. Вывести все записи.\n"
                     "2. Вывести определенную запись.\n"
                     "3. Создать новую запись.\n"
                     "4. Отредактировать запись.\n"
                     "5. Удалить запись.\n"
                     "6. Выйти из программы.\n";
        input(variable);
}

void get_contact_id(std::optional<uint32_t>& variable)
{
    std::cout << "Введите Id контакта" << std::endl;
    input(variable);
}

void get_contact_number(std::string& variable)
{
    std::cout << "Введите имя контакта" << std::endl;
    input(variable);
}

void get_contact_name(std::string& variable)
{
    std::cout << "Введите номер контакта" << std::endl;
    input(variable);
}

void book_empty_output()
{
    std::cout << "Книга контактов пуста" << std::endl;
}

void contact_not_found_output()
{
    std::cout << "Контакт не найден" << std::endl;
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
    std::cin.exceptions(std::cin.exceptions()|std::ios_base::badbit);
    while (true)
    {
        get_choice(choice);
        switch (*choice)
        {
            case 1:
                {
                    std::pair<En_ResultCode, std::vector<Tm_Contact>> result = PhoneBook->GetAllContacts();
                    switch (result.first)
                    {
                        case En_ResultCode::Ok:
                            std::cout << "Книга контактов содержит:" << std::endl;
                            for (Tm_Contact contact: result.second)
                            {
                                std::cout << "Id пользователя: " << contact.m_Id << " "
                                          << "Имя пользователя: " << contact.m_Name << " "
                                          << "Номер пользователя: " << contact.m_Number << std::endl;
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
            case 2:
                {
                    std::optional<uint32_t> id;
                    get_contact_id(id);
                    if (id == std::nullopt)
                    {
                        repeat_input_output();
                        continue;
                    }
                    std::pair<En_ResultCode, std::optional<Tm_Contact>> result = PhoneBook->GetContact(*id);
                    switch (result.first)
                    {
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
                }
                break;
            case 3:
                std::cout << "Case 3" << std::endl;
                exit(0);
                {
                    std::string name, number;
                    get_contact_number(name);
                    if (name.empty())
                    {
                        repeat_input_output();
                        //continue;
                    }
                    get_contact_number(number);
                    if (number.empty())
                    {
                        repeat_input_output();
                        //continue;
                    }

                    Tm_Contact contact;
                    contact.m_Id = 0;
                    contact.m_Name = name;
                    contact.m_Number = number;
                    En_ResultCode result = PhoneBook->AddContact(contact);
                    switch (result)
                    {
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
                }
                break;
            case 4:
                std::cout << "Case 4" << std::endl;
                exit(0);
                {
                    std::optional<uint32_t> id;
                    std::cout << "Введите Id контакта для редактирования" << std::endl;
                    input(id);
                    if (!id)
                    {
                        repeat_input_output();
                        continue;
                    }
                    std::pair<En_ResultCode, std::optional<Tm_Contact>> result = PhoneBook->GetContact(*id);
                    switch (result.first)
                    {
                        case En_ResultCode::Ok:
                            {
                                std::string name, number;
                                std::cout << "Введите новое имя контакта" << std::endl;
                                input(name);
                                if (name.empty())
                                {
                                    repeat_input_output();
                                    //continue;
                                }
                                std::cout << "Введите новый номер контакта" << std::endl;
                                input(number);
                                if (number.empty())
                                {
                                    repeat_input_output();
                                    //continue;
                                }
                                Tm_Contact contact;
                                contact.m_Name = name;
                                contact.m_Number = number;
                                En_ResultCode result = PhoneBook->EditContact(contact);
                                switch (result)
                                {
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
                            }
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
                }
                break;
            case 5:
                std::cout << "Case 5" << std::endl;
                exit(0);
                {
                    std::optional<uint32_t> id;
                    get_contact_id(id);
                    if (!id)
                    {
                        repeat_input_output();
                        continue;
                    }
                    En_ResultCode result = PhoneBook->RemoveContact(*id);
                    switch (result)
                    {
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
                }
                break;
            case 6:
                std::cout << "Программа завершается" << std::endl;
                return 0;
            default:
                std::cout << "Попробуйте снова." << std::endl;
                break;
        }
    }
    return 0;
}
