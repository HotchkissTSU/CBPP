#include "cbpp/locales.h"

#include "cbpp/fileio.h"
#include "cbpp/cb_alloc.h"

#include "cbpp/yyjson.h"

#define CBPP_LOCALE_INIT_LINEBUFF 64
#define CBPP_LOCALE_LINEBUFF_OVERHEAD 16

namespace cbpp {
    Locale* g_pCurrentLocale = NULL;
    std::map<hash_t, Locale> g_mLocales;

    Char* Locale::Load(const char* fname) {
        m_bRWresult = true;

        File hInput(fname, "rb"); //open the file in a binary mode to forget about C locales
        if(!hInput.IsOpen()) { m_bRWresult = false; return NULL; }

        size_t iFileLen = hInput.Length();

        char* aFileBuffer = (char*)malloc(iFileLen+1); //the buffer for the UTF-8 bytes
        aFileBuffer[iFileLen] = '\0';

        hInput.Read(aFileBuffer, iFileLen);

        Char* aBuffer32 = String::U8_U32(aFileBuffer);
        free(aFileBuffer);

        if(aBuffer32 == NULL) { m_bRWresult = false; return NULL; }

        return aBuffer32;
    }

    bool Locale::ProcessPair(const Char* aName, const Char* aText) {
        size_t iNameLen = String::str32len(aName);
        size_t iTextLen = String::str32len(aText);

        char* sName = (char*)malloc(iNameLen+1);
        sName[iNameLen] = '\0';

        Char* sText = (Char*)malloc(sizeof(Char)*(iTextLen+1));
        sText[iTextLen] = U'\0';

        bool bNarrowWarning = false;
        for(size_t i = 0; i < iNameLen; i++) { //truncate the name to only have ASCII symbols in
            sName[i] = (char)(aName[i]);
            bNarrowWarning = bNarrowWarning || (aName[i] > 256);
        }

        if(bNarrowWarning) {
            CbThrowWarningf("The text label '%ls' seems to have non-ASCII characters.\nThe resulting name will look like '%s'",
                            aName, sName);
        }

        memcpy(sText, aText, sizeof(Char)*(iTextLen+1));

        if(sText[iTextLen-1] == U'\n') { //this newline is present almost in every label definition, so wipe it out if needed
            sText[iTextLen-1] = U'\0';
        }

        hash_t iNameHash = Hash(sName);
        if(m_mData.count(iNameHash) > 0) {
            CbThrowWarningf("The label named '%s' seems to already be present in the locale table. This may be a hash collision!", sName);
        }

        m_mData[iNameHash] = String(sText);

        //printf("%s:\t%ls\n", sName, sText);

        free(sName);
        free(sText);

        return true;
    }
    
