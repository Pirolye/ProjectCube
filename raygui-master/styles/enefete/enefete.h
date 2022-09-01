//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// StyleAsCode exporter v1.2 - Style data exported as a values array            //
//                                                                              //
// USAGE: On init call: GuiLoadStyleEnefete();                             //
//                                                                              //
// more info and bugs-report:  github.com/raysan5/raygui                        //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2020-2022 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#define ENEFETE_STYLE_PROPS_COUNT  16

// Custom style name: enefete
static const GuiStyleProp enefeteStyleProps[ENEFETE_STYLE_PROPS_COUNT] = {
    { 0, 0, 0x1980d5ff },    // DEFAULT_BORDER_COLOR_NORMAL 
    { 0, 1, 0x4df3ebff },    // DEFAULT_BASE_COLOR_NORMAL 
    { 0, 2, 0x103e60ff },    // DEFAULT_TEXT_COLOR_NORMAL 
    { 0, 3, 0xe7e2f7ff },    // DEFAULT_BORDER_COLOR_FOCUSED 
    { 0, 4, 0x23d4ddff },    // DEFAULT_BASE_COLOR_FOCUSED 
    { 0, 5, 0xf1f1f1ff },    // DEFAULT_TEXT_COLOR_FOCUSED 
    { 0, 6, 0x6413a6ff },    // DEFAULT_BORDER_COLOR_PRESSED 
    { 0, 7, 0xea66d9ff },    // DEFAULT_BASE_COLOR_PRESSED 
    { 0, 8, 0x9f00bbff },    // DEFAULT_TEXT_COLOR_PRESSED 
    { 0, 9, 0x4b909eff },    // DEFAULT_BORDER_COLOR_DISABLED 
    { 0, 10, 0x73c7d0ff },    // DEFAULT_BASE_COLOR_DISABLED 
    { 0, 11, 0x448894ff },    // DEFAULT_TEXT_COLOR_DISABLED 
    { 0, 16, 0x00000010 },    // DEFAULT_TEXT_SIZE 
    { 0, 17, 0x00000000 },    // DEFAULT_TEXT_SPACING 
    { 0, 18, 0x1d3f6cff },    // DEFAULT_LINE_COLOR 
    { 0, 19, 0x29c9e5ff },    // DEFAULT_BACKGROUND_COLOR 
};

// WARNING: This style uses a custom font: GenericMobileSystemNuevo.ttf (size: 16, spacing: 0)

#define ENEFETE_COMPRESSED_DATA_SIZE 1270

