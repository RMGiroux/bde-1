// bdlma_concurrentallocatoradapter.t.cpp                             -*-C++-*-

#include <bdlma_concurrentallocatoradapter.h>

#include <bdlqq_xxxthread.h>
#include <bdlqq_lockguard.h>

#include <bslma_testallocator.h>            // for testing only
#include <bdlqq_barrier.h>                  // for testing only
#include <bslma_testallocator.h>            // for testing only
#include <bslma_testallocatorexception.h>   // for testing only

#include <bsls_alignment.h>
#include <bsls_stopwatch.h>

#include <bsl_iostream.h>
#include <bsl_string.h>
#include <bsl_vector.h>

#include <bsl_cstdlib.h>                         // atoi()
#include <bsl_cstring.h>                         // memcpy(), memset()

using namespace BloombergLP;
using namespace bsl;  // automatically added by script

//=============================================================================
//                                  TEST PLAN
//-----------------------------------------------------------------------------
//                                  Overview
//                                  --------
// The 'bdlma::ConcurrentAllocatorAdapter' class consists of one constructor, a
// destructor, and four manipulators.  The manipulators are used to allocate,
// deallocate, and reserve memory.  Since this component is a memory manager,
// the 'bslma_testallocator' component is used extensively to verify expected
// behaviors.  Note that the copying of objects is explicitly disallowed since
// the copy constructor and assignment operator are declared 'private' and left
// unimplemented.  So we are primarily concerned that the internal memory
// management system functions as expected and that the manipulators operator
// correctly.  Note that memory allocation must be tested for exception
// neutrality (also via the 'bslma_testallocator' component).  Several small
// helper functions are also used to facilitate testing.
//-----------------------------------------------------------------------------
// [2] bdlma::ConcurrentAllocatorAdapter(int               numPools,
//                                         bslma::Allocator *ba = 0);
// [2] ~bdlma::ConcurrentAllocatorAdapter();
// [3] void *allocate(int size);
// [4] void deallocate(void *address);
// [5] void release();
// [6] void reserveCapacity(int size, int numObjects);
//-----------------------------------------------------------------------------
// [1] BREATHING TEST
// [7] USAGE EXAMPLE
//=============================================================================
//                    STANDARD BDE ASSERT TEST MACRO
//-----------------------------------------------------------------------------

static int testStatus = 0;

static void aSsErT(int c, const char *s, int i) {
    if (c) {
        cout << "Error " << __FILE__ << "(" << i << "): " << s
             << "    (failed)" << endl;
        if (testStatus >= 0 && testStatus <= 100) ++testStatus;
    }
}
# define ASSERT(X) { aSsErT(!(X), #X, __LINE__); }

//=============================================================================
//                  STANDARD BDE LOOP-ASSERT TEST MACROS
//-----------------------------------------------------------------------------

