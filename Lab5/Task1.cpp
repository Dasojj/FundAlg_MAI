#include <iostream>
#include <fstream>
#include <memory>

class logger {
public:
    enum severity { trace, debug, information, warning, error, critical };
    virtual logger& log(const std::string& target, severity level) const = 0;
};

class stream_logger : public logger {
public:
    explicit stream_logger(std::ostream* stream, logger::severity min_severity)
        : stream_(stream), min_severity_(min_severity) {}
    logger& log(const std::string& target, severity level) const override {
        if (level >= min_severity_ && stream_) {
            *stream_ << "[" << target << "] ";
            switch (level) {
                case trace:
                    *stream_ << "TRACE: ";
                    break;
                case debug:
                    *stream_ << "DEBUG: ";
                    break;
                case information:
                    *stream_ << "INFO: ";
                    break;
                case warning:
                    *stream_ << "WARNING: ";
                    break;
                case error:
                    *stream_ << "ERROR: ";
                    break;
                case critical:
                    *stream_ << "CRITICAL: ";
                    break;
                default:
                    break;
            }
        }
        return const_cast<stream_logger&>(*this);
    }
private:
    std::ostream* stream_;
    logger::severity min_severity_;
};

class logger_builder {
public:
    logger_builder() = default;
    logger_builder& console() {
        stream_ = &std::cout;
        return *this;
    }
    logger_builder& file(const std::string& filename) {
        file_stream_.open(filename);
        stream_ = &file_stream_;
        return *this;
    }
    logger_builder& severity(logger::severity min_severity) {
        min_severity_ = min_severity;
        return *this;
    }
    std::unique_ptr<logger> build() {
        return std::make_unique<stream_logger>(stream_, min_severity_);
    }
private:
    std::ostream* stream_ = nullptr;
    std::ofstream file_stream_;
    logger::severity min_severity_ = logger::information;
};
