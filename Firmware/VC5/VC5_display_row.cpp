#include "VC5_global.h"


VC5DisplayRow g_display_row;


// ctor
VC5DisplayRow::VC5DisplayRow()
{
}


void VC5DisplayRow::init()
{
    for(uint8_t i = 0; i < c_num_displays; i++)
    {
        DisplayDriverITF* display = get_display_itf(i);
        display->init();
        print_on_screen(i, 0, 0, "Ready...", c_display_font);
    }
}


bool 
VC5DisplayRow::clear_display(uint8_t disp_index)
{
    DisplayDriverITF* display = get_display_itf(disp_index);

    std::fill(image_buffer_, image_buffer_ + c_display_size, '\0');  
    return display->send_image(image_buffer_, sizeof(image_buffer_));
}


bool 
VC5DisplayRow::print_on_screen(uint8_t disp_index, uint8_t padding_top, uint8_t padding_left, const char *string, sFONT *font)
{
    DisplayDriverITF* display = get_display_itf(disp_index);
    if(display == nullptr)
    {
        return false;
    }
    DisplayDriverITF::DisplayProperties props = display->get_display_properties();

    // prepare for writing a new image
    Paint_NewImage(image_buffer_, props.width, props.height, c_image_rotation, BLACK);

    // write the string as a new image
    Paint_DrawString_EN(padding_left, padding_top, string, font, BLACK, WHITE);

    // send the new image to the display
    bool succ = display->send_image(image_buffer_, sizeof(image_buffer_));

    return succ;
}


DisplayDriverITF*
VC5DisplayRow::get_display_itf(uint8_t itf_index)
{
    if(itf_index < c_num_displays)
    {
        return &display_row_[itf_index];
    }
    return nullptr;
}


void
init_displays()
{
    g_display_row.init();
}


bool 
clear_display(uint8_t disp_index)
{
    return g_display_row.clear_display(disp_index);
}


bool 
print_on_screen(uint8_t disp_index, uint8_t padding_top, uint8_t padding_left, const char* string, sFONT* font)
{
    return g_display_row.print_on_screen(disp_index, padding_top, padding_left, string, font);
}
