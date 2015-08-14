// bdlat_sequencefunctions.t.cpp                                      -*-C++-*-

#include <bdlat_sequencefunctions.h>

#include <bdls_testutil.h>

#include <bdlat_attributeinfo.h>
#include <bdlat_formattingmode.h>
#include <bdlat_typetraits.h>

#include <bslalg_typetraits.h>

#include <bsl_cstdlib.h>
#include <bsl_cstring.h>
#include <bsl_iomanip.h>
#include <bsl_iostream.h>
#include <bsl_sstream.h>
#include <bsl_string.h>

#include <bdlb_string.h>

using namespace BloombergLP;
using namespace bsl;  // automatically added by script

//=============================================================================
//                             TEST PLAN
//-----------------------------------------------------------------------------
//                              Overview
//                              --------
//                                  TBD doc
//-----------------------------------------------------------------------------
// [ 2] bdeat_AttributeInfo Obj::attributeInfo(const TYPE&, int);
// [ 2] const char *Obj::className(const TYPE&);
// [ 3] int lookupAttributeInfo(*info, object, *name, nameLength);
// [ 3] int lookupAttributeInfo(*info, object, id);
// [ 2] int Obj::numAttributes(const TYPE&);
//-----------------------------------------------------------------------------
// [ 1] METHOD FORWARDING TEST
// [ 2] INFO ACCESS TEST
// [ 4] USAGE EXAMPLE

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

namespace Obj = bdeat_SequenceFunctions;

//=============================================================================
//                           CLASSES FOR TESTING
//-----------------------------------------------------------------------------

static int globalFlag = 0;

namespace geom {

class Point {

  public:
    // TYPES
    enum {
        NUM_ATTRIBUTES = 2 // the number of attributes in this class
    };

    enum {
        ATTRIBUTE_INDEX_X = 0,
            // index for 'X' attribute
        ATTRIBUTE_INDEX_Y = 1
            // index for 'Y' attribute
    };

    enum {
        ATTRIBUTE_ID_X = 1,
            // id for 'X' attribute
        ATTRIBUTE_ID_Y = 2
            // id for 'Y' attribute
    };

  public:
    // CONSTANTS
    static const char CLASS_NAME[];
        // the name of this class (i.e., "Point")

    static const double DEFAULT_X;
        // default value of 'X' attribute
    static const double DEFAULT_Y;
        // default value of 'Y' attribute

    static const bdeat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];
        // attribute info for each attribute

  public:
    // TYPE TRAITS
    BSLALG_DECLARE_NESTED_TRAITS(Point, bdeat_TypeTraitBasicSequence);

  private:
    double d_x; // X coordinate
    double d_y; // Y coordinate

  public:
    // CLASS METHODS
    static const bdeat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdeat_AttributeInfo *lookupAttributeInfo(
                                                    const char *name,
                                                    int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    Point()
        // Create an instance having default values, when appropriate.
    : d_x(0)
    , d_y(0)
    {
    }

    Point(const Point& original)
        // Create an instance having the value of the specified 'original'
        // object.
    : d_x(original.d_x)
    , d_y(original.d_y)
    {
    }

    ~Point()
        // Destroy this object.
    {
    }

    // MANIPULATORS
    Point& operator=(const Point& rhs)
        // Assign to this object the value of the specified 'rhs' object.
    {
        if (this != &rhs) {
            d_x = rhs.d_x;
            d_y = rhs.d_y;
        }
        return *this;
    }

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&, const char *, int)
        // visit the modifiable attribute with a given name
    {
        globalFlag = 1;
        return globalFlag;
    }

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&, int)
        // visit the modifiable attribute with a given id
    {
        globalFlag = 2;
        return globalFlag;
    }

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR&)
        // visits modifiable attributes
    {
        globalFlag = 3;
        return globalFlag;
    }

    double& x()
        // Return a reference to the modifiable x coordinate
    {
        return d_x;
    }

    double& y()
        // Return a reference to the modifiable y coordinate
    {
        return d_y;
    }

    // ACCESSORS
    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&, const char *, int) const
        // visit the non-modifiable attribute with a given name
    {
        globalFlag = 4;
        return globalFlag;
    }

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&, int) const
        // visit the non-modifiable attribute with a given id
    {
        globalFlag = 5;
        return globalFlag;
    }

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR&) const
        // visits non-modifiable attributes
    {
        globalFlag = 6;
        return globalFlag;
    }

    double x() const
        // Return the x coordinate
    {
        return d_x;
    }

    double y() const
        // Return the y coordinate
    {
        return d_y;
    }
};

