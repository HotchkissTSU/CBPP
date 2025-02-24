#include "cbpp/print.h"

/*
 *    $$$$$$$$\ $$\       $$\                 $$\                   $$\                                   $$\ $$\       $$\           
 *    \__$$  __|$$ |      \__|                \__|                  $$ |                                  \__|$$ |      $$ |          
 *       $$ |   $$$$$$$\  $$\  $$$$$$$\       $$\  $$$$$$$\       $$$$$$\    $$$$$$\   $$$$$$\   $$$$$$\  $$\ $$$$$$$\  $$ | $$$$$$\  
 *       $$ |   $$  __$$\ $$ |$$  _____|      $$ |$$  _____|      \_$$  _|  $$  __$$\ $$  __$$\ $$  __$$\ $$ |$$  __$$\ $$ |$$  __$$\ 
 *       $$ |   $$ |  $$ |$$ |\$$$$$$\        $$ |\$$$$$$\          $$ |    $$$$$$$$ |$$ |  \__|$$ |  \__|$$ |$$ |  $$ |$$ |$$$$$$$$ |
 *       $$ |   $$ |  $$ |$$ | \____$$\       $$ | \____$$\         $$ |$$\ $$   ____|$$ |      $$ |      $$ |$$ |  $$ |$$ |$$   ____|
 *       $$ |   $$ |  $$ |$$ |$$$$$$$  |      $$ |$$$$$$$  |        \$$$$  |\$$$$$$$\ $$ |      $$ |      $$ |$$$$$$$  |$$ |\$$$$$$$\ 
 *       \__|   \__|  \__|\__|\_______/       \__|\_______/          \____/  \_______|\__|      \__|      \__|\_______/ \__| \_______|                                                                                                                             
 */

namespace cbpp {
    template<> void Print<float >(const float& refValue, FILE* hTarget)      { fprintf(hTarget, "%f", refValue);  }
    template<> void Print<double>(const double& refValue, FILE* hTarget)     { fprintf(hTarget, "%lf", refValue); }

    template<> void Print<int8_t >(const int8_t& refValue, FILE* hTarget)    { fprintf(hTarget, "%i", refValue);  }
    template<> void Print<int16_t>(const int16_t& refValue, FILE* hTarget)   { fprintf(hTarget, "%i", refValue);  }
    template<> void Print<int32_t>(const int32_t& refValue, FILE* hTarget)   { fprintf(hTarget, "%i", refValue);  }
    template<> void Print<int64_t>(const int64_t& refValue, FILE* hTarget)   { fprintf(hTarget, "%li", refValue); }

    template<> void Print<uint8_t >(const uint8_t& refValue, FILE* hTarget)  { fprintf(hTarget, "%u", refValue);  }
    template<> void Print<uint16_t>(const uint16_t& refValue, FILE* hTarget) { fprintf(hTarget, "%u", refValue);  }
    template<> void Print<uint32_t>(const uint32_t& refValue, FILE* hTarget) { fprintf(hTarget, "%u", refValue);  }
    template<> void Print<uint64_t>(const uint64_t& refValue, FILE* hTarget) { fprintf(hTarget, "%lu", refValue); }

    template<> void Print<Vec2>(const Vec2& refValue, FILE* hTarget) {
        fprintf(hTarget, "Vec2(%f, %f)", refValue.x, refValue.y);
    }

    template<> size_t SPrint<float >(const float& refValue, char* sTarget, size_t iMaxWrite)      { return snprintf(sTarget, iMaxWrite, "%f", refValue);  }
    template<> size_t SPrint<double>(const double& refValue, char* sTarget, size_t iMaxWrite)     { return snprintf(sTarget, iMaxWrite, "%lf", refValue); }

    template<> size_t SPrint<int8_t >(const int8_t& refValue, char* sTarget, size_t iMaxWrite)    { return snprintf(sTarget, iMaxWrite, "%d", refValue);  }
    template<> size_t SPrint<int16_t>(const int16_t& refValue, char* sTarget, size_t iMaxWrite)   { return snprintf(sTarget, iMaxWrite, "%d", refValue);  }
    template<> size_t SPrint<int32_t>(const int32_t& refValue, char* sTarget, size_t iMaxWrite)   { return snprintf(sTarget, iMaxWrite, "%d", refValue);  }
    template<> size_t SPrint<int64_t>(const int64_t& refValue, char* sTarget, size_t iMaxWrite)   { return snprintf(sTarget, iMaxWrite, "%ld", refValue); }

    template<> size_t SPrint<uint8_t >(const uint8_t& refValue, char* sTarget, size_t iMaxWrite)  { return snprintf(sTarget, iMaxWrite, "%u", refValue);  }
    template<> size_t SPrint<uint16_t>(const uint16_t& refValue, char* sTarget, size_t iMaxWrite) { return snprintf(sTarget, iMaxWrite, "%u", refValue);  }
    template<> size_t SPrint<uint32_t>(const uint32_t& refValue, char* sTarget, size_t iMaxWrite) { return snprintf(sTarget, iMaxWrite, "%u", refValue);  }
    template<> size_t SPrint<uint64_t>(const uint64_t& refValue, char* sTarget, size_t iMaxWrite) { return snprintf(sTarget, iMaxWrite, "%lu", refValue); }

    template<> size_t SPrint<Vec2>(const Vec2& refValue, char* sTarget, size_t iMaxWrite) {
        return snprintf(sTarget, iMaxWrite, "Vec2(%f, %f)", refValue.x, refValue.y);
    }
}
