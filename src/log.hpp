#ifndef LOG_HPP_
#define LOG_HPP_

#include <atomic>

namespace logging {
    extern int verbose;
    inline void vout(int level, std::string msg) {
      if(verbose >= level)
      std::cout << msg << std::endl;
    }
    inline void vout(std::string msg) {
      vout(1,msg);
    }
    inline void verr(int level,std::string msg) {
      if(verbose >= level)
      std::cerr << msg << std::endl;
    }
    inline void verr(std::string msg) {
      verr(1,msg);
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
    extern bool links;
    extern long number;
}

namespace threading {
    extern std::atomic<bool> running;
}

#endif
