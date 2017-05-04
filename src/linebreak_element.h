// cmark_renderer - a renderer for CommonMark
//
// document.cpp - implementation of the document class
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "element.h"

class linebreak_element : public element {
    virtual void add_to_leaf_block(renderer *rndr,
                                   paragraph_state &pstate) override {
        pstate.line_break();
    }
};