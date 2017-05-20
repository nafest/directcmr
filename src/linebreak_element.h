// directcmr - the CommonMark renderer you never wanted
//
// document.cpp - implementation of the document class
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "element.h"

namespace dcmr {

class linebreak_element : public element {
    virtual void add_to_leaf_block(backend *bcknd,
                                   paragraph_state &pstate) override {
        pstate.line_break();
    }
};
}
