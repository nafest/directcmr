#pragma once

#include <sstream>
#include <string>
#include <strstream>
#include <vector>

std::vector<std::string> split_string(const std::string &in,
                                      const char split_char = ' ');

int num_lines(const std::string &text);

template <char S> class splitter {
  public:
    class iterator {
      public:
        iterator() : m_start(std::string::npos), m_end(std::string::npos) {}
        iterator(const std::string &text) : m_text(text), m_start(0) {
            m_end = m_text.find(S, m_start);
        }

        iterator &operator++() {
            // after the last token, set m_start to std::string::npos
            if (m_end == std::string::npos) {
                m_start = std::string::npos;
            } else {
                m_start = m_end + 1;
                m_end = m_text.find(S, m_start);
            }
            return *this;
        }

        const std::string operator*() {
            size_t length = (m_end == std::string::npos) ? std::string::npos
                                                         : m_end - m_start;
            return m_text.substr(m_start, length);
        }

        bool operator==(const iterator &rhs) {
            return (m_start == rhs.m_start) && (m_end == rhs.m_end);
        }

        bool operator!=(const iterator &rhs) { return !operator==(rhs); }

      private:
        std::string m_text;
        size_t m_start;
        size_t m_end;
    };

  public:
    splitter(const std::string &text) : m_text(text) {}

    iterator begin() const { return iterator(m_text); }

    iterator end() const { return iterator(); }

  private:
    std::string m_text;
};

using line_splitter = splitter<'\n'>;
