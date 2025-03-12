#pragma once

// based on example code from waveshare
// https://www.waveshare.com/wiki/1.3inch_OLED_(B)
// https://files.waveshare.com/upload/2/2c/OLED_Module_Code.7z


// for Waveshare 1.3 inch OLED blue on black display
class SH1106DisplayDriver : public DisplayDriverITF
{
protected:
    int32_t cs_pin_;
    uint32_t width_;
    uint32_t height_;
    uint32_t image_size_;

public:
    // ctor
    SH1106DisplayDriver(int32_t cs_pin, uint32_t width, uint32_t height)
        : cs_pin_ {cs_pin}, width_ {width}, height_ {height}, image_size_ {width * height}
    {}

// implementation of the DisplayDriverITF
    virtual
    void
    init() override;

    virtual
    DisplayDriverITF::DisplayProperties
    get_display_properties() override;

    virtual
    void
    display_on() override;

    virtual
    void
    display_off() override;

    virtual
    bool
    clear_screen() override;

    virtual
    bool
    send_image(const uint8_t* image, uint32_t image_size) override;


// implementation of project specific functions
    void
    test();

protected:
    void 
    hw_reset();

    void 
    init_controller();

    void 
    send_cmd(uint8_t cmd);

    bool 
    send_data(uint8_t data);
};