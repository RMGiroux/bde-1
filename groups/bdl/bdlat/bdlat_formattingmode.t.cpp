// bdlat_formattingmode.t.cpp                                         -*-C++-*-

#include <bdlat_formattingmode.h>

#include <bdls_testutil.h>

#include <bslmf_assert.h>

#include <bsl_cstring.h>     // strlen()
#include <bsl_cstdlib.h>     // atoi()
#include <bsl_iostream.h>

using namespace BloombergLP;
using namespace bsl;  // automatically added by script

//=============================================================================
//                                 TEST PLAN
//-----------------------------------------------------------------------------
//                                 Overview
//                                 --------
// Due to the extremely simple nature of this component, we only have to test
// that the flags are defined and that they do not overlap in bit patterns.
//-----------------------------------------------------------------------------
//  bdeat_FormattingMode::DEFAULT
//  bdeat_FormattingMode::DEC
//  bdeat_FormattingMode::HEX
//  bdeat_FormattingMode::BASE64
//  bdeat_FormattingMode::TEXT
//  bdeat_FormattingMode::LIST
//  bdeat_FormattingMode::UNTAGGED
//  bdeat_FormattingMode::ATTRIBUTE
//  bdeat_FormattingMode::SIMPLE_CONTENT
//  bdeat_FormattingMode::NILLABLE
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// ============================================================================
//                     STANDARD BDE ASSERT TEST FUNCTION
// ----------------------------------------------------------------------------

namespace {

int testStatus = 0;

void aSsErT(bool condition, const char *message, int line)
{
    if (condition) {
        cout << "Error " __FILE__ "(" << line << "): " << message
             << "    (failed)" << endl;

        if (0 <= testStatus && testStatus <= 100) {
            ++testStatus;
        }
    }
}

}  // close unnamed namespace

// ============================================================================
//               STANDARD BDE TEST DRIVER MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT       BDLS_TESTUTIL_ASSERT
#define ASSERTV      BDLS_TESTUTIL_ASSERTV

#define LOOP_ASSERT  BDLS_TESTUTIL_LOOP_ASSERT
#define LOOP0_ASSERT BDLS_TESTUTIL_LOOP0_ASSERT
#define LOOP1_ASSERT BDLS_TESTUTIL_LOOP1_ASSERT
#define LOOP2_ASSERT BDLS_TESTUTIL_LOOP2_ASSERT
#define LOOP3_ASSERT BDLS_TESTUTIL_LOOP3_ASSERT
#define LOOP4_ASSERT BDLS_TESTUTIL_LOOP4_ASSERT
#define LOOP5_ASSERT BDLS_TESTUTIL_LOOP5_ASSERT
#define LOOP6_ASSERT BDLS_TESTUTIL_LOOP6_ASSERT

#define Q            BDLS_TESTUTIL_Q   // Quote identifier literally.
#define P            BDLS_TESTUTIL_P   // Print identifier and value.
#define P_           BDLS_TESTUTIL_P_  // P(X) without '\n'.
#define T_           BDLS_TESTUTIL_T_  // Print a tab (w/o newline).
#define L_           BDLS_TESTUTIL_L_  // current Line number

//=============================================================================
//                  GLOBAL TYPEDEFS/CONSTANTS FOR TESTING
//-----------------------------------------------------------------------------

typedef bdeat_FormattingMode FM;

//=============================================================================
//                              MAIN PROGRAM
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int test = argc > 1 ? atoi(argv[1]) : 0;
    int verbose = argc > 2;
    int veryVerbose = argc > 3;
