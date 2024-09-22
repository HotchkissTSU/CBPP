#ifndef CBPP_IDT_H
#define CBPP_IDT_H

#include <cstdint>

namespace cbpp {
    class ID {
        public:
            ID(ID& other);
            ID(int& other);

            int64_t AsInt();
        
        private:
            int64_t value = 0;
    };
}

#endif
