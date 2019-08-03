// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_typeenums.h>

#include <exception>
#include <string>

struct InvalidBuildCommand : public std::exception {
    explicit InvalidBuildCommand(sc2::ABILITY_ID id_) {
        m_msg = std::string("Invalid build command is used: ")
            + std::to_string(static_cast<int>(id_));
    }

    const char* what() const throw() {
        return m_msg.c_str();
    }

 private:
    std::string m_msg;
};
