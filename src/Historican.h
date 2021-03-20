// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#pragma once

#include <fstream>
#include <iostream>

struct TypeWriter {
    ~TypeWriter();

    void RedirectToFile(const std::string& filename_);

    template <class T>
    TypeWriter& operator<<(const T& data_) {
        if (m_file.is_open())
            m_file << data_;

        std::cout << data_;

        return *this;
    }

    TypeWriter& operator<<(std::ostream& (*manipulator_)(std::ostream&));

 private:
    std::ofstream m_file;
};

struct Historican {
    explicit Historican(const std::string topic_);

    static void Init(const std::string& filename_);

    TypeWriter& info() const;

    TypeWriter& warning() const;

    TypeWriter& error() const;

 private:
    std::string m_topic;
};
