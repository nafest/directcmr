// directcmr - the CommonMark renderer you never wanted
//
// utils.h - general purpose functions
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include <sstream>
#include <sstream>
#include <string>
#include <vector>

namespace dcmr {

struct color;
color string_to_color(const std::string &str);

int num_lines(const std::string &in);

class line {
  public:
    static void first_token(const std::string &text, size_t &start,
                            size_t &end) {
        end = text.find('\n', start);
    }

    static void next_token(
        const std::string &text, size_t &start,
        size_t &end) { // after the last token, set m_start to std::string::npos
        if (end == std::string::npos) {
            start = std::string::npos;
        } else {
            start = end + 1;
            end = text.find('\n', start);
        }
    }
};

class word {
  public:
    static void first_token(const std::string &text, size_t &start,
                            size_t &end) {
        if (text.size() == 0) {
            start = end = std::string::npos;
            return;
        }
        end = text.find(' ', start);
        if (end == std::string::npos) { // just a single word ..
            return;
        }

        auto len = end - start;
        while (len == 0) {
            start = end + 1;
            end = text.find(' ', start);
            if (end == std::string::npos) {
                len = text.size() - start;
                if (len == 0)
                    start = std::string::npos;
                return;
            }
            len = end - start;
        }
    }

    static void next_token(
        const std::string &text, size_t &start,
        size_t &end) { // after the last token, set m_start to std::string::npos
        if (end == std::string::npos) {
            start = std::string::npos;
        } else {
            size_t len = 0;
            do {
                start = end + 1;
                end = text.find(' ', start);
                if (end == std::string::npos) {
                    len = text.size() - start;
                    if (len == 0)
                        start = std::string::npos;
                    return;
                }
                len = end - start;
            } while (len == 0);
        }
    }
};

template<typename T> class splitter {
  public:
    class iterator {
      public:
        iterator() : m_start(std::string::npos), m_end(std::string::npos) {}
        iterator(const std::string &text) : m_text(text), m_start(0) {
            T::first_token(m_text, m_start, m_end);
        }

        iterator &operator++() {
            // after the last token, set m_start to std::string::npos
            T::next_token(m_text, m_start, m_end);
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

using line_splitter = splitter<line>;
using word_splitter = splitter<word>;
}
