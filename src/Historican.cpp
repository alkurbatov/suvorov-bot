// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Alexander Kurbatov

#include "Historican.h"
#include "core/API.h"

namespace {
    TypeWriter typeWriter;
}

void TypeWriter::RedirectToFile(const std::string& filename_) {
    if (m_file.is_open())
        m_file.close();

    m_file.open(filename_.c_str());
}

TypeWriter::~TypeWriter() {
    if (m_file.is_open())
        m_file.close();
}

TypeWriter& TypeWriter::operator<<(std::ostream& (*manipulator_)(std::ostream&)) {
    if (m_file.is_open())
        m_file << manipulator_;

    std::cout << manipulator_;

    return *this;
}

Historican::Historican(const std::string topic_): m_topic(topic_) {
}

void Historican::Init(const std::string& filename_) {
    typeWriter.RedirectToFile(filename_);
}

TypeWriter& Historican::info() const {
    typeWriter << "#" << gAPI->observer().GetGameLoop()
        << " [INFO] " << m_topic << ": ";
    return typeWriter;
}

TypeWriter& Historican::warning() const {
    typeWriter << "#" << gAPI->observer().GetGameLoop()
        << " [WARNING] " << m_topic << ": ";
    return typeWriter;
}

TypeWriter& Historican::error() const {
    typeWriter << "#" << gAPI->observer().GetGameLoop()
        << " [ERROR] " << m_topic << ": ";
    return typeWriter;
}