// Font image pixels data compressed (DEFLATE)
// NOTE: Original pixel data simplified to GRAYSCALE
static unsigned char enefeteFontData[ENEFETE_COMPRESSED_DATA_SIZE] = { 0xed,
    0xdd, 0x51, 0x96, 0xe3, 0x26, 0x10, 0x05, 0x50, 0xf6, 0xbf, 0xe9, 0xca, 0xc9, 0x47, 0x72, 0x26, 0x93, 0x69, 0x01, 0x45,
    0x21, 0x61, 0xf9, 0xf6, 0xfd, 0xb3, 0xda, 0xdd, 0x32, 0xcf, 0x12, 0x92, 0x29, 0x70, 0x34, 0x00, 0x80, 0xdf, 0xfc, 0xfd,
    0xf3, 0xe7, 0xc7, 0xfe, 0xb4, 0xe5, 0x7a, 0xdb, 0x3f, 0xbf, 0xd1, 0xdb, 0x12, 0x3f, 0xee, 0x49, 0x5c, 0x6c, 0x1b, 0xfb,
    0x5f, 0xb9, 0xfd, 0x8a, 0xa9, 0xb6, 0x89, 0x8b, 0x56, 0x6b, 0xcb, 0x8f, 0xb7, 0xcb, 0xb6, 0xbf, 0x7a, 0xd6, 0xf3, 0xf9,
    0xc7, 0xc5, 0x3e, 0xc6, 0xbf, 0x19, 0xc4, 0xc5, 0x7e, 0xc4, 0xe5, 0xd6, 0x91, 0x44, 0xaf, 0xfe, 0x77, 0x5d, 0x42, 0xab,
    0x7f, 0x25, 0xba, 0x47, 0xc2, 0x5c, 0x0b, 0x9d, 0x90, 0x7f, 0xfc, 0xd2, 0xce, 0x91, 0xcc, 0x60, 0x3c, 0xff, 0xba, 0xa3,
    0xf6, 0xfa, 0x3d, 0xbb, 0xaf, 0x85, 0xaf, 0xda, 0x69, 0xfe, 0x6c, 0x55, 0xb7, 0x77, 0xd7, 0xbf, 0xdd, 0x3b, 0x03, 0xee,
    0xc8, 0xbf, 0xe2, 0x78, 0xfb, 0xf9, 0x2f, 0xc7, 0xc4, 0x99, 0xa4, 0xa6, 0x7d, 0x7b, 0xaf, 0xf6, 0xd4, 0xfc, 0x47, 0x8f,
    0xff, 0x9f, 0x7b, 0xe0, 0xe8, 0xbe, 0xb7, 0x76, 0x9c, 0x6f, 0xfb, 0xbd, 0xd6, 0x9d, 0xd7, 0x57, 0x2b, 0xf9, 0xc7, 0xc1,
    0xf9, 0x8f, 0xec, 0x5d, 0x74, 0xfb, 0xff, 0xd1, 0xe3, 0x6e, 0x2e, 0xff, 0xba, 0x23, 0xfa, 0xc9, 0xfc, 0xef, 0xb9, 0xfe,
    0xcb, 0x9e, 0xff, 0x47, 0xce, 0xb4, 0x3f, 0x25, 0xfc, 0x6b, 0x8b, 0xc4, 0x86, 0xf3, 0xff, 0x49, 0xf7, 0x57, 0xd9, 0xfc,
    0xe3, 0x80, 0xd7, 0x36, 0x76, 0xff, 0x77, 0xd5, 0xa3, 0xf6, 0xf3, 0x6f, 0xaf, 0xc9, 0x3f, 0x92, 0x57, 0x3b, 0x3f, 0x5f,
    0xa9, 0xde, 0xb5, 0x87, 0x6b, 0xfd, 0x7f, 0xef, 0x9e, 0xe7, 0xdb, 0xf3, 0xbf, 0xbe, 0xff, 0x9b, 0xbb, 0xdf, 0xf9, 0xb4,
    0xfc, 0x7b, 0x77, 0xbd, 0x6f, 0x3a, 0xff, 0x67, 0xaf, 0xe3, 0xe7, 0xef, 0x54, 0xcf, 0x79, 0xbd, 0xf7, 0xed, 0xe1, 0xbe,
    0xbb, 0x36, 0xaa, 0x53, 0xb9, 0xeb, 0x7f, 0x49, 0xff, 0x9b, 0xf2, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9f, 0xcb, 0xf7,
    0xab, 0x76, 0xe6, 0xeb, 0x63, 0x7b, 0x5b, 0xe6, 0x2b, 0xd9, 0xaf, 0x9f, 0x35, 0x37, 0xda, 0x5b, 0x53, 0xc1, 0x3f, 0xd3,
    0x82, 0x6d, 0x7a, 0x6b, 0x6f, 0x5c, 0x37, 0x86, 0x5a, 0x79, 0x74, 0x24, 0x7a, 0xb6, 0xd5, 0x47, 0x46, 0xa9, 0x33, 0x23,
    0xd9, 0x2b, 0xdb, 0xc6, 0x2b, 0x25, 0x6b, 0x2b, 0x36, 0x62, 0x60, 0x2e, 0x42, 0x24, 0xab, 0x26, 0xfa, 0xaf, 0x2f, 0x36,
    0x56, 0xcd, 0xc4, 0x45, 0xd5, 0xf6, 0x5a, 0x25, 0x72, 0x65, 0xfe, 0x75, 0x75, 0x17, 0xb1, 0xf0, 0xba, 0xa2, 0x33, 0xea,
    0x1c, 0x5b, 0x12, 0xee, 0xed, 0xcb, 0x5a, 0xd5, 0xcc, 0xae, 0xfc, 0xe7, 0xab, 0x1f, 0x2b, 0xf2, 0x8f, 0xad, 0xa3, 0xd8,
    0x3f, 0xe7, 0x1f, 0x87, 0xe7, 0xdf, 0x9b, 0x5f, 0x37, 0xd7, 0x5b, 0xae, 0x1c, 0x49, 0xfd, 0x3e, 0x7e, 0xed, 0xfc, 0xbf,
    0xf7, 0xec, 0x71, 0x5d, 0xd7, 0x9a, 0x49, 0x78, 0xee, 0x9a, 0xa7, 0xfa, 0xf8, 0xdf, 0xd1, 0x8f, 0xe7, 0xab, 0x9f, 0xe7,
    0xae, 0xcf, 0xee, 0xeb, 0xff, 0xdb, 0x60, 0x5d, 0xf3, 0xe7, 0x9d, 0xff, 0xcf, 0xca, 0xbf, 0x4d, 0x5f, 0xd1, 0x54, 0xce,
    0x6e, 0xc8, 0xd6, 0x9c, 0xf6, 0xf2, 0x1f, 0x79, 0x57, 0x3f, 0x99, 0x7f, 0xe6, 0x4c, 0xbe, 0x32, 0x8b, 0xa5, 0x2a, 0xff,
    0x33, 0xaa, 0xcd, 0xce, 0x3f, 0xfe, 0xef, 0xbe, 0x57, 0xcb, 0xe6, 0xbf, 0x67, 0x96, 0xf3, 0xfe, 0xf9, 0x1c, 0x23, 0x73,
    0xdb, 0xd6, 0xf2, 0x5f, 0x6d, 0xad, 0xfe, 0x27, 0x05, 0x95, 0x9f, 0xd5, 0x8c, 0xcc, 0x62, 0x8c, 0x92, 0x79, 0xa0, 0xe7,
    0xe4, 0x9f, 0x3b, 0x4a, 0xef, 0xf9, 0xfc, 0x87, 0xbb, 0xaa, 0x9e, 0x65, 0xe1, 0xf3, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0xfb, 0x46, 0x27, 0xfa, 0x55, 0xfe, 0x91, 0x18, 0x83, 0xac, 0xaa, 0xcf, 0xca, 0xd7, 0x42, 0xc4, 0x6f, 0x35, 0xb8,
    0x31, 0x5d, 0x2b, 0x11, 0xe9, 0xd9, 0x0c, 0xcf, 0x3c, 0x77, 0xbe, 0xf6, 0x73, 0x7d, 0x6b, 0x65, 0x66, 0xf3, 0xf5, 0x0c,
    0xa3, 0xf3, 0x2b, 0x62, 0x5b, 0x9d, 0xc5, 0xdd, 0x5b, 0xc7, 0x1e, 0xbf, 0x2f, 0xff, 0x76, 0xd3, 0xf1, 0x9f, 0xcd, 0x3f,
    0xba, 0x33, 0x99, 0xce, 0xcd, 0x3f, 0x53, 0x43, 0xfd, 0xce, 0xfc, 0xf3, 0xab, 0xe2, 0x47, 0xb7, 0x92, 0xfd, 0xc4, 0xfc,
    0xe3, 0xa6, 0xfc, 0x57, 0x66, 0xeb, 0xb5, 0xe4, 0xb5, 0xc3, 0xbd, 0xf9, 0xb7, 0x6e, 0xc2, 0xb1, 0x90, 0x52, 0xfe, 0x0a,
    0x68, 0xa5, 0xde, 0xed, 0xa4, 0xe3, 0xff, 0x8e, 0xfe, 0x3f, 0x0a, 0xbe, 0x15, 0x63, 0x47, 0xfe, 0xab, 0xed, 0x93, 0xab,
    0x86, 0xfc, 0xc6, 0xfc, 0xb3, 0xfd, 0xff, 0xf8, 0xb5, 0xc5, 0x59, 0xf9, 0x8f, 0xcc, 0xcd, 0xfa, 0x9e, 0xfc, 0xdb, 0x4b,
    0xf3, 0xcf, 0xcf, 0x2d, 0x7a, 0x5b, 0xfe, 0xb1, 0xe1, 0x2c, 0xbc, 0x7a, 0xff, 0xff, 0x54, 0xfe, 0x77, 0xde, 0xff, 0xad,
    0xaf, 0x7c, 0x50, 0xf5, 0xf9, 0x4f, 0xfd, 0x95, 0xd4, 0xfe, 0xcf, 0x7f, 0xf6, 0xb4, 0x4f, 0x1b, 0xfa, 0x3e, 0x9d, 0xec,
    0x4c, 0x76, 0x4e, 0x98, 0x1d, 0x82, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xbe, 0x91, 0x89, 0xde, 0xe8, 0x69,
    0xed, 0x2a, 0x74, 0x63, 0xf3, 0x0e, 0x66, 0xc6, 0xbc, 0x33, 0xe3, 0xe4, 0xd9, 0xfa, 0xc4, 0xec, 0x9a, 0xbc, 0xd5, 0xeb,
    0xec, 0xb5, 0xb2, 0xf5, 0xff, 0xef, 0xce, 0x7f, 0x74, 0x4c, 0x3e, 0x5b, 0xe9, 0x31, 0xde, 0xea, 0xb3, 0x7f, 0x6f, 0xad,
    0x0e, 0xa5, 0x76, 0x2d, 0xd5, 0xfc, 0x3a, 0xba, 0x33, 0x09, 0xf6, 0x6b, 0xef, 0xaa, 0xd7, 0xf3, 0xad, 0xca, 0x7f, 0x6d,
    0xd5, 0xd0, 0x8a, 0x75, 0xa8, 0xdb, 0x62, 0x2d, 0x96, 0xfc, 0xd7, 0xb7, 0xec, 0xe8, 0x2f, 0x33, 0x6b, 0xe2, 0x67, 0xea,
    0x84, 0x56, 0x7a, 0xc9, 0x3b, 0xf2, 0x6f, 0xa9, 0x6f, 0x0f, 0xca, 0x1f, 0xe7, 0xa7, 0xe7, 0x5f, 0xdd, 0x37, 0x64, 0xaf,
    0x18, 0xc6, 0x5b, 0x64, 0xf4, 0xe8, 0xce, 0xe4, 0x9f, 0x3d, 0x27, 0xbe, 0x37, 0xff, 0xda, 0xb5, 0xb4, 0x33, 0x73, 0x47,
    0x6b, 0xf3, 0xbf, 0x7e, 0x6f, 0xbc, 0x2f, 0xff, 0x91, 0xfe, 0x3f, 0xf3, 0x9d, 0x68, 0x2d, 0xbd, 0xce, 0xf6, 0x6a, 0xd5,
    0xd9, 0x53, 0xf9, 0x57, 0x9f, 0x35, 0x9e, 0xaf, 0xb1, 0xcb, 0x5e, 0x6d, 0xe6, 0xaf, 0x52, 0x2b, 0xce, 0xff, 0xa3, 0xdf,
    0xff, 0xb3, 0xab, 0x4a, 0x5d, 0xfe, 0xd5, 0xf9, 0xcf, 0xde, 0xf5, 0xc6, 0xc2, 0xb7, 0xdb, 0xad, 0xe4, 0x5f, 0x7b, 0x27,
    0x75, 0x46, 0xfe, 0xb9, 0x4f, 0x1b, 0xb2, 0xcf, 0x7c, 0x6b, 0x5d, 0x71, 0xb6, 0x0d, 0x7d, 0x9e, 0xfb, 0xae, 0x55, 0x69,
    0x76, 0xfd, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x37, 0xd4, 0xff, 0xaf, 0xd6, 0x6e, 0x8c, 0xae, 0xc2, 0xfe, 0xff,
    0x47, 0x73, 0xdf, 0x62, 0x1e, 0x8b, 0xcf, 0x8b, 0xe9, 0x4a, 0x98, 0xb9, 0x91, 0xe7, 0x8a, 0x4a, 0xc1, 0xda, 0x6a, 0xb1,
    0xe7, 0xf2, 0xcf, 0x55, 0x97, 0x3e, 0x51, 0x6b, 0x97, 0xad, 0xd0, 0xaa, 0xce, 0xbf, 0xbe, 0x55, 0x76, 0x8f, 0x9a, 0xbf,
    0x23, 0xff, 0x56, 0x56, 0xe3, 0xba, 0x2b, 0xff, 0xfc, 0xfb, 0x74, 0xb6, 0xf2, 0x64, 0xf7, 0x1a, 0xbd, 0x9f, 0x93, 0xff,
    0x48, 0x6f, 0x73, 0x5f, 0xfe, 0xfd, 0xc7, 0xe7, 0x8f, 0xff, 0x27, 0xd6, 0xe8, 0xff, 0xa4, 0xfc, 0xdb, 0xd0, 0xea, 0xb4,
    0x9f, 0x79, 0xfc, 0xb7, 0xd2, 0x35, 0xfa, 0xcf, 0x38, 0xff, 0xe7, 0xaf, 0xff, 0xda, 0x0d, 0xfd, 0x7f, 0xfb, 0xcf, 0xb7,
    0x52, 0xd4, 0xb5, 0xca, 0x7c, 0x25, 0x61, 0x75, 0xfe, 0x77, 0xf5, 0xff, 0x6d, 0x61, 0x55, 0xe6, 0xda, 0xf3, 0x7f, 0x76,
    0x16, 0xc6, 0xea, 0xbb, 0x6d, 0x75, 0xb6, 0xe1, 0xf3, 0xc7, 0xff, 0x93, 0xd5, 0xa3, 0x95, 0xfb, 0x73, 0x4e, 0xe5, 0xe1,
    0x67, 0xf5, 0xff, 0xf2, 0x3f, 0xa3, 0x72, 0xb6, 0x3d, 0xfc, 0x1d, 0x1d, 0xdf, 0x59, 0xaf, 0xfe, 0xc9, 0xaf, 0x54, 0xad,
    0xf5, 0xb7, 0xd7, 0xe6, 0x6b, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x93, 0x47, 0x73, 0x62, 0x72, 0x75, 0xd7, 0xd9,
    0xfa, 0x9f, 0xf8, 0x65, 0x74, 0x58, 0x9b, 0x7f, 0xce, 0x08, 0x76, 0xcd, 0xaa, 0x8e, 0xd1, 0x7d, 0x97, 0x71, 0x66, 0xfd,
    0x42, 0x55, 0xfe, 0xdf, 0x56, 0x25, 0xf1, 0x96, 0xea, 0x95, 0xda, 0x55, 0x5d, 0x79, 0xcb, 0x0c, 0x40, 0xf9, 0x9b, 0xff,
    0xf9, 0xed, 0x95, 0x70, 0xdf, 0x70, 0xfd, 0xef, 0xf8, 0xf7, 0xee, 0x90, 0xbf, 0xfc, 0xe5, 0x2f, 0x7f, 0xf9, 0xcb, 0x5f,
    0xfe, 0xaa, 0xfd, 0x73, 0xf3, 0x3f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x8f, 0xf3, 0x17 };

