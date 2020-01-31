
#include "jsonx.hpp"

#include <cstdlib>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace jsonx;

int main(int, const char *[])
{
    cout << "JsonX Test START" << endl;
    cout << endl;

    try {
        cout << "Testing constructors:" << endl;
        {
            json x1(undefined);
            assert(!x1.isDefined());
            assert(x1.write() == "undefined");

            json x2(null);
            assert(x2.isDefined());
            assert(x2.isNil());
            assert(x2.write() == "null");

            json x3(true);
            assert(x3.isBool());
            assert(x3.write() == "true");

            json x4(false);
            assert(x4.isBool());
            assert(x4.write() == "false");

            json x5(1024);
            assert(x5.write() == "1024");

            json x6(-1024);
            assert(x6.write() == "-1024");

            json x7(11.31);
            assert(x7.write() == "11.31");

            json x8("AB\"CD\\EF : \"");
            assert(x8.write() == "\"AB\\\"CD\\\\EF : \\\"\"");

            json x9({
                false,
                1,
                json({{"First",99},{"Second","Blub"},{"Third", true}}),
                "Bla"
            });
            assert(x9.write() == "[false,1,{\"First\":99,\"Second\":\"Blub\",\"Third\":true},\"Bla\"]");
        }
        cout << "OK" << endl;
        cout << endl;

        cout << "Testing set functions:" << endl;
        {
            json x;
            x.setUndefined();
            assert(!x.isDefined());
            assert(x.write() == "undefined");

            x.setNil();
            assert(x.isDefined());
            assert(x.isNil());
            assert(x.write() == "null");

            x.set(true);
            assert(x.isBool());
            assert(x.write() == "true");

            x.set(false);
            assert(x.isBool());
            assert(x.write() == "false");

            x.set(1024);
            assert(x.write() == "1024");

            x.set(-1024);
            assert(x.write() == "-1024");

            x.set(11.31);
            assert(x.write() == "11.31");

            x.set("AB\"CD\\EF : \"");
            assert(x.write() == "\"AB\\\"CD\\\\EF : \\\"\"");

            x.set({
                false,
                1,
                json({{"First",99},{"Second","Blub"},{"Third", true}}),
                "Bla"
            });
            assert(x.write() == "[false,1,{\"First\":99,\"Second\":\"Blub\",\"Third\":true},\"Bla\"]");
        }
        cout << "OK" << endl;
        cout << endl;

        cout << "Testing right assignments:" << endl;
        {
            json x;
            x = undefined;
            assert(!x.isDefined());
            assert(x.write() == "undefined");

            x = null;
            assert(x.isDefined());
            assert(x.isNil());
            assert(x.write() == "null");

            x = true;
            assert(x.isBool());
            assert(x.write() == "true");

            x = false;
            assert(x.isBool());
            assert(x.write() == "false");

            x = 1024;
            assert(x.write() == "1024");

            x = -1024;
            assert(x.write() == "-1024");

            x = 11.31;
            assert(x.write() == "11.31");

            x = "AB\"CD\\EF : \"";
            assert(x.write() == "\"AB\\\"CD\\\\EF : \\\"\"");

            x =
            {
                false,
                1,
                json({{"First",99},{"Second","Blub"},{"Third", true}}),
                "Bla"
            };
            assert(x.write() == "[false,1,{\"First\":99,\"Second\":\"Blub\",\"Third\":true},\"Bla\"]");
        }
        cout << "OK" << endl;
        cout << endl;

        cout << "Testing left assignments:" << endl;
        {
            json x;

            x = true;
            bool b = x;
            assert(b);

            x = false;
            b = x;
            assert(!b);

            x = 1024;
            int i = x;
            assert(i = 1024);

            x = -1024;
            i = x;
            assert(i == -1024);

            x = 11.31;
            double d = x;
            assert(d == 11.31);

            x = "AB\"CD\\EF : \"";
            string s = x;
            assert(s == "AB\"CD\\EF : \"");
        }
        cout << "OK" << endl;
        cout << endl;

        cout << "Testing deserialization:" << endl;
        {
            json x;
            x.parse("null");
            assert(x.isNil());

            x.parse("undefined");
            assert(!x.isDefined());

            x.parse("true");
            assert(x.isBool());
            assert(x.toBool());

            x.parse("false");
            assert(x.isBool());
            assert(!x.toBool());

            x.parse("12345678");
            assert(x.isNumber());
            assert(x.isUnsigned());
            assert(x.toInteger() == 12345678);
            assert(x.toUnsigned() == 12345678);
            assert(x.toReal() == 12345678);

            x.parse("+87654321");
            assert(x.isNumber());
            assert(x.isSigned());
            assert(x.toInteger() == 87654321);
            assert(x.toSigned() == 87654321);
            assert(x.toReal() == 87654321);

            x.parse("-87654321");
            assert(x.isNumber());
            assert(x.isSigned());
            assert(x.toInteger() == -87654321);
            assert(x.toSigned() == -87654321);
            assert(x.toReal() == -87654321);

            x.parse("5.71");
            assert(x.isNumber());
            assert(x.isReal());
            assert(x.toInteger() == 6);
            assert(x.toUnsigned() == 6);
            assert(x.toSigned() == 6);
            assert(x.toReal() == 5.71);

            x.parse("-11e3");
            assert(x.isNumber());
            assert(x.isReal());
            assert(x.toInteger() == -11000);
            assert(x.toUnsigned() == 0);
            assert(x.toSigned() == -11000);
            assert(x.toReal() == -11e3);

            x.parse("[]");
            assert(x.isArray());

            x.parse("{}");
            assert(x.isObject());

            x.parse("[false,1,{\"First\":99,\"Second\":\"Blub\",\"Third\":true},\"Bla\"]");
            assert(x.write() == "[false,1,{\"First\":99,\"Second\":\"Blub\",\"Third\":true},\"Bla\"]");
        }
        cout << "OK" << endl;
        cout << endl;

        cout << "JsonX Test FINISHED" << endl;
        return EXIT_SUCCESS;
    }
    catch (const exception &ex) {
        cerr << ex.what() << endl;
        return EXIT_FAILURE;
    }
    catch (...) {
        cerr << "..." << endl;
        return EXIT_FAILURE;
    }
}
