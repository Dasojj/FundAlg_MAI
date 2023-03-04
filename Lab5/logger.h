#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <string>
#include <map>
#include <ctime>

class logger {
public:
    enum severity { trace, debug, information, warning, error, critical };
    virtual logger* log(const std::string& target, severity level) const = 0;
};

class stream_logger : public logger {
private:
    static std::map<std::string, std::pair<std::ostream*, size_t>> streams_;
    std::map<std::string, std::pair<std::ostream*, logger::severity>> current_streams_;
public:
    explicit stream_logger(std::map<std::string, logger::severity> const & targets) {
        for(auto & target : targets){
            auto new_stream = streams_.find(target.first);
            std::ostream *stream = nullptr;
            if(new_stream == streams_.end()){
                if(target.first == "console") stream = &(std::cout);
                else stream = new std::ofstream(target.first);
                streams_.insert(std::make_pair(target.first, std::make_pair(stream, 1)));
            }
            else{
                stream = new_stream->second.first;
                new_stream->second.second++;
            }
            current_streams_.insert(std::make_pair(target.first, std::make_pair(stream, target.second)));
        }
    }

    logger* log(const std::string& target, severity level) const override {
        auto t_now = std::time(nullptr);
        auto t_local = *std::localtime(&t_now);
        for(auto strm : current_streams_){
            if (level >= strm.second.second && strm.second.first) {
                *(strm.second.first) << "[" << std::put_time(&t_local, "%d/%m/%Y %H:%M:%S") << "]";
                switch (level) {
                    case trace:
                        *(strm.second.first) << "[TRACE] ";
                        break;
                    case debug:
                        *(strm.second.first) << "[DEBUG] ";
                        break;
                    case information:
                        *(strm.second.first) << "[INFO] ";
                        break;
                    case warning:
                        *(strm.second.first) << "[WARNING] ";
                        break;
                    case error:
                        *(strm.second.first) << "[ERROR] ";
                        break;
                    case critical:
                        *(strm.second.first) << "[CRITICAL] ";
                        break;
                    default:
                        break;
                }
                *(strm.second.first) << target << std::endl;
            }
        }
        return const_cast<stream_logger*>(this);
    }

    ~stream_logger(){
        for(auto & current_stream : current_streams_){
            auto global_stream = streams_.find(current_stream.first);
            if(global_stream != streams_.end()){
                if (--(global_stream->second.second) == 0){
                    if (global_stream->second.first != nullptr){
                        global_stream->second.first->flush();
                        if(global_stream->first != "console"){
                            static_cast<std::ofstream*>(global_stream->second.first)->close();
                            delete global_stream->second.first;
                        }
                    }
                    streams_.erase(global_stream);
                }
            }
        }
    }
};

std::map<std::string, std::pair<std::ostream*, size_t>> stream_logger::streams_ = std::map<std::string, std::pair<std::ostream*, size_t>>();

class logger_builder {
private:
    std::map<std::string, logger::severity> built_info_;
public:
    logger_builder() = default;
    logger_builder(const logger_builder& other) {
        built_info_ = other.built_info_;
    }
    logger_builder& add_stream(const std::string& output, logger::severity min_sev) {
        built_info_[output] = min_sev;
        return *this;
    }
    logger_builder& clear() {
        built_info_.clear();
        return *this;
    }
    stream_logger* build() {
        return new stream_logger(built_info_);
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
            int level;
            std::string output;
            iss >> level >> output;
            builder_.add_stream(output, static_cast<logger::severity>(level));
        }
        file.close();
        return builder_;
    }
};

#endif //LOGGER_H