// Font characters rectangles data
static const Rectangle enefeteFontRecs[95] = {
    { 4, 4, 4 , 16 },
    { 16, 4, 2 , 10 },
    { 26, 4, 5 , 3 },
    { 39, 4, 7 , 10 },
    { 54, 4, 7 , 13 },
    { 69, 4, 7 , 10 },
    { 84, 4, 7 , 10 },
    { 99, 4, 2 , 3 },
    { 109, 4, 3 , 12 },
    { 120, 4, 3 , 12 },
    { 131, 4, 5 , 6 },
    { 144, 4, 6 , 5 },
    { 158, 4, 2 , 4 },
    { 168, 4, 5 , 1 },
    { 181, 4, 2 , 2 },
    { 191, 4, 4 , 10 },
    { 203, 4, 6 , 10 },
    { 217, 4, 4 , 10 },
    { 229, 4, 6 , 10 },
    { 4, 28, 6 , 10 },
    { 18, 28, 6 , 10 },
    { 32, 28, 6 , 10 },
    { 46, 28, 6 , 10 },
    { 60, 28, 6 , 10 },
    { 74, 28, 6 , 10 },
    { 88, 28, 6 , 10 },
    { 102, 28, 2 , 6 },
    { 112, 28, 2 , 8 },
    { 122, 28, 7 , 7 },
    { 137, 28, 5 , 3 },
    { 150, 28, 7 , 7 },
    { 165, 28, 6 , 10 },
    { 179, 28, 7 , 12 },
    { 194, 28, 7 , 10 },
    { 209, 28, 7 , 10 },
    { 224, 28, 7 , 10 },
    { 239, 28, 7 , 10 },
    { 4, 52, 7 , 10 },
    { 19, 52, 7 , 10 },
    { 34, 52, 7 , 10 },
    { 49, 52, 7 , 10 },
    { 64, 52, 2 , 10 },
    { 74, 52, 5 , 10 },
    { 87, 52, 7 , 10 },
    { 102, 52, 6 , 10 },
    { 116, 52, 9 , 10 },
    { 133, 52, 7 , 10 },
    { 148, 52, 7 , 10 },
    { 163, 52, 7 , 10 },
    { 178, 52, 7 , 12 },
    { 193, 52, 7 , 10 },
    { 208, 52, 7 , 10 },
    { 223, 52, 6 , 10 },
    { 237, 52, 7 , 10 },
    { 4, 76, 7 , 10 },
    { 19, 76, 8 , 10 },
    { 35, 76, 7 , 10 },
    { 50, 76, 6 , 10 },
    { 64, 76, 7 , 10 },
    { 79, 76, 4 , 12 },
    { 91, 76, 4 , 10 },
    { 103, 76, 4 , 12 },
    { 115, 76, 6 , 3 },
    { 129, 76, 7 , 1 },
    { 144, 76, 4 , 3 },
    { 156, 76, 6 , 7 },
    { 170, 76, 6 , 10 },
    { 184, 76, 6 , 7 },
    { 198, 76, 6 , 10 },
    { 212, 76, 6 , 7 },
    { 226, 76, 4 , 10 },
    { 238, 76, 6 , 9 },
    { 4, 100, 6 , 10 },
    { 18, 100, 2 , 10 },
    { 28, 100, 5 , 12 },
    { 41, 100, 6 , 10 },
    { 55, 100, 3 , 10 },
    { 66, 100, 8 , 7 },
    { 82, 100, 6 , 7 },
    { 96, 100, 6 , 7 },
    { 110, 100, 6 , 9 },
    { 124, 100, 6 , 9 },
    { 138, 100, 5 , 7 },
    { 151, 100, 6 , 7 },
    { 165, 100, 4 , 10 },
    { 177, 100, 6 , 7 },
    { 191, 100, 6 , 7 },
    { 205, 100, 8 , 7 },
    { 221, 100, 6 , 7 },
    { 235, 100, 6 , 9 },
    { 4, 124, 6 , 7 },
    { 18, 124, 5 , 12 },
    { 31, 124, 2 , 12 },
    { 41, 124, 5 , 12 },
    { 54, 124, 7 , 3 },
};

