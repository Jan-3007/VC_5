#pragma once


class DisplayDriverITF
{
public:
    struct DisplayProperties
    {
        uint32_t width;
        uint32_t height;
    };

    virtual
    void
    init() = 0;

    virtual
    DisplayProperties
    get_display_properties() = 0;

    virtual
    void
    display_on() = 0;

    virtual
    void
    display_off() = 0;

    virtual
    bool
    clear_screen() = 0;

    virtual
    bool
    send_image(const uint8_t* image, uint32_t image_size) = 0;
};