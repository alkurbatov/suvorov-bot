#include "Historican.h"

Historican::~Historican() {
    if (m_file.is_open())
        m_file.close();
}

void Historican::Init(const std::string& filename_) {
    if (m_file.is_open())
        m_file.close();

    m_file.open(filename_.c_str());
}

Historican& Historican::operator<<(std::ostream& (*manipulator_)(std::ostream&)) {
    m_file << manipulator_;
    std::cout << manipulator_;

    return *this;
}

Historican gHistory;
