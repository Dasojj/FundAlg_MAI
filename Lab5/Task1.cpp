#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include "Task0.h"

class stream_logger : public logger {
private:
    std::vector<std::ostream*> streams_;
    logger::severity min_severity_;
public:
    explicit stream_logger(std::vector<std::ostream*> streams, logger::severity min_severity) : streams_(streams), min_severity_(min_severity) {}
    logger* log(const std::string& target, severity level) const override {
        for(auto strm : streams_){
            if (level >= min_severity_ && strm) {
                switch (level) {
                    case trace:
                        *strm << "TRACE: ";
                        break;
                    case debug:
                        *strm << "DEBUG: ";
                        break;
                    case information:
                        *strm << "INFO: ";
                        break;
                    case warning:
                        *strm << "WARNING: ";
                        break;
                    case error:
                        *strm << "ERROR: ";
                        break;
                    case critical:
                        *strm << "CRITICAL: ";
                        break;
                    default:
                        break;
                }
                *strm << "[" << target << "] \n";
            }
        }
        return const_cast<stream_logger*>(this);
    }
};

class logger_builder {
private:
    std::vector<std::ostream*> streams_;
    std::ostream* stream_ = nullptr;
    std::ofstream file_stream_;
    logger::severity min_severity_ = logger::information;
public:
    logger_builder() = default;
    logger_builder(const logger_builder& other) {
        min_severity_ = other.min_severity_;
        streams_ = other.streams_;
        // file_stream_ = other.file_stream_;
    }

    logger_builder& console() {
        stream_ = &std::cout;
        streams_.push_back(stream_);
        return *this;
    }
    logger_builder& file(const std::string& filename) {
        file_stream_.open(filename);
        stream_ = &file_stream_;
        streams_.push_back(stream_);
        return *this;
    }
    logger_builder& severity(logger::severity min_severity) {
        min_severity_ = min_severity;
        return *this;
    }
    std::unique_ptr<logger> build() {
        return std::make_unique<stream_logger>(streams_, min_severity_);
    }
};

class config_parser {
private:
    logger_builder builder_;
public:
    config_parser() {}

    logger_builder parse_config_file(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open config file: " << filename << std::endl;
            return logger_builder();
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }
            std::istringstream iss(line);
            std::string key;
            iss >> key;
            if (key == "output") {
                std::string output;
                iss >> output;
                if (output == "console") {
                    builder_.console();
                } else if (output == "file") {
                    std::string filename;
                    iss >> filename;
                    builder_.file(filename);
                } else {
                    std::cerr << "Invalid output type: " << output << std::endl;
                }
            } else if (key == "level") {
                int level;
                iss >> level;
                builder_.severity(static_cast<logger::severity>(level));
            } else {
                std::cerr << "Invalid config key: " << key << std::endl;
            }
        }
        return builder_;
    }
};

int main() {
    config_parser parser;
    logger_builder builder = parser.parse_config_file("logger_config.txt");
    std::unique_ptr<logger> log = builder.build();
    log->log("Example log message (level trace)", logger::severity::trace)
       ->log("Example log message (level debug)", logger::severity::debug)
       ->log("Example log message (level info)", logger::severity::information)
       ->log("Example log message (level warning)", logger::severity::warning)
       ->log("Example log message (level error)", logger::severity::error)
       ->log("Example log message (level critical)", logger::severity::critical);
    return 0;
}