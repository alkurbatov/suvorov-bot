// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <fstream>
#include <iostream>

struct Historican {
    ~Historican();

    void Init(const std::string &filename_);

    template <class T>
    Historican& operator<<(const T& data_) {
        m_file << data_;
        std::cout << data_;

        return *this;
    }

    Historican& operator<<(std::ostream& (*manipulator_)(std::ostream&));

 private:
    std::ofstream m_file;
};

extern Historican gHistory;
