#pragma once


class USBCmdProcessor
{
protected:
    std::array<uint8_t, VC5_MsgHeader::max_cmd_length> cmd_buffer_ {0};
    std::array<uint8_t, VC5_MsgHeader::max_cmd_length> rsp_buffer_ {0};

public:
    // ctor
    USBCmdProcessor();

    void
    task();

protected:
    void
    process_cmd(uint32_t len);

    void
    send_rsp_done(uint8_t unique_id, uint8_t status);
};

void
vendor_1_task();