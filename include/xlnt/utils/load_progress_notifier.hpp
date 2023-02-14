#pragma once

#include <functional>
#include <string>
#include <vector>


namespace xlnt
{

class load_progress_notifier
{
public:
    explicit load_progress_notifier();

    void init(std::function<void(int)> notify);

    void set_worksheets_num(size_t num);

    void notify_document_load_start();
    void notify_document_load_end();

    void notify_worksheet_load_start();
    void notify_worksheet_load_progress(size_t parsed_bytes, size_t total_bytes);
    void notify_worksheet_load_end();

private:
    std::function<void(int)> notify_;
    size_t worksheets_num_;
    size_t loaded_worksheets_num_;
    int last_notified_progress_;
};

}  // namespace xlnt