#define LOOP_ASSERT(I,X) { \
    if (!(X)) { cout << #I << ": " << I << "\n"; aSsErT(1, #X, __LINE__);}}

#define LOOP2_ASSERT(I,J,X) { \
    if (!(X)) { cout << #I << ": " << I << "\t" << #J << ": " \
              << J << "\n"; aSsErT(1, #X, __LINE__); } }

#define LOOP3_ASSERT(I,J,K,X) { \
   if (!(X)) { cout << #I << ": " << I << "\t" << #J << ": " << J << "\t" \
              << #K << ": " << K << "\n"; aSsErT(1, #X, __LINE__); } }

//=============================================================================
//                  SEMI-STANDARD TEST OUTPUT MACROS
//-----------------------------------------------------------------------------

#define P(X) cout << #X " = " << (X) << endl; // Print identifier and value.
#define Q(X) cout << "<| " #X " |>" << endl;  // Quote identifier literally.
#define P_(X) cout << #X " = " << (X) << ", " << flush; // P(X) without '\n'
#define L_ __LINE__                           // current Line number
#define TAB cout << '\t';

//=============================================================================
//                       GLOBAL TYPES AND CONSTANTS
//-----------------------------------------------------------------------------

typedef bdlma::ConcurrentAllocatorAdapter Obj;

//=============================================================================
//                      HELPER FUNCTIONS FOR TESTING
//-----------------------------------------------------------------------------

class NoopAllocator : public bslma::Allocator{
    // The NoopAllocator is an empty implementation of the 'bslma::Allocator'
    // protocol that tracks the methods called and verifies the
    // 'bslma::Allocator' protocol.

  private:
    char const **d_lastMethod;  // the last method called on this instance

  public:

    // CREATORS
    NoopAllocator(char const **lastMethod)
        // Create this 'NoopAllocator' with the specified 'lastMethod' to store
        // the last method called.
    : d_lastMethod(lastMethod)
    {
        *d_lastMethod = "NoopAllocator";
    }

    ~NoopAllocator();
        // Destroy this 'NoopAllocator'

    // MANIPULATORS
    virtual void *allocate(size_type size);
        // This method has no effect for this allocator except that a
        // subsequent call to 'lastMethod()' will return 'allocate'.

    virtual void deallocate(void *address);
        // This method has no effect for this allocator except that a
        // subsequent call to 'lastMethod()' will return 'deallocate'.

    // ACCESSORS
    const char *lastMethod() { return *d_lastMethod; }
        // Return the last method called on this 'NoopAllocator' instance.
};

NoopAllocator::~NoopAllocator()
{
    *d_lastMethod = "~NoopAllocator";
}

void *NoopAllocator::allocate(size_type size)
{
    (void)size;
    *d_lastMethod = "allocate";
    return 0;
}

void NoopAllocator::deallocate(void *address)
{
    (void *)address;
    *d_lastMethod = "deallocate";
}

enum {
    k_NUM_THREADS = 10
};
struct WorkerArgs {
    Obj       *d_allocator; // allocator to perform allocations
    const int *d_sizes;     // array of allocations sizes
    int        d_numSizes;  // number of allocations

};

bdlqq::Barrier g_barrier(k_NUM_THREADS);
extern "C" void *workerThread(void *arg) {
    // Perform a series of allocate, and deallocate operations on the
    // 'bdlma::ConcurrentAllocatorAdapter' and verify their results.  This
    // operation is intended to be a thread entry point.  Cast the specified
    // 'args' to a 'WorkerArgs', and perform a series of
    // '(WorkerArgs *)args->d_numSizes' allocations using the corresponding
    // allocations sizes specified by '(WorkerARgs *)args->d_sizes'.  Use the
    // barrier 'g_barrier' to ensure tests are performed while the allocator is
    // in the correct state.

    WorkerArgs *args = (WorkerArgs *) arg;
    ASSERT(0 != args);
    ASSERT(0 != args->d_sizes);

    Obj       *allocator  = args->d_allocator;
    const int *allocSizes = args->d_sizes;
    const int  numAllocs  = args->d_numSizes;

    bsl::vector<char *> blocks(bslma::Default::allocator(0));
    blocks.resize(numAllocs);

    g_barrier.wait();

    // Perform allocations
    for (int i = 0; i < numAllocs; ++i) {
        blocks[i] = (char *)allocator->allocate(allocSizes[i]);
    }

    // deallocate all the blocks
    for (int i = 0; i < numAllocs; ++i) {
        allocator->deallocate(blocks[i]);
    }

    g_barrier.wait();

    return arg;
}

//=============================================================================
//                              USAGE EXAMPLE
//-----------------------------------------------------------------------------

///Usage
///-----
// In the following usage example, we develop a simple 'AddressBook' class
// containing two thread-enabled vectors of strings: one for names, the other
// for addresses.  We use a 'bdlma::ConcurrentAllocatorAdapter' to synchronize
// memory allocations across our two thread-enabled vectors.  For the purpose
// of this discussion, we first define a simple thread-enabled vector:
//..
    template <typename TYPE>
    class ThreadEnabledVector {
        // This class defines a trivial thread-enabled vector.

        // DATA
        mutable bdlqq::Mutex d_mutex;     // synchronize access
        bsl::vector<TYPE>    d_elements;  // underlying list of strings

        // NOT IMPLEMENTED
        ThreadEnabledVector(const ThreadEnabledVector&);
        ThreadEnabledVector& operator=(const ThreadEnabledVector&);

      public:
        // CREATORS
        ThreadEnabledVector(bslma::Allocator *basicAllocator = 0)
            // Create a thread-enabled vector.  Optionally specify a
            // 'basicAllocator' used to supply memory.  If 'basicAllocator'
            // is 0, the currently installed default allocator is used.
        : d_elements(basicAllocator)
        {
        }

        ~ThreadEnabledVector() {}
            // Destroy this thread-enabled vector object.

        // MANIPULATORS
        int pushBack(const TYPE& value)
            // Append the specified 'value' to this thread-enabled vector and
            // return the index of the new element.
        {
            bdlqq::LockGuard<bdlqq::Mutex> guard(&d_mutex);
            d_elements.push_back(value);
            return static_cast<int>(d_elements.size()) - 1;
        }

        void set(int index, const TYPE& value)
            // Set the element at the specified 'index' in this thread-enabled
            // vector to the specified 'value'.  The behavior is undefined
            // unless '0 <= index < length()'.
        {
            bdlqq::LockGuard<bdlqq::Mutex> guard(&d_mutex);
            d_elements[index] = value;
        }

        // ACCESSORS
        TYPE element(int index) const
            // Return the value of the element at the specified 'index' in this
            // thread-enabled vector.  Note that elements are returned *by*
            // *value* because references to elements managed by this container
            // may be invalidated by another thread.
        {
            bdlqq::LockGuard<bdlqq::Mutex> guard(&d_mutex);
            return d_elements[index];
        }

        int length() const
            // Return the number of elements in this thread-enabled vector.
        {
            bdlqq::LockGuard<bdlqq::Mutex> guard(&d_mutex);
            return static_cast<int>(d_elements.size());
        }
    };
//..
// We use this thread-enabled vector to create a AddressBook class.  However,
// we use the 'bdlma::ConcurrentAllocatorAdapter' to prevent our two
// (thread-enabled) vectors from attempting synchronous memory allocations from
// our (potentially) non-thread safe 'bslma::Allocator'.  Note that we define a
// local class, 'AddressBook_PrivateData', in order to guarantee that
// 'd_allocatorAdapter' and 'd_mutex' are initialized before the thread-enabled
// vectors that depend on them:
//..
    struct AddressBook_PrivateData {
        // This 'struct' contains a mutex and an allocator adapter.  The
        // 'AddressBook' class will inherit from this structure, ensuring that
        // the mutex and adapter are initialized before other member variables
        // that depend on them.

        bdlqq::Mutex           d_mutex;             // synchronize allocator

        bdlma::ConcurrentAllocatorAdapter
                              d_allocatorAdapter;  // adapter for allocator

        AddressBook_PrivateData(bslma::Allocator *basicAllocator = 0)
            // Create a empty AddressBook private data object.  Optionally
            // specify a 'basicAllocator' used to supply memory.  If
            // 'basicAllocator' is 0, the currently installed default allocator
            // is used.
        : d_allocatorAdapter(&d_mutex, basicAllocator)
        {
        }
    };

    class AddressBook : private AddressBook_PrivateData {
        // This 'class' defines a thread-enabled AddressBook containing vectors
        // of names and addresses.  Note that this class uses private
        // inheritance to ensure that the allocator adapter and mutex are
        // initialized before the vectors of names and addresses.

        // DATA
        ThreadEnabledVector<bsl::string> d_names;      // list of names
        ThreadEnabledVector<bsl::string> d_addresses;  // list of addresses

      public:
        // CREATORS
        AddressBook(bslma::Allocator *basicAllocator = 0)
            // Create an empty AddressBook for storing names and addresses.
            // Optionally specify a 'basicAllocator' used to supply memory.  If
            // 'basicAllocator' is 0, the currently installed default allocator
            // is used.
        : AddressBook_PrivateData(basicAllocator)
        , d_names(&d_allocatorAdapter)
        , d_addresses(&d_allocatorAdapter)
        {
        }

        ~AddressBook()
            // Destroy this AddressBook.
        {
        }

        // MANIPULATORS
        int addName(const bsl::string& name)
            // Add the specified 'name' to this AddressBook and return the
            // index of the newly-added name.
        {
            return d_names.pushBack(name);
        }

        int addAddress(const bsl::string& address)
            // Add the specified 'address' to this AddressBook and return the
            // index of the newly-added address.
        {
            return d_addresses.pushBack(address);
        }

        // ACCESSORS
        bsl::string name(int index) const
            // Return the value of the name at the specified 'index' in this
            // AddressBook.
        {
            return d_names.element(index);
        }

        bsl::string address(int index) const
            // Return the value of the address at the specified 'index' in this
            // AddressBook.
        {
            return d_addresses.element(index);
        }

        int numNames() const
            // Return the number of names in this AddressBook.
        {
            return d_names.length();
        }

        int numAddresses() const
            // Return the number of addresses in this AddressBook.
        {
            return d_addresses.length();
        }
    };
//..

//=============================================================================
//                                MAIN PROGRAM
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int  test            = argc > 1 ? atoi(argv[1]) : 0;
    bool verbose         = argc > 2;
    bool veryVerbose     = argc > 3;
    bool veryVeryVerbose = argc > 4;

    (void)veryVerbose;

    cout << "TEST " << __FILE__ << " CASE " << test << endl;

    bslma::TestAllocator  testAllocator(veryVeryVerbose);
    bslma::Allocator     *Z = &testAllocator;

    switch (test) { case 0:
      case 3: {
        // --------------------------------------------------------------------
        // TESTING USAGE EXAMPLE
        //
        // Concerns:
        //   The usage example provided in the component header file must
        //   compile, link, and run on all platforms as shown.
        //
        // Plan:
        //   Incorporate usage example from header into driver, remove leading
        //   comment characters, and replace 'assert' with 'ASSERT'.
        //
        // Testing:
        //   USAGE EXAMPLE
        // --------------------------------------------------------------------

        if (verbose) cout << endl << "Testing Usage Example"
                          << endl << "=====================" << endl;

        AddressBook addressBook(Z);
        int nameIdx1 = addressBook.addName("Name1");
        int addrIdx1 = addressBook.addAddress("Address1");
        int nameIdx2 = addressBook.addName("Name2");
        int addrIdx2 = addressBook.addAddress("Address2");

        ASSERT(2 == addressBook.numNames());
        ASSERT(2 == addressBook.numAddresses());
        ASSERT(0 == nameIdx1);
        ASSERT(0 == addrIdx1);
        ASSERT(1 == nameIdx2);
        ASSERT(1 == addrIdx2);

        ASSERT("Name1"    == addressBook.name(nameIdx1));
        ASSERT("Address1" == addressBook.address(addrIdx1));
        ASSERT("Name2"    == addressBook.name(nameIdx2));
        ASSERT("Address2" == addressBook.address(addrIdx2));

      } break;
      case 2: {
        // --------------------------------------------------------------------
        // CONCURRENCY TEST
        //
        // Testing:
        //     Thread-safety of allocate/deallocate methods.
        //
        // --------------------------------------------------------------------

        if (verbose) cout << endl << "TEST CONCURRENCY" << endl
                                  << "================" << endl;
        bdlqq::ThreadUtil::Handle threads[k_NUM_THREADS];

        bslma::TestAllocator talloc(false);
        bdlqq::Mutex         mutex;
        Obj                 mX(&mutex, &talloc);

        const int SIZES [] = { 1 , 2 , 4,  8, 16, 32, 64, 128, 256, 512,
                               1 , 2 , 4,  8, 16, 32, 64, 128, 256, 512};

        const int NUM_SIZES = sizeof (SIZES) / sizeof(*SIZES);

        WorkerArgs args;
        args.d_allocator = &mX;
        args.d_sizes     = (const int *)&SIZES;
        args.d_numSizes  = NUM_SIZES;

        for (int i = 0; i < k_NUM_THREADS; ++i) {
            int rc =
                bdlqq::ThreadUtil::create(&threads[i], workerThread, &args);
            LOOP_ASSERT(i, 0 == rc);
        }
        for (int i = 0; i < k_NUM_THREADS; ++i) {
            int rc =
                bdlqq::ThreadUtil::join(threads[i]);
            LOOP_ASSERT(i, 0 == rc);
        }
      } break;
      case 1: {
        // --------------------------------------------------------------------
        // BREATHING TEST
        //
        // Concerns:
        //   We are concerned that the basic functionality of
        //   'bdlma::ConcurrentAllocatorAdapter' works
        //   properly.
        //
        // Plan:
        //   Create a NoopAllocator and supply it to a
        //   'bdlma::ConcurrentAllocatorAdapter' under test.  Verify that
        //   operations on the allocator are delegated to the noop allocator.
        //
        // Testing:
        //   This "test" exercises basic functionality, but tests nothing.
        // --------------------------------------------------------------------

        if (verbose) cout << endl << "Breathing Test"
                          << endl << "==============" << endl;

        const char   *lastMethod = 0;
        bdlqq::Mutex   mutex;
        NoopAllocator noopAllocator = NoopAllocator(&lastMethod);
        {
            bdlma::ConcurrentAllocatorAdapter mX(&mutex, &noopAllocator);

            ASSERT( 0 == bsl::strcmp("NoopAllocator",
                                     lastMethod));

            void *memory = mX.allocate(1);
            ASSERT(0 == bsl::strcmp("allocate", lastMethod));

            mX.deallocate(memory);
            ASSERT(0 == bsl::strcmp("deallocate", lastMethod));
        }
        ASSERT(0 == bsl::strcmp("deallocate", lastMethod));
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