    bool Locale::ParseSource(const Char* sSource) {
        Char* sSourcePrep = Preprocess(sSource);
        const Char* aSource;
        if(sSourcePrep == NULL) {
            aSource = sSource;
        }else {
            aSource = sSourcePrep;
        }

        bool bOut = (sSourcePrep != NULL);
        size_t iSrcLen = String::str32len(aSource);

        Char aEntryNameBuffer[CBPP_LOCALE_MAXNAME+1]; aEntryNameBuffer[0] = U'\0';
        Char aEntryTextBuffer[CBPP_LOCALE_MAXTEXT+1]; aEntryTextBuffer[0] = U'\0';

        size_t iNameLen = 0, iTextLen = 0;
        bool bIsReadingName = false, bIsQuoted = false, bIsProtected = false;

        for(size_t i = 0; i < iSrcLen; i++) {
            Char iCurrent = aSource[i];
            
            switch( iCurrent ) {
                case U'\\':
                    if(!bIsProtected) {
                        bIsProtected = true;
                        continue;
                    }

                case U'#':
                    if(!bIsQuoted && !bIsProtected) {
                        if(!bIsReadingName) {
                            iNameLen = 0; iTextLen = 0;
                            if(aEntryNameBuffer[0] != U'\0' && aEntryTextBuffer[0] != U'\0') {
                                bOut = bOut && ProcessPair(aEntryNameBuffer, aEntryTextBuffer);
                            }
                        }

                        bIsReadingName = true;
                        break;
                    }

                case U'"':
                    if(!bIsProtected) {
                        bIsQuoted = !bIsQuoted;
                        break;
                    }

                case U' ':
                    if(!bIsQuoted && !bIsProtected) {
                        bIsReadingName = false;
                        if(iTextLen == 0) { continue; } //skip the syntax-only whitespace between the name and the text
                    }

                case U'\n':
                    if(!bIsQuoted && !bIsProtected) {
                        bIsReadingName = false;
                        if(iTextLen == 0) { continue; }
                    }

                case U'n': //if `n` is protected, this means that we have a `\n` sequence
                    if(bIsProtected) {
                        iCurrent = U'\n';
                    }

                default:
                    if(bIsReadingName) {
                        if(iNameLen < CBPP_LOCALE_MAXNAME) {
                            aEntryNameBuffer[iNameLen] = iCurrent;
                            aEntryNameBuffer[iNameLen+1] = U'\0';
                            iNameLen++;
                        }else {
                            bIsReadingName = false;
                        }
                    }else {
                        if(iNameLen < CBPP_LOCALE_MAXTEXT) {
                            aEntryTextBuffer[iTextLen] = iCurrent;
                            aEntryTextBuffer[iTextLen+1] = U'\0';
                            iTextLen++;
                        }else {
                            bOut = false;
                        }
                    }
            }
            
            bIsProtected = false;

            if(i+1 == iSrcLen) { //this is the last iteration
                if(aEntryNameBuffer[0] != U'\0' && aEntryTextBuffer[0] != U'\0') {
                    bOut = bOut && ProcessPair(aEntryNameBuffer, aEntryTextBuffer);
                }
            }
        }

        return bOut;
    }

    Char* Locale::Preprocess(const Char* sSource) {
        size_t iSourceLen = String::str32len(sSource);

        Char* sSourceMod = (Char*)malloc(sizeof(Char) * (iSourceLen+1));
        memset(sSourceMod, 0, sizeof(Char)*(iSourceLen+1));

        size_t iProc = 0, iSource = 0;
        Char iCurrent = U'a';
        while(iCurrent != U'\0') {
            iCurrent = sSource[iSource];

            if(iCurrent == U'/') {
                Char iTemp = U'a';
                while(iTemp != U'\n' && iSource < iSourceLen) { //rewind the string to skip the commentary
                    iSource++;
                    iTemp = sSource[iSource];
                }
            }else{
                iSource++;
            }

            if(iCurrent != U'/' && iCurrent != U'\n') { //also, omit all newlines
                sSourceMod[iProc] = iCurrent;
                iProc++;
            }
        }

        return sSourceMod;
    }

    void Locale::SetName(const String& nm) {
        m_sPrettyName = nm;
    }

    void Locale::operator=(Locale& other) {
        for(auto it = other.m_mData.begin(); it != other.m_mData.end(); it++) { //i have no words and i`m losing my sanity here
            m_mData[it->first] = it->second; //this STL ape somehow breaks strings without this by-hand stupid copy
        } //TODO: write my own hash tables that WORK AS INTENDED
    }
    