const char Point::CLASS_NAME[] = "Point";

const double Point::DEFAULT_X = 0;
     // default value of 'X' attribute
const double Point::DEFAULT_Y = 0;
     // default value of 'Y' attribute

const bdeat_AttributeInfo Point::ATTRIBUTE_INFO_ARRAY[] = {
    {ATTRIBUTE_ID_X, "X", sizeof("X") - 1, "X coordinate"},
    {ATTRIBUTE_ID_Y, "Y", sizeof("Y") - 1, "Y coordinate"}
};

                               // -------------
                               // CLASS METHODS
                               // -------------

const bdeat_AttributeInfo *Point::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    switch(nameLength) {
        case 1: {
            switch(name[0]) {
                case 'X': {
                    return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_X];  // RETURN
                } break;
                case 'Y': {
                    return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_Y];  // RETURN
                } break;
            }
        } break;
    }
    return 0;
}

const bdeat_AttributeInfo *Point::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_X:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_X];
      case ATTRIBUTE_ID_Y:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_Y];
      default:
        return 0;
    }
}

}  // close namespace geom

//=============================================================================
//                               USAGE EXAMPLE
//-----------------------------------------------------------------------------

///Usage
///-----
// The following snippets of code illustrate the usage of this component.
// Suppose you had a 'struct' that contains three members:
//..

namespace BloombergLP {

namespace mine {

struct MySequence {
    // This struct represents a sequence containing a 'string' member, an
    // 'int' member, and a 'float' member.

    // CONSTANTS
    enum {
        NAME_ATTRIBUTE_ID   = 1,
        AGE_ATTRIBUTE_ID    = 2,
        SALARY_ATTRIBUTE_ID = 3
    };

