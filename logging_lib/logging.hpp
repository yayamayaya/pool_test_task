#ifndef LOGGING_HEADER
#define LOGGING_HEADER

#ifndef LOGG_DIR
#define LOGG_DIR "./logging"
#endif

#ifdef WIN32
#define _MKDIR_CALL() mkdir(logging_dir.c_str())
#else
#define _MKDIR_CALL() mkdir(logging_dir.c_str(), 0777)
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <sstream>
#include <time.h>
#include <sys/stat.h>
#include <cstring>

using ret_t = int;

#define _LOG logger::log

#define END_ logger::endl

enum Message_types
{
    INFO    = 1,
    WARNING = 2,
    ERROR   = 3
};

class logger
{
private:
    static std::mutex open_lock;

    static std::mutex print_lock;

    class logging_unit
    {
        public:
        static std::ofstream log_file;
        
        static void open_log_file(const std::string file_name);
        
        logging_unit& operator [](Message_types t);
        
        template<typename T>
        logging_unit& operator <<(const T data);

        static inline bool new_message              = true;
    };

    static inline const std::string logging_dir = "./logging/";

    static inline const std::string info_msg    = "[INFO]   ";

    static inline const std::string warn_msg    = "[WARNING]";

    static inline const std::string errr_msg    = "[ERROR]  ";

    static inline const char sq_brack_o         = '[';

    static inline const char sq_brack_c         = ']';

    static inline const std::string msg_start   = "> ";

    static inline const std::unordered_map<Message_types, std::string> msg_types = 
    {
        {INFO,    info_msg},
        {WARNING, warn_msg},
        {ERROR,   errr_msg}
    };

    static inline std::string msg_format = info_msg;

    logger() {};

    logger(const logger &cp) =delete;

    const logger& operator=(const logger &cp) =delete;

    static ret_t create_dir(struct stat *dir_info);

public:
    static logging_unit log;

    static void logger_init(const std::string file_name);

    static inline const std::string endl = "\n";
};

template <typename T>
logger::logging_unit& logger::logging_unit::operator <<(const T data)
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

    return *this;
}

#endif