// Font glyphs info data
// NOTE: No glyphs.image data provided
static const GlyphInfo enefeteFontChars[95] = {
    { 32, 0, 12, 4, { 0 }},
    { 33, 0, 2, 3, { 0 }},
    { 34, 0, 2, 6, { 0 }},
    { 35, 0, 2, 8, { 0 }},
    { 36, 0, 1, 8, { 0 }},
    { 37, 0, 2, 8, { 0 }},
    { 38, 0, 2, 8, { 0 }},
    { 39, 0, 2, 3, { 0 }},
    { 40, 0, 2, 4, { 0 }},
    { 41, 0, 2, 4, { 0 }},
    { 42, 0, 4, 6, { 0 }},
    { 43, 0, 6, 7, { 0 }},
    { 44, 0, 10, 3, { 0 }},
    { 45, 0, 8, 6, { 0 }},
    { 46, 0, 10, 3, { 0 }},
    { 47, 0, 2, 5, { 0 }},
    { 48, 0, 2, 7, { 0 }},
    { 49, 0, 2, 7, { 0 }},
    { 50, 0, 2, 7, { 0 }},
    { 51, 0, 2, 7, { 0 }},
    { 52, 0, 2, 7, { 0 }},
    { 53, 0, 2, 7, { 0 }},
    { 54, 0, 2, 7, { 0 }},
    { 55, 0, 2, 7, { 0 }},
    { 56, 0, 2, 7, { 0 }},
    { 57, 0, 2, 7, { 0 }},
    { 58, 0, 4, 3, { 0 }},
    { 59, 0, 4, 3, { 0 }},
    { 60, 0, 4, 8, { 0 }},
    { 61, 0, 6, 6, { 0 }},
    { 62, 0, 4, 8, { 0 }},
    { 63, 0, 2, 7, { 0 }},
    { 64, 0, 2, 8, { 0 }},
    { 65, 0, 2, 8, { 0 }},
    { 66, 0, 2, 8, { 0 }},
    { 67, 0, 2, 8, { 0 }},
    { 68, 0, 2, 8, { 0 }},
    { 69, 0, 2, 8, { 0 }},
    { 70, 0, 2, 8, { 0 }},
    { 71, 0, 2, 8, { 0 }},
    { 72, 0, 2, 8, { 0 }},
    { 73, 0, 2, 3, { 0 }},
    { 74, 0, 2, 6, { 0 }},
    { 75, 0, 2, 8, { 0 }},
    { 76, 0, 2, 7, { 0 }},
    { 77, 0, 2, 10, { 0 }},
    { 78, 0, 2, 8, { 0 }},
    { 79, 0, 2, 8, { 0 }},
    { 80, 0, 2, 8, { 0 }},
    { 81, 0, 2, 8, { 0 }},
    { 82, 0, 2, 8, { 0 }},
    { 83, 0, 2, 8, { 0 }},
    { 84, 0, 2, 7, { 0 }},
    { 85, 0, 2, 8, { 0 }},
    { 86, 0, 2, 8, { 0 }},
    { 87, 0, 2, 9, { 0 }},
    { 88, 0, 2, 8, { 0 }},
    { 89, 0, 2, 7, { 0 }},
    { 90, 0, 2, 8, { 0 }},
    { 91, 0, 2, 5, { 0 }},
    { 92, 0, 2, 5, { 0 }},
    { 93, 0, 2, 5, { 0 }},
    { 94, 0, 2, 7, { 0 }},
    { 95, 0, 14, 8, { 0 }},
    { 96, 0, 2, 5, { 0 }},
    { 97, 0, 5, 7, { 0 }},
    { 98, 0, 2, 7, { 0 }},
    { 99, 0, 5, 7, { 0 }},
    { 100, 0, 2, 7, { 0 }},
    { 101, 0, 5, 7, { 0 }},
    { 102, 0, 2, 5, { 0 }},
    { 103, 0, 5, 7, { 0 }},
    { 104, 0, 2, 7, { 0 }},
    { 105, 0, 2, 3, { 0 }},
    { 106, 0, 2, 6, { 0 }},
    { 107, 0, 2, 7, { 0 }},
    { 108, 0, 2, 4, { 0 }},
    { 109, 0, 5, 9, { 0 }},
    { 110, 0, 5, 7, { 0 }},
    { 111, 0, 5, 7, { 0 }},
    { 112, 0, 5, 7, { 0 }},
    { 113, 0, 5, 7, { 0 }},
    { 114, 0, 5, 6, { 0 }},
    { 115, 0, 5, 7, { 0 }},
    { 116, 0, 2, 5, { 0 }},
    { 117, 0, 5, 7, { 0 }},
    { 118, 0, 5, 7, { 0 }},
    { 119, 0, 5, 9, { 0 }},
    { 120, 0, 5, 7, { 0 }},
    { 121, 0, 5, 7, { 0 }},
    { 122, 0, 5, 7, { 0 }},
    { 123, 0, 2, 6, { 0 }},
    { 124, 0, 2, 3, { 0 }},
    { 125, 0, 2, 6, { 0 }},
    { 126, 0, 6, 8, { 0 }},
};

