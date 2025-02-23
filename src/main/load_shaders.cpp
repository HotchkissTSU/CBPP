#include "cbvs/cbvs.h"
#include "cb_main/load_shaders.h"

bool ProcessShaderGroup(yyjson_val* jGrp, const char* sGName) noexcept {
    char sPrefix[64]; sPrefix[0] = '\0';
    yyjson_val* jPrefix = yyjson_obj_get(jGrp, "prefix");
    if(yyjson_is_str(jPrefix)) {
        snprintf(sPrefix, 64, "%s", yyjson_get_str(jPrefix));
    }

    bool bStrict = true;
    yyjson_val* jStrict = yyjson_obj_get(jGrp, "strict");
    if(yyjson_is_bool(jStrict)) {
        bStrict = yyjson_get_bool(jStrict);
    }

    yyjson_val* jMembers = yyjson_obj_get(jGrp, "members");
    if(jMembers == NULL && bStrict) {
        char sBuffer[128]; snprintf(sBuffer, 128, "Shader group '%s' has no members defined");
        cbpp::PushError(cbpp::ERROR_IO, sBuffer);
        return false;
    }

    size_t i, iMax;
    yyjson_val* jName;
    yyjson_val* jValue;

    char sVTXBuffer[128], sFRAGBuffer[128], sGEOMBuffer[128]; //Tear this fucking stack apart! Yarr!
    sGEOMBuffer[0] = '\0';

    char sNameBuffer[64];

    yyjson_obj_foreach(jMembers, i, iMax, jName, jValue) {
        const char* sName = jName->uni.str;

        snprintf(sNameBuffer, 64, "%s_%s", sGName, sName);

        //printf("\n\nNB=%s\n\n", sNameBuffer);

        yyjson_val* jVTX = yyjson_obj_get(jValue, "vtx");
        if(!yyjson_is_str(jVTX)) {
            snprintf(sVTXBuffer, 128, "Pipeline '%s' has no vertex shader", sName);
            return false;
        }

        yyjson_val* jFRAG = yyjson_obj_get(jValue, "frag");
        if(!yyjson_is_str(jFRAG)) {
            snprintf(sVTXBuffer, 128, "Pipeline '%s' has no geometry shader", sName);
            return false;
        }

        yyjson_val* jGEOM = yyjson_obj_get(jValue, "geom");
        bool bHasGEOM = yyjson_is_str(jGEOM);

        snprintf(sVTXBuffer, 128, "%s%s", sPrefix, yyjson_get_str(jVTX));
        snprintf(sFRAGBuffer, 128, "%s%s", sPrefix, yyjson_get_str(jFRAG));
        if(bHasGEOM) {
            snprintf(sGEOMBuffer, 128, "%s%s", sPrefix, yyjson_get_str(jGEOM));
            cbvs::__shloader_insert(new cbvs::ShaderLoaderNode(sNameBuffer, sVTXBuffer, sFRAGBuffer, sGEOMBuffer));
        }else{
            cbvs::__shloader_insert(new cbvs::ShaderLoaderNode(sNameBuffer, sVTXBuffer, sFRAGBuffer, NULL));
        }
    }

    return true;
}

bool RegisterShaders(yyjson_val* jValue) noexcept {
    size_t i, iMax;
    yyjson_val* sGrpName;
    yyjson_val* jGrp;

    yyjson_obj_foreach(jValue, i, iMax, sGrpName, jGrp) {
        if(!ProcessShaderGroup(jGrp, sGrpName->uni.str)) {
            char sBuffer[128]; snprintf(sBuffer, 128, "Failed to process shader group '%s'", yyjson_get_str(sGrpName));
            cbpp::PushError(cbpp::ERROR_IO, sBuffer);
            return false;
        }
    }

    return true;
}
