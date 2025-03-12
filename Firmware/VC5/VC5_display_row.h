#pragma once



class VC5DisplayRow
{
protected:
    SH1106DisplayDriver display_row_[c_num_displays]
        {
            // cs_pin, width, height
            {disp_0.cs_pin, disp_0.width, disp_0.height},
            {disp_1.cs_pin, disp_1.width, disp_1.height},
            {disp_2.cs_pin, disp_2.width, disp_2.height},
            {disp_3.cs_pin, disp_3.width, disp_3.height},
            {disp_4.cs_pin, disp_4.width, disp_4.height}
        };

    uint8_t image_buffer_[c_display_size];

public:
    // ctor
    VC5DisplayRow();

    void
    init();

    bool
    clear_display(uint8_t disp_index);

    bool
    print_on_screen(uint8_t disp_index, uint8_t padding_top, uint8_t padding_left, const char* string, sFONT* font);

protected:
    DisplayDriverITF*
    get_display_itf(uint8_t itf_index);
};


void
init_displays();

bool
clear_display(uint8_t disp_index);

bool
print_on_screen(uint8_t disp_index, uint8_t padding_top, uint8_t padding_left, const char* string, sFONT* font);
