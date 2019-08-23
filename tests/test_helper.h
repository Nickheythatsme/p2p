//
// Created by Nick Grout on 2019-08-22.
//

#ifndef _TEST_HELPER_H_
#define _TEST_HELPER_H_
#include <iostream>

using std::cout;
using std::endl;

class VerboseObject {
    public:
        explicit VerboseObject(int x): x(x)
        {
            cout << "specialized constructor: " << x << endl;
        }
        VerboseObject(): x(-1)
        {
            cout << "constructor: " << x << endl;
        }
        VerboseObject(const VerboseObject& rhs):
            x(rhs.x)
        {
            cout << "copy constructor: " << x << endl;
        }
        VerboseObject(VerboseObject &&rhs) noexcept:
            x(rhs.x)
        {
            cout << "move constructor: " << x << endl;
            rhs.x = -1;
        }
        ~VerboseObject()
        {
            cout << "destructor: " << x << endl;
        }
        VerboseObject& operator=(VerboseObject&& rhs) noexcept
        {
            cout << "move assignment operator: " << x << " -> " << rhs.x << endl;
            x = rhs.x;
            rhs.x = -1;
            return *this;
        }
        VerboseObject& operator=(const VerboseObject& rhs)
        {
            cout << "assignment operator: " << x << " -> " << rhs.x << endl;
            x = rhs.x;
            return *this;
        }
    protected:
        int x;
};

#endif //_TEST_HELPER_H_
