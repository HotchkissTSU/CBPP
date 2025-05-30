#ifndef CBPP_CDF_CLASSES_H
#define CBPP_CDF_CLASSES_H

/*
    All new entries MUST appear after the existing ones to maintain 
    compatibility with any older engine builds
*/

enum {
    CDF_CLASS_SPRITESHEET,      // *.cta
    CDF_CLASS_FONT,             // *.cff
    CDF_CLASS_MAP,              // *.cmf

    /*
        Your custom ClassID`s go here
    */

    CBPP_CDF_CLASSES_AMOUNT
};

#endif
