#ifndef LOG_HPP_
#define LOG_HPP_

namespace logging {
    extern bool verbose;
    inline void vout(std::string msg) {
        if(verbose)
            std::cout << msg << std::endl;
    }
    inline void verr(std::string msg) {
        if(verbose)
            std::cerr << msg << std::endl;
    }
}

namespace dict {
    extern bool whitelist;
    extern bool blacklist;
    extern bool other;
    extern std::string whitefile;
    extern std::string blackfile;
    extern std::string otherfile;
}

namespace countdown {
    extern bool timeout;
    extern long time;
    extern bool links;
    extern long number;
}

#endif