    // DATA MEMBERS
    bsl::string d_name;
    int         d_age;
    float       d_salary;
};

}  // close namespace mine
//..
// We can now make 'mine::MySequence' expose "sequence" behavior by
// implementing the necessary 'bdeat_sequence*' functions for 'MySequence'
// inside the 'mine' namespace.  First, we should forward declare all the
// functions that we will implement inside the 'mine' namespace:
//..
namespace mine {

template <class MANIPULATOR>
int bdeat_sequenceManipulateAttribute(MySequence   *object,
                                      MANIPULATOR&  manipulator,
                                      const char   *attributeName,
                                      int           attributeNameLength);
template <class MANIPULATOR>
int bdeat_sequenceManipulateAttribute(MySequence   *object,
                                      MANIPULATOR&  manipulator,
                                      int           attributeId);
template <class MANIPULATOR>
int bdeat_sequenceManipulateAttributes(MySequence   *object,
                                       MANIPULATOR&  manipulator);
template <class ACCESSOR>
int bdeat_sequenceAccessAttribute(const MySequence&  object,
                                  ACCESSOR&          accessor,
                                  const char        *attributeName,
                                  int                attributeNameLength);
template <class ACCESSOR>
int bdeat_sequenceAccessAttribute(const MySequence& object,
                                  ACCESSOR&         accessor,
                                  int               attributeId);
template <class ACCESSOR>
int bdeat_sequenceAccessAttributes(const MySequence& object,
                                   ACCESSOR&         accessor);
bool bdeat_sequenceHasAttribute(const MySequence&  object,
                                const char        *attributeName,
                                int                attributeNameLength);
bool bdeat_sequenceHasAttribute(const MySequence& object,
                                int               attributeId);

}  // close namespace mine
//..
// Now, we will implement these functions.  Note that for this implementation,
// we will create a temporary 'bdeat_AttributeInfo' object and pass it along
// when invoking the manipulator or accessor.  See the 'bdlat_attributeinfo'
// component-level documentation for more information.  The implementation of
// the functions are as follows:
//..
template <class MANIPULATOR>
int mine::bdeat_sequenceManipulateAttribute(
                                      MySequence   *object,
                                      MANIPULATOR&  manipulator,
                                      const char   *attributeName,
                                      int           attributeNameLength)
{
    enum { NOT_FOUND = -1 };

    if (bdlb::String::areEqualCaseless("name",
                                       attributeName,
                                       attributeNameLength)) {
        return bdeat_sequenceManipulateAttribute(
                                                object,
                                                manipulator,
                                                MySequence::NAME_ATTRIBUTE_ID);
                                                                      // RETURN
    }

    if (bdlb::String::areEqualCaseless("age",
                                       attributeName,
                                       attributeNameLength)) {
        return bdeat_sequenceManipulateAttribute(
                                                 object,
                                                 manipulator,
                                                 MySequence::AGE_ATTRIBUTE_ID);
                                                                      // RETURN
    }

    if (bdlb::String::areEqualCaseless("salary",
                                       attributeName,
                                       attributeNameLength)) {
        return bdeat_sequenceManipulateAttribute(
                                              object,
                                              manipulator,
                                              MySequence::SALARY_ATTRIBUTE_ID);
                                                                      // RETURN
    }

    return NOT_FOUND;
}

template <class MANIPULATOR>
int mine::bdeat_sequenceManipulateAttribute(MySequence   *object,
                                            MANIPULATOR&  manipulator,
                                            int           attributeId)
{
    enum { NOT_FOUND = -1 };

    switch (attributeId) {
      case MySequence::NAME_ATTRIBUTE_ID: {
        bdeat_AttributeInfo info;

        info.annotation()     = "Name of employee";
        info.formattingMode() = bdeat_FormattingMode::BDEAT_DEFAULT;
        info.id()             = MySequence::NAME_ATTRIBUTE_ID;
        info.name()           = "name";
        info.nameLength()     = 4;

        return manipulator(&object->d_name, info);                    // RETURN
      }
      case MySequence::AGE_ATTRIBUTE_ID: {
        bdeat_AttributeInfo info;

        info.annotation()     = "Age of employee";
        info.formattingMode() = bdeat_FormattingMode::BDEAT_DEFAULT;
        info.id()             = MySequence::AGE_ATTRIBUTE_ID;
        info.name()           = "age";
        info.nameLength()     = 3;

        return manipulator(&object->d_age, info);                     // RETURN
      }
      case MySequence::SALARY_ATTRIBUTE_ID: {
        bdeat_AttributeInfo info;

        info.annotation()     = "Salary of employee";
        info.formattingMode() = bdeat_FormattingMode::BDEAT_DEFAULT;
        info.id()             = MySequence::SALARY_ATTRIBUTE_ID;
        info.name()           = "salary";
        info.nameLength()     = 6;

        return manipulator(&object->d_salary, info);                  // RETURN
      }
      default: {
          return NOT_FOUND;                                           // RETURN
      }
    }
}

template <class MANIPULATOR>
int mine::bdeat_sequenceManipulateAttributes(MySequence   *object,
                                             MANIPULATOR&  manipulator)
{
    int retVal;

    retVal = bdeat_sequenceManipulateAttribute(object,
                                               manipulator,
                                               MySequence::NAME_ATTRIBUTE_ID);

    if (0 != retVal) {
        return retVal;                                                // RETURN
    }

    retVal = bdeat_sequenceManipulateAttribute(object,
                                               manipulator,
                                               MySequence::AGE_ATTRIBUTE_ID);

    if (0 != retVal) {
        return retVal;                                                // RETURN
    }

    retVal = bdeat_sequenceManipulateAttribute(
                                              object,
                                              manipulator,
                                              MySequence::SALARY_ATTRIBUTE_ID);

    return retVal;
}

// ACCESSORS

template <class ACCESSOR>
int mine::bdeat_sequenceAccessAttribute(const MySequence&  object,
                                        ACCESSOR&          accessor,
                                        const char        *attributeName,
                                        int                attributeNameLength)
{
    enum { NOT_FOUND = -1 };

    if (bdlb::String::areEqualCaseless("name",
                                       attributeName,
                                       attributeNameLength)) {
        return bdeat_sequenceAccessAttribute(object,
                                             accessor,
                                             MySequence::NAME_ATTRIBUTE_ID);
                                                                      // RETURN
    }

    if (bdlb::String::areEqualCaseless("age",
                                       attributeName,
                                       attributeNameLength)) {
        return bdeat_sequenceAccessAttribute(object,
                                             accessor,
                                             MySequence::AGE_ATTRIBUTE_ID);
                                                                      // RETURN
    }

    if (bdlb::String::areEqualCaseless("salary",
                                       attributeName,
                                       attributeNameLength)) {
        return bdeat_sequenceAccessAttribute(object,
                                             accessor,
                                             MySequence::SALARY_ATTRIBUTE_ID);
                                                                      // RETURN
    }

    return NOT_FOUND;
}

template <class ACCESSOR>
int mine::bdeat_sequenceAccessAttribute(const MySequence& object,
                                        ACCESSOR&         accessor,
                                        int               attributeId)
{
    enum { NOT_FOUND = -1 };

    switch (attributeId) {
      case MySequence::NAME_ATTRIBUTE_ID: {
        bdeat_AttributeInfo info;

        info.annotation()     = "Name of employee";
        info.formattingMode() = bdeat_FormattingMode::BDEAT_DEFAULT;
        info.id()             = MySequence::NAME_ATTRIBUTE_ID;
        info.name()           = "name";
        info.nameLength()     = 4;

        return accessor(object.d_name, info);                         // RETURN
      }
      case MySequence::AGE_ATTRIBUTE_ID: {
        bdeat_AttributeInfo info;

        info.annotation()     = "Age of employee";
        info.formattingMode() = bdeat_FormattingMode::BDEAT_DEFAULT;
        info.id()             = MySequence::AGE_ATTRIBUTE_ID;
        info.name()           = "age";
        info.nameLength()     = 3;

        return accessor(object.d_age, info);                          // RETURN
      }
      case MySequence::SALARY_ATTRIBUTE_ID: {
        bdeat_AttributeInfo info;

        info.annotation()     = "Salary of employee";
        info.formattingMode() = bdeat_FormattingMode::BDEAT_DEFAULT;
        info.id()             = MySequence::SALARY_ATTRIBUTE_ID;
        info.name()           = "salary";
        info.nameLength()     = 6;

        return accessor(object.d_salary, info);                       // RETURN
      }
      default: {
          return NOT_FOUND;                                           // RETURN
      }
    }
}

template <class ACCESSOR>
int mine::bdeat_sequenceAccessAttributes(const MySequence& object,
                                         ACCESSOR&         accessor)
{
    int retVal;

    retVal = bdeat_sequenceAccessAttribute(object,
                                           accessor,
                                           MySequence::NAME_ATTRIBUTE_ID);

    if (0 != retVal) {
        return retVal;                                                // RETURN
    }

    retVal = bdeat_sequenceAccessAttribute(object,
                                           accessor,
                                           MySequence::AGE_ATTRIBUTE_ID);

    if (0 != retVal) {
        return retVal;                                                // RETURN
    }

    retVal = bdeat_sequenceAccessAttribute(object,
                                           accessor,
                                           MySequence::SALARY_ATTRIBUTE_ID);

    return retVal;
}

bool mine::bdeat_sequenceHasAttribute(const MySequence&  object,
                                      const char        *attributeName,
                                      int                attributeNameLength)
{
    return bdlb::String::areEqualCaseless("name",
                                          attributeName,
                                          attributeNameLength)
        || bdlb::String::areEqualCaseless("age",
                                          attributeName,
                                          attributeNameLength)
        || bdlb::String::areEqualCaseless("salary",
                                          attributeName,
                                          attributeNameLength);
}

bool mine::bdeat_sequenceHasAttribute(const MySequence& object,
                                      int               attributeId)
{
    return MySequence::NAME_ATTRIBUTE_ID   == attributeId
        || MySequence::AGE_ATTRIBUTE_ID    == attributeId
        || MySequence::SALARY_ATTRIBUTE_ID == attributeId;
}
//..
// Finally, we need to specialize the 'IsSequence' meta-function in the
// 'bdeat_SequenceFunctions' namespace for the 'mine::MySequence' type.  This
// makes the 'bdeat' infrastructure recognize 'mine::MySequence' as a sequence
// abstraction:
//..
namespace bdeat_SequenceFunctions {