//  int veryVeryVerbose = argc > 4;

    cout << "TEST " << __FILE__ << " CASE " << test << endl;;

    switch (test) { case 0:  // Zero is always the leading case.
      case 1: {
        // --------------------------------------------------------------------
        // Basic Attribute Test:
        //
        // Concerns:
        //   - BDEAT_TYPE_MASK and BDEAT_FLAGS_MASK are disjoint (i.e., have no
        //     bits in common).
        //   - Each of the mode enumerations are unique.
        //   - All of the bits of each schema type enumeration are within the
        //     BDEAT_TYPE_MASK bit-mask.
        //   - All of the bits of formatting flag enumeration are within the
        //     BDEAT_FLAGS_MASK bit-mask.
        //   - None of the formatting flag enumerations share any bits in
        //     common with any of the other mode enumerations.
        //
        // Plan:
        //   - Test that BDEAT_TYPE_MASK and BDEAT_FLAGS_MASK have no bits in
        //     common.
        //   - Loop over each schema type enumeration and verify that it is
        //     within the TYPE_MASK bit-mask and does not overlap the
        //     BDEAT_FLAGS_MASK.
        //   - In a nested loop, test that each schema type enumeration is
        //     not equal to another schema type enumeration.
        //   - Loop over each formatting flag enumeration and verify that it is
        //     within the BDEAT_FLAGS_MASK bit-mask and does not overlap the
        //     BDEAT_TYPE_MASK.
        //   - In a nested loop, test that each formatting flag enumeration has
        //     no bits in common with another formatting flag enumeration.
        //
        // Testing:
        //    bdeat_FormattingMode::BDEAT_DEFAULT
        //    bdeat_FormattingMode::BDEAT_DEC
        //    bdeat_FormattingMode::BDEAT_HEX
        //    bdeat_FormattingMode::BDEAT_BASE64
        //    bdeat_FormattingMode::BDEAT_TEXT
        //    bdeat_FormattingMode::BDEAT_TYPE_MASK
        //    bdeat_FormattingMode::BDEAT_UNTAGGED
        //    bdeat_FormattingMode::BDEAT_ATTRIBUTE
        //    bdeat_FormattingMode::BDEAT_SIMPLE_CONTENT
        //    bdeat_FormattingMode::BDEAT_NILLABLE
        //    bdeat_FormattingMode::BDEAT_LIST
        //    bdeat_FormattingMode::BDEAT_FLAGS_MASK
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "Basic Attribute Test" << endl
                          << "====================" << endl;

        // Test that BDEAT_TYPE_MASK and BDEAT_FLAGS_MASK have no bits in
        // common.
        ASSERT(0 == (FM::BDEAT_TYPE_MASK & FM::BDEAT_FLAGS_MASK));

        if (veryVerbose) cout << "\tTesting type masks\n"
                              << "\t------------------" << endl;

        static const int TYPE_MODES[] = {
            FM::BDEAT_DEFAULT,
            FM::BDEAT_DEC,
            FM::BDEAT_HEX,
            FM::BDEAT_BASE64,
            FM::BDEAT_TEXT
        };

        static const int NUM_TYPE_MODES =
            sizeof(TYPE_MODES) / sizeof(TYPE_MODES[0]);

        // Loop over each schema type enumerations.
        for (int i = 0; i < NUM_TYPE_MODES; ++i) {
            const int TYPE_MODE1 = TYPE_MODES[i];

            // Verify that TYPE_MODE1 is within the TYPE_MASK bit-mask and
            // does not overlap the FLAGS_MASK.
            LOOP_ASSERT(TYPE_MODE1,
                        TYPE_MODE1 == (TYPE_MODE1 & FM::BDEAT_TYPE_MASK));
            LOOP_ASSERT(TYPE_MODE1,
                        0          == (TYPE_MODE1 & FM::BDEAT_FLAGS_MASK));

            for (int j = 0; j < NUM_TYPE_MODES; ++j) {
                if (j == i) continue;

                const int TYPE_MODE2 = TYPE_MODES[j];

                // test that each schema type enumeration is not equal to
                // another schema type enumeration.
                LOOP2_ASSERT(TYPE_MODE1, TYPE_MODE2, TYPE_MODE1 != TYPE_MODE2)
            }
        }

        if (veryVerbose) cout << "\tTesting flags masks\n"
                              << "\t-------------------" << endl;

        static const int FLAG_MODES[] = {
            FM::BDEAT_UNTAGGED,
            FM::BDEAT_ATTRIBUTE,
            FM::BDEAT_SIMPLE_CONTENT,
            FM::BDEAT_NILLABLE,
            FM::BDEAT_LIST
        };

        static const int NUM_FLAG_MODES =
            sizeof(FLAG_MODES) / sizeof(FLAG_MODES[0]);

        // Loop over each schema type enumeration.
        for (int i = 0; i < NUM_FLAG_MODES; ++i) {
            const int FLAG_MODE1 = FLAG_MODES[i];

            // Verify that FLAG_MODE1 is within the 'BDEAT_FLAGS_MASK' bit-mask
            // and does not overlap the 'BDEAT_TYPE_MASK'.
            LOOP_ASSERT(FLAG_MODE1,
                        FLAG_MODE1 == (FLAG_MODE1 & FM::BDEAT_FLAGS_MASK));
            LOOP_ASSERT(FLAG_MODE1,
                        0          == (FLAG_MODE1 & FM::BDEAT_TYPE_MASK));

            for (int j = 0; j < NUM_FLAG_MODES; ++j) {
                if (j == i) continue;

                const int FLAG_MODE2 = FLAG_MODES[j];

                // Test that each formatting flag enumeration has no bits in
                // common with another formatting flag enumeration.
                LOOP2_ASSERT(FLAG_MODE1, FLAG_MODE2,
                             0 == (FLAG_MODE1 & FLAG_MODE2));
            }
        }

      } break;

      default: {
        cerr << "WARNING: CASE `" << test << "' NOT FOUND." << endl;
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        cerr << "Error, non-zero test status = " << testStatus << "." << endl;
    }
    return testStatus;
}

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
