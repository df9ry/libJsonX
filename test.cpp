
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
            json x1{json::undefined};
            assert(!x1.isDefined());
            assert(x1.write() == "");

            json x2(json::null);
            assert(x2.isDefined());
            assert(x2.isNull());
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

#if 0
            json x9({
                false,
                1,
                json("First",99),
                json("Second","Blub"),
                json("Third", true),
                "Bla"
            });
            cout << "<" << x9.write() << ">" << endl;
            assert(x9.write() == "[false,1,{\"First\":99,\"Second\":\"Blub\",\"Third\":true},\"Bla\"]");
#endif
        }
        cout << "OK" << endl;
        cout << endl;

        cout << "Testing set functions:" << endl;
        {
            json x;
            x.setUndefined();
            assert(!x.isDefined());
            assert(x.write() == "");

            x.setNull();
            assert(x.isDefined());
            assert(x.isNull());
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

#if 0
            x.set(json({
                false,
                1,
                json({json({"First",99}),
                      json({"Second","Blub"}),
                      json({"Third", true})}),
                "Bla"
            }));
            assert(x.write() == "[false,1,{\"First\":99,\"Second\":\"Blub\",\"Third\":true},\"Bla\"]");
#endif
        }
        cout << "OK" << endl;
        cout << endl;

        cout << "Testing right assignments:" << endl;
        {
            json x;
            x.setUndefined();
            assert(!x.isDefined());
            assert(x.write() == "");

            x.setNull();
            assert(x.isDefined());
            assert(x.isNull());
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

#if 0
            x =
            {
                false,
                1,
                json({
                    json({"First",99}),
                    json({"Second","Blub"}),
                    json({"Third", true})}),
                "Bla"
            };
            assert(x.write() == "[false,1,{\"First\":99,\"Second\":\"Blub\",\"Third\":true},\"Bla\"]");
#endif
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
            assert(x == "AB\"CD\\EF : \"");
            string s = x;
            assert(s == "AB\"CD\\EF : \"");
        }
        cout << "OK" << endl;
        cout << endl;

        cout << "Testing deserialization:" << endl;
        {
            json x;

            x.parse("null");
            assert(x.isNull());

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

        cout << "Testing array subscription:" << endl;
        {
            json x;

            x.parse("[null,null,1,null,\"Bla\"]");
            assert(x.size() == 5);
            assert(x[0].isNull());
            assert(x[1].isNull());
            assert(x[2].isInteger());
            assert(x[3].isNull());
            assert(x[4].isString());
            assert(!x[5].isDefined());

            //cout << "<" << x[2] << ">" << endl;
            assert(x[2].toInteger() == 1);
            assert(x[4].toString() == "Bla");

            int i1 = x[2];
            assert(i1 = 1);
            string s1 = x[4];
            assert(s1 == "Bla");

            i1 = x[2];
            assert(i1 == 1);
            x[2] = 5;
            i1 = x[2];
            assert(i1 == 5);
            x[2] = false;
            assert(!x[2]);

            assert(x[4] == "Bla");
            x[4] = "Blub";
            assert(x[4] == "Blub");

            x[7].setNull();
            json y{x[5]};
            assert(y != json::undefined);
            assert(!y.isDefined());
            assert(!x[6].isDefined());
            assert(x[7] == json::null);

            assert(x.size() == 8);
            x.add(json::undefined);
            assert(x[7].isNull());
            assert(x.size() == 8);
            x.add(json{"Prima", json{"Klima", 12}});
            //cout << x.write() << endl;
            assert(x[8].isArray());
            assert(x.size() == 9);

            x[0] = 10.1;
            x[1] = 20.2;
            assert(x[0] < x[1]);
        }
        cout << "OK" << endl;
        cout << endl;

        cout << "Testing object subscription:" << endl;
        {
            json x;
            assert(!x["Mama"].isDefined());
            assert(!x["Mama"].isDefined());
            x["Mama"] = "Maria";
            assert(x["Mama"].isDefined());
            x["Papa"] = "Josef";
            assert(x["Mama"].isDefined());
            assert(x["Papa"].isDefined());
            assert(!x["Nanu"].isDefined());
            assert(!x["Nana"].isDefined());
            assert(x["Mama"] == "Maria");
            string s1 = x["Mama"];
            assert(s1 == "Maria");
            assert(x["Papa"] == "Josef");
            string s2 = x["Papa"];
            assert(s2 == "Josef");
            x["Jahr"] = 2020;
            int j = x["Jahr"];
            assert(j == 2020);
            x["Schaltjahr"] = false;
            assert(!x["Schaltjahr"]);
            x["Prozent"] = 99.9;
            double d1 = x["Prozent"];
            assert(d1 == 99.9);
            string s3 = x.write();
            assert(s3 == "{\"Jahr\":2020,\"Mama\":\"Maria\",\"Papa\":\"Josef\",\"Prozent\":99.9,\"Schaltjahr\":false}");
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