    template <>
    struct IsSequence<mine::MySequence> {
        enum { VALUE = 1 };
    };

}  // close namespace bdeat_SequenceFunctions
}  // close enterprise namespace
//..
// The 'bdeat' infrastructure (and any component that uses this infrastructure)
// will now recognize 'mine::MySequence' as a "sequence" type.  For example,
// suppose we have the following XML data:
//..
//  <?xml version='1.0' encoding='UTF-8' ?>
//  <MySequence>
//      <name>John Doe</name>
//      <age>29</age>
//      <salary>12345.00</salary>
//  </MySequence>
//..
// Using the 'balxml_decoder' component, we can now load this XML data
// into a 'mine::MySequence' object:
//..
//  #include <balxml_decoder.h>
//
//  void decodeMySequenceFromXML(bsl::istream& inputData)
//  {
//      using namespace BloombergLP;
//
//      mine::MySequence object;
//
//      balxml::DecoderOptions options;
//      balxml::MiniReader     reader;
//      balxml::ErrorInfo      errInfo;
//
//      balxml::Decoder decoder(&options, &reader, &errInfo);
//      int result = decoder.decode(inputData, &object);
//
//      assert(0          == result);
//      assert("John Doe" == object.d_name);
//      assert(29         == object.d_age);
//      assert(12345.00   == object.d_salary);
//  }
//..
// Note that the 'bdeat' framework can be used for functionality other than
// encoding/decoding into XML.  When 'mine::MySequence' is plugged into the
// framework, then it will be automatically usable within the framework.  For
// example, the following snippets of code will print out all the attributes of
// a sequence object:
//..
struct PrintAttribute {
    // Print each visited object to the bound 'd_stream_p' object.

