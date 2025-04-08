#include "logging.hpp"
#include <cerrno>

logger::logging_unit  logger::log;

std::ofstream logger::logging_unit::log_file;

std::mutex logger::open_lock;

std::mutex logger::print_lock;

logger::logging_unit& logger::logging_unit::operator [](Message_types t) 
{
    if (!log_file.is_open())
        return *this;

    auto iter = msg_types.find(t);

    if (iter != msg_types.end())
        msg_format = iter->second;
    else 
        msg_format = info_msg;

    return *this;
}

logger::logging_unit& logger::logging_unit::operator << (std::string data)
{
    if (!log_file.is_open())
        return *this;

    std::lock_guard<std::mutex> mu(logger::print_lock);

    if (new_message)
    {
        time_t t;
        time(&t);

        log_file << sq_brack_o << strtok(ctime(&t), "\n") \
            << sq_brack_c << msg_format << msg_start;

        new_message = false;
    }

    log_file << data;

    if (data == logger::endl)
    {
        log_file.flush();

        msg_format = info_msg;

        new_message = true;
    }

    return *this;
}

ret_t logger::create_dir(struct stat *dir_info)
{
    std::cout << "> No logging directory, creating new\n";

    errno = 0;

    if (_MKDIR_CALL() == -1)
        std::cout << "> Couldn't create directory, there will be no logging\n";
    else
        stat(logging_dir.c_str(), dir_info);

    return errno;
}

void logger::logging_unit::open_log_file(const std::string file_name)
{
    struct stat dir_info = {};

    if (stat(logging_dir.c_str(), &dir_info) && create_dir(&dir_info))
        return;

    if (!(dir_info.st_mode & S_IFDIR))
    {
        std::cout << "> logging_dir is NOT an directory, there will be no logging\n";
        return;
    }

    log_file.open(logging_dir + file_name, std::ios::out);
    if (log_file.fail()) 
        std::cout << "> Failed to open log_file\n";
}

void logger::logger_init(const std::string file_name)
{
    std::lock_guard<std::mutex> mu(logger::open_lock);

    if (!log.log_file.is_open())
        log.open_log_file(file_name);
    else
        logger::log << "an attempt to create another logger was made" << logger::endl;
}   