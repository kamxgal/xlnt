#include <xlnt/utils/load_progress_notifier.hpp>

namespace
{

int calc_progress(size_t current, size_t max)
{
    if (max == 0) {
        return static_cast<int>(current);
    }
    return static_cast<int>(current * 100.0f / max);
}

}  // namespace

namespace xlnt
{

load_progress_notifier::load_progress_notifier()
    : notify_([](int){})
    , worksheets_num_{0}
    , loaded_worksheets_num_{0}
    , last_notified_progress_{-1}
{}

void load_progress_notifier::init(std::function<void(int)> notify)
{
    notify_ = std::move(notify);
    last_notified_progress_ = -1;
}

void load_progress_notifier::set_worksheets_num(size_t num)
{
    worksheets_num_ = num;
    loaded_worksheets_num_ = 0;
    last_notified_progress_ = -1;
}

void load_progress_notifier::notify_document_load_start()
{
    notify_(0);
    last_notified_progress_ = 0;
}

void load_progress_notifier::notify_document_load_end()
{
    notify_(100);
    last_notified_progress_ = 100;
}

void load_progress_notifier::notify_worksheet_load_start()
{
    int progress = 10 + calc_progress(loaded_worksheets_num_, worksheets_num_) * 0.8f;

    if (progress > last_notified_progress_)
    {
        notify_(progress);
        last_notified_progress_ = progress;
    }
}

void load_progress_notifier::notify_worksheet_load_progress(size_t parsed_bytes, size_t total_bytes)
{
    float progress_per_ws = 1.0f / worksheets_num_;
    int ws_load_progress = calc_progress(parsed_bytes, total_bytes);
    int progress = 10 + calc_progress(loaded_worksheets_num_, worksheets_num_) * 0.8f + ws_load_progress * progress_per_ws;

    if (progress > last_notified_progress_)
    {
        notify_(progress);
        last_notified_progress_ = progress;
    }
}

void load_progress_notifier::notify_worksheet_load_end()
{
    ++loaded_worksheets_num_;
    int progress = 10 + calc_progress(loaded_worksheets_num_, worksheets_num_) * 0.8f;

    if (progress > last_notified_progress_)
    {
        notify_(progress);
        last_notified_progress_ = progress;
    }
}

}  // namespace xlnt