    // DATA MEMBERS
    bsl::ostream *d_stream_p;

    template <class TYPE, class INFO>
    int operator()(const TYPE& object, const INFO& info)
    {
        (*d_stream_p) << info.name() << ": " << object << bsl::endl;
        return 0;
    }
};

template <class TYPE>
void printSequenceAttributes(bsl::ostream& stream, const TYPE& object)
{
    PrintAttribute accessor;
    accessor.d_stream_p = &stream;

    bdeat_SequenceFunctions::accessAttributes(object, accessor);
}
//..
// Now we have a generic function that takes an output stream and a sequence
// object, and prints out each attribute with its name and value.  We can use
// this generic function as follows:
//..
void printMySequence(bsl::ostream& stream)
{
    mine::MySequence object;

    object.d_name   = "John Doe";
    object.d_age    = 25;
    object.d_salary = 12345.00;

    stream << bsl::fixed << bsl::setprecision(2);

    printSequenceAttributes(stream, object);
}
//..
// The function above will print the following to provided stream:
//..
//  name: John Doe
//  age: 25
//  salary: 12345.00
//..

//=============================================================================
//                              MAIN PROGRAM
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int test = argc > 1 ? atoi(argv[1]) : 0;
    int verbose = argc > 2;
    int veryVerbose = argc > 3;
//  int veryVeryVerbose = argc > 4;

    cout << "TEST " << __FILE__ << " CASE " << test << endl;

    switch (test) { case 0:  // Zero is always the leading case.
      case 3: {
        // --------------------------------------------------------------------
        // TESTING USAGE EXAMPLE
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //   Usage Example
        // --------------------------------------------------------------------

        if (verbose) cout << "\nTesting Usage Example"
                          << "\n=====================" << endl;

        bsl::stringstream ss;
        printMySequence(ss);

        if (veryVerbose) {
            P(ss.str());
        }

        LOOP_ASSERT(ss.str(),
                    "name: John Doe\n"
                    "age: 25\n"
                    "salary: 12345.00\n" == ss.str());
      } break;
      case 2: {
        // --------------------------------------------------------------------
        // TESTING META-FUNCTIONS
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //   struct IsChoice
        // --------------------------------------------------------------------

        if (verbose) cout << "\nTesting meta-functions"
                          << "\n======================" << endl;

        ASSERT(0 == Obj::IsSequence<int>::VALUE);
        ASSERT(1 == Obj::IsSequence<geom::Point>::VALUE);

      } break;
      case 1: {
        // --------------------------------------------------------------------
        // METHOD FORWARDING TEST
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "METHOD FORWARDING TEST" << endl
                          << "======================" << endl;

        geom::Point mP;  const geom::Point& P = mP;
        int         dummyVisitor;
        int         dummyId         = 1;
        const char *dummyName       = "foo";
        int         dummyNameLength = 3;

        globalFlag = 0;
        ASSERT(1 == bdeat_SequenceFunctions::manipulateAttribute(
                                                             &mP,
                                                             dummyVisitor,
                                                             dummyName,
                                                             dummyNameLength));
        ASSERT(1 == globalFlag);

        globalFlag = 0;
        ASSERT(2 == bdeat_SequenceFunctions::manipulateAttribute(&mP,
                                                                 dummyVisitor,
                                                                 dummyId));
        ASSERT(2 == globalFlag);

        globalFlag = 0;
        ASSERT(3 == bdeat_SequenceFunctions::manipulateAttributes(
                                                               &mP,
                                                               dummyVisitor));
        ASSERT(3 == globalFlag);

        globalFlag = 0;
        ASSERT(4 == bdeat_SequenceFunctions::accessAttribute(P,
                                                             dummyVisitor,
                                                             dummyName,
                                                             dummyNameLength));
        ASSERT(4 == globalFlag);

        globalFlag = 0;
        ASSERT(5 == bdeat_SequenceFunctions::accessAttribute(P,
                                                             dummyVisitor,
                                                             dummyId));
        ASSERT(5 == globalFlag);

        globalFlag = 0;
        ASSERT(6 == bdeat_SequenceFunctions::accessAttributes(P,
                                                              dummyVisitor));
        ASSERT(6 == globalFlag);

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
