#ifndef CBPP_RANGE_H
#define CBPP_RANGE_H

#include "cbpp/cb_alloc.h"

namespace cbpp {
    //Fixes a value between the two boundaries
    template<typename T> class Range {
        public:
            Range() = default;
            Range(const T& start_value, const T& min_value, const T& max_value) : m_value(start_value), m_max(max_value), m_min(min_value) {};

            const T& Value() const noexcept {
                return m_value;
            }

            bool Set(const T& other) {
                m_value = other;
                if(m_value > m_max) {
                    m_value = m_max;
                    return false;
                }
                if(m_value < m_min) {
                    m_value = m_min;
                    return false;
                }

                return true;
            }

            bool Add(const T& to_add) {
                return Set(m_value+to_add);
            }

            void SetLimits(const T& min_value, const T& max_value) {
                m_min = min_value;
                m_max = max_value;
            }

            const T& Max() const noexcept {
                return m_max;
            }

            const T& Min() const noexcept {
                return m_min;
            }

        private:
            T m_value;
            T m_max, m_min;
    };
}

#endif