// Style loading function: enefete
static void GuiLoadStyleEnefete(void)
{
    // Load style properties provided
    // NOTE: Default properties are propagated
    for (int i = 0; i < ENEFETE_STYLE_PROPS_COUNT; i++)
    {
        GuiSetStyle(enefeteStyleProps[i].controlId, enefeteStyleProps[i].propertyId, enefeteStyleProps[i].propertyValue);
    }

    // Custom font loading
    // NOTE: Compressed font image data (DEFLATE), it requires DecompressData() function
    int enefeteFontDataSize = 0;
    unsigned char *data = DecompressData(enefeteFontData, ENEFETE_COMPRESSED_DATA_SIZE, &enefeteFontDataSize);
    Image imFont = { data, 256, 256, 1, 2 };

    Font font = { 0 };
    font.baseSize = 16;
    font.glyphCount = 95;

    // Load texture from image
    font.texture = LoadTextureFromImage(imFont);
    UnloadImage(imFont);  // Uncompressed data can be unloaded from memory

    // Copy char recs data from global fontRecs
    // NOTE: Required to avoid issues if trying to free font
    font.recs = (Rectangle *)malloc(font.glyphCount*sizeof(Rectangle));
    memcpy(font.recs, enefeteFontRecs, font.glyphCount*sizeof(Rectangle));

    // Copy font char info data from global fontChars
    // NOTE: Required to avoid issues if trying to free font
    font.glyphs = (GlyphInfo *)malloc(font.glyphCount*sizeof(GlyphInfo));
    memcpy(font.glyphs, enefeteFontChars, font.glyphCount*sizeof(GlyphInfo));

    GuiSetFont(font);

    // TODO: Setup a white rectangle on the font to be used on shapes drawing,
    // this way we make sure all gui can be drawn on a single pass because no texture change is required
    // NOTE: Setting up this rectangle is a manual process (for the moment)
    Rectangle whiteChar = { 99, 4, 2, 2 };
    SetShapesTexture(font.texture, whiteChar);

    //-----------------------------------------------------------------

    // TODO: Custom user style setup: Set specific properties here (if required)
    // i.e. Controls specific BORDER_WIDTH, TEXT_PADDING, TEXT_ALIGNMENT
}
