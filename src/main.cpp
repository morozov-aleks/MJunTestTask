
#include <iostream>
#include <PhoneBook/FilesPhoneBook.hpp>

std::shared_ptr<Tm_PhoneBookInterface> CreatePhoneBook(const std::string &FileName)
{
    return std::make_shared<Tm_FilesPhoneBook>(FileName);
}

int main()
{
    auto PhoneBook = CreatePhoneBook("./book.json");
    //TODO
    return 0;
}
