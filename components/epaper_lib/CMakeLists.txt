set(embed_files "")

if(CONFIG_FONT12_EMBEDDED)
    list(APPEND embed_files
        "Fonts/Font12/font12EN.FON"
        "Fonts/Font12/GBK_font12CH.FON"
        "Fonts/Font12/GBK_font12CH_ASICC.FON"
        "Fonts/Font12/UTF_font12CH.FON"
        "Fonts/Font12/UTF_font12CH_ASICC.FON")
endif()

if(CONFIG_FONT16_EMBEDDED)
    list(APPEND embed_files
        "Fonts/Font16/font16EN.FON"
        "Fonts/Font16/GBK_font16CH.FON"
        "Fonts/Font16/GBK_font16CH_ASICC.FON"
        "Fonts/Font16/UTF_font16CH.FON"
        "Fonts/Font16/UTF_font16CH_ASICC.FON")
endif()

if(CONFIG_FONT18_EMBEDDED)
    list(APPEND embed_files
        "Fonts/Font18/font18EN.FON"
        "Fonts/Font18/GBK_font18CH.FON"
        "Fonts/Font18/GBK_font18CH_ASICC.FON"
        "Fonts/Font18/UTF_font18CH.FON"
        "Fonts/Font18/UTF_font18CH_ASICC.FON")
endif()

if(CONFIG_FONT24_EMBEDDED)
    list(APPEND embed_files
        "Fonts/Font24/font24EN.FON"
        "Fonts/Font24/GBK_font24CH.FON"
        "Fonts/Font24/GBK_font24CH_ASICC.FON"
        "Fonts/Font24/UTF_font24CH.FON"
        "Fonts/Font24/UTF_font24CH_ASICC.FON")
endif()

if(CONFIG_FONT28_EMBEDDED)
    list(APPEND embed_files
        "Fonts/Font28/font28EN.FON"
        "Fonts/Font28/GBK_font28CH.FON"
        "Fonts/Font28/GBK_font28CH_ASICC.FON"
        "Fonts/Font28/UTF_font28CH.FON"
        "Fonts/Font28/UTF_font28CH_ASICC.FON")
endif()

if(CONFIG_FONT36_EMBEDDED)
    list(APPEND embed_files
        "Fonts/Font36/font36EN.FON"
        "Fonts/Font36/GBK_font36CH.FON"
        "Fonts/Font36/GBK_font36CH_ASICC.FON"
        "Fonts/Font36/UTF_font36CH.FON"
        "Fonts/Font36/UTF_font36CH_ASICC.FON")
endif()

if(CONFIG_FONT48_EMBEDDED)
    list(APPEND embed_files
        "Fonts/Font48/font48EN.FON"
        "Fonts/Font48/GBK_font48CH.FON"
        "Fonts/Font48/GBK_font48CH_ASICC.FON"
        "Fonts/Font48/UTF_font48CH.FON"
        "Fonts/Font48/UTF_font48CH_ASICC.FON")
endif()

if(CONFIG_Auxiliary_Font_EMBEDDED)
    list(APPEND embed_files
        "Fonts/ASCII/font80EN.FON"
        "Fonts/ASCII/font182EN.FON")
endif()

idf_component_register(
    SRCS
        "GUI_Paint.c"
        "GUI_BMPfile.c"
        "Fonts/font.c"
        "Fonts/fonts.c"
    INCLUDE_DIRS
        "."
        "Fonts"
    REQUIRES
        esp_driver_gpio
        esp_driver_spi
        freertos
    EMBED_FILES ${embed_files}
)