    void MountLocale(const char* sPath) {
        char sJsonPath[512];
        snprintf(sJsonPath, 512, "assets/locales/%s.json", sPath);

        File hInput(sJsonPath, "rb");

        size_t iFileLen = hInput.Length();
        
        char* sJsonBuffer = (char*)malloc(iFileLen+1);
        sJsonBuffer[iFileLen] = '\0';

        hInput.Read(sJsonBuffer, iFileLen);
        hInput.Close();
        
        yyjson_read_err jError;
        yyjson_doc* jDoc = yyjson_read_opts(sJsonBuffer, iFileLen, CBPP_JSONREAD_OPTS, NULL, &jError);

        size_t iLine, iCol, iChar;

        if(jDoc == NULL) {
            if(yyjson_locate_pos(sJsonBuffer, iFileLen, jError.pos, &iLine, &iCol, &iChar)) {
                char sErrorBuffer[512];
                snprintf(sErrorBuffer, 512, "Json read error: %s, code %u, at [%lu, %lu, index %lu]", 
                        jError.msg, jError.code, iLine, iCol, iChar);
                PushError(ERROR_IO, sErrorBuffer);
            }
        }

        free(sJsonBuffer);

        Char* sFullLocaleName = NULL;
        char* sCodeLocaleName = NULL;
        char** aLocaleFiles = NULL;
        size_t iNumFiles = 0;
        char* sLocaleFilesPrefix = NULL;

        if(jDoc) {
            yyjson_val* jRoot = yyjson_doc_get_root(jDoc);
            if(jRoot) {
                yyjson_val* jFullName = yyjson_obj_get(jRoot, "fullname");
                const char* sFullName_u8 = yyjson_get_str(jFullName); //read the locale name
                if(sFullName_u8 != NULL) {
                    sFullLocaleName = String::U8_U32(sFullName_u8);
                }

                yyjson_val* jCodeName = yyjson_obj_get(jRoot, "codename");
                if(yyjson_is_str(jCodeName)) {
                    const char* sCodeName = yyjson_get_str(jCodeName);
                    sCodeLocaleName = strdup(sCodeName);
                }

                yyjson_val* jFilePrefix = yyjson_obj_get(jRoot, "prefix");
                if(jFilePrefix != NULL && yyjson_is_str(jFilePrefix)) {
                    sLocaleFilesPrefix = strdup(yyjson_get_str(jFilePrefix)); //read files` prefix if it is providen
                }

                yyjson_val* jFilesList = yyjson_obj_get(jRoot, "files");
                if(jFilesList != NULL) {
                    iNumFiles = yyjson_get_len(jFilesList);
                    aLocaleFiles = (char**)malloc(sizeof(char*)*iNumFiles);
                    for(size_t i = 0; i < iNumFiles; i++) {
                        yyjson_val* jFile = yyjson_arr_get(jFilesList, i);

                        if(jFile && yyjson_is_str(jFile)) {
                            aLocaleFiles[i] = strdup(yyjson_get_str(jFile));
                        }
                    }
                }
            }
        }

        if(sFullLocaleName == NULL) {
            sFullLocaleName = String::str32dup(U"UNNAMED");
        }

        if(sCodeLocaleName == NULL) {
            sCodeLocaleName = const_cast<char*>(sPath);
            CbThrowWarningf("No locale codename is providen. This locale`s one is set to '%s'", sPath);
        }

        hash_t iNameHash = Hash(sCodeLocaleName);
        if(g_mLocales.count(iNameHash) > 0) {
            CbThrowWarningf("Possible hash collision with a new locale named '%s' (%ls)", sCodeLocaleName, sFullLocaleName);
        }

        Locale& out = g_mLocales[iNameHash];

        out.SetName(sFullLocaleName);

        if(aLocaleFiles != NULL) {
            char sFileName[512];

            for(size_t i = 0; i < iNumFiles; i++) {
                if(sLocaleFilesPrefix != NULL){
                    snprintf(sFileName, 512, "assets/locales/%s%s", sLocaleFilesPrefix, aLocaleFiles[i]);
                }else{
                    snprintf(sFileName, 512, "assets/locales/%s", aLocaleFiles[i]);
                }

                Char* sLocaleSource = out.Load(sFileName);
                if(sLocaleSource != NULL) { out.ParseSource(sLocaleSource); }
                free(sLocaleSource);
                free(aLocaleFiles[i]);
            }
        }

        if(jDoc != NULL) { yyjson_doc_free(jDoc); }

        out.m_sDefaultEntry = String(U"<UNDEF LABEL>");

        free(aLocaleFiles);
        free(sFullLocaleName);
        free(sCodeLocaleName);
    }

    const String& Locale::GetString(const char* key) {
        hash_t iKeyHash = Hash(key);

        if(m_mData.count(iKeyHash) == 0) {
            return m_sDefaultEntry;
        }

        return m_mData.at(iKeyHash);
    }

    Locale* GetCurrentLocale() noexcept {
        return g_pCurrentLocale;
    }

    bool SetLocale(const char* sName) noexcept {
        bool bOut = true;

        hash_t iNameHash = Hash(sName);
        if(g_mLocales.count(iNameHash) > 0) {
            g_pCurrentLocale = &g_mLocales[iNameHash];
        }else{
            bOut = false;
        }

        return bOut;
    }
}
