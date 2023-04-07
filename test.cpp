
#include "jsonx.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>

#undef NDEBUG
#include <assert.h>

using namespace std;
using namespace jsonx;

static bool string_arg_val(string s)
{
    return (s == "Test");
}


static bool string_arg_const_val(const string s)
{
    return (s == "Test");
}


static bool string_arg_ref(string& s)
{
    return (s == "Test");
}


static bool string_arg_const_ref(string s)
{
    return (s == "Test");
}

static bool string_arg_const_ptr(const char *s)
{
    return (string(s) == "Test");
}

int main(int, const char *[])
{
    cout << "JsonX Test START" << endl;
    cout << endl;

    try {
        cout << "Testing constructors:" << endl;
        {
            json x1(json::undefined);
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

            json x9 = jarray({
                false,
                1,
                jobject({
                    jitem("First",99),
                    jitem("Second","Blub"),
                    jitem("Third", true)
                }),
                "Bla"
            });
            //cout << "<" << x9.write() << ">" << endl;
            assert(x9.write() == "[false,1,{\"First\":99,\"Second\":\"Blub\",\"Third\":true},\"Bla\"]");
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

            x.set("\""   "\\"  "\b"  "\f"  "\n"  "\r"  "\t");
            cout << x.write() << endl;
            assert(x.write() ==
              "\"\\\"" "\\\\" "\\b" "\\f" "\\n" "\\r" "\\t\"");

            x.set(json({
                false,
                1,
                jobject({
                    jitem("First",99),
                    jitem("Second","Blub"),
                    jitem("Third", true)
                }),
                "Bla"
            }));
            assert(x.write() == "[false,1,{\"First\":99,\"Second\":\"Blub\",\"Third\":true},\"Bla\"]");
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

            string s1 = "AB\"CD\\EF : \"";
            x = s1;
            assert(x.write() == "\"AB\\\"CD\\\\EF : \\\"\"");

            const string s2 = s1;
            x = s2;
            assert(x.write() == "\"AB\\\"CD\\\\EF : \\\"\"");

            string& s3 = s1;
            x = s3;
            assert(x.write() == "\"AB\\\"CD\\\\EF : \\\"\"");

            const string& s4 = s1;
            x = s4;
            assert(x.write() == "\"AB\\\"CD\\\\EF : \\\"\"");

            x = {
                false,
                1,
                jobject({
                    jitem("First",99),
                    jitem("Second","Blub"),
                    jitem("Third", true)
                }),
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
            const json x1{x};
            bool b = x;
            assert(b);
            b = x1;
            assert(b);

            x = false;
            const json x2{x};
            b = x;
            assert(!b);
            b = x2;
            assert(!b);

            x = 1024;
            const json x3{x};
            int i = x;
            assert(i = 1024);
            i = x3;
            assert(i = 1024);

            x = -1024;
            const json x4{x};
            i = x;
            assert(i == -1024);
            i = x4;
            assert(i == -1024);

            x = 11.31;
            const json x5{x};
            double d = x;
            assert(d == 11.31);
            d = x5;
            assert(d == 11.31);

            x = "AB\"CD\\EF : \"";
            const json x6{x};
            assert(x  == "AB\"CD\\EF : \"");
            assert(x6 == "AB\"CD\\EF : \"");
            string s1 = x;
            assert(s1 == "AB\"CD\\EF : \"");
            s1 = x6.toString(); /// Investigate: Why do we need this here?
            assert(s1 == "AB\"CD\\EF : \"");

            x = 11;
            json x7 = x;
            assert(x7 == 11);
            i = x7;
            assert(i == 11);
            const json x8 = x;
            assert(x8 == 11);
            i = x8;
            assert(i == 11);
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
            assert(x.toInt() == 12345678);
            assert(x.toUnsigned() == 12345678);
            assert(x.toReal() == 12345678);

            x.parse("+87654321");
            assert(x.isNumber());
            assert(x.isSigned());
            assert(x.toInt() == 87654321);
            assert(x.toSigned() == 87654321);
            assert(x.toReal() == 87654321);

            x.parse("-87654321");
            assert(x.isNumber());
            assert(x.isSigned());
            assert(x.toInt() == -87654321);
            assert(x.toSigned() == -87654321);
            assert(x.toReal() == -87654321);

            x.parse("5.71");
            assert(x.isNumber());
            assert(x.isReal());
            assert(x.toInt() == 6);
            assert(x.toUnsigned() == 6);
            assert(x.toSigned() == 6);
            assert(x.toReal() == 5.71);

            x.parse("-11e3");
            assert(x.isNumber());
            assert(x.isReal());
            assert(x.toInt() == -11000);
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
            assert(x[2].isInt());
            assert(x[3].isNull());
            assert(x[4].isString());
            assert(!x[5].isDefined());

            //cout << "<" << x[2] << ">" << endl;
            assert(x[2].toInt() == 1);
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
            json y(x[5]);
            assert(y != json::undefined);
            assert(!y.isDefined());
            assert(!x[6].isDefined());
            assert(x[7] == json::null);

            assert(x.size() == 8);
            x.add(json::undefined);
            assert(x[7].isNull());
            assert(x.size() == 8);
            x.add(jarray({"Prima", jobject({jitem("Klima", 12)})}));
            //cout << x.write() << endl;
            assert(x[8].isArray());
            assert(x.size() == 9);
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
            const json cx{x};
            string s = cx["Papa"];
            assert(s == "Josef");
            string st;
            st = cx["Papa"].toString(); /// Investigate: Why do we need this here?
            assert(st == "Josef");
            json& rx = x;
            s = rx["Papa"].toString(); /// Investigate: Why do we need this here?
            assert(st == "Josef");

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

        cout << "Testing string arguments:" << endl;
        {
            json x("Test");
            assert(string_arg_val(x));
            assert(string_arg_const_val(x));
            assert(string_arg_ref(x));
            assert(string_arg_const_ref(x));
            assert(string_arg_const_ptr(x.c_str()));

            const json cx(x);
            assert(string_arg_val(cx));
            assert(string_arg_const_val(cx));
            assert(string_arg_ref(json(cx)));
            assert(string_arg_const_ref(cx));
            assert(string_arg_const_ptr(cx.c_str()));

            json rg;
            rg.add("Test");
            assert(string_arg_val(rg[0]));
            assert(string_arg_const_val(rg[0]));
            assert(string_arg_ref(rg[0]));
            assert(string_arg_const_ref(rg[0]));
            assert(string_arg_const_ptr(rg[0].c_str()));

            const json crg(rg);
            assert(string_arg_val(crg[0]));
            assert(string_arg_const_val(crg[0]));
            assert(string_arg_ref(json(crg[0])));
            assert(string_arg_const_ref(crg[0]));
            assert(string_arg_const_ptr(crg[0].c_str()));

            json obj;
            obj["A"] = "Test";
            assert(string_arg_val(obj["A"]));
            assert(string_arg_const_val(obj["A"]));
            assert(string_arg_ref(obj["A"]));
            assert(string_arg_const_ref(obj["A"]));
            assert(string_arg_const_ptr(obj["A"].c_str()));

            const json cobj(obj);
            assert(string_arg_val(cobj["A"]));
            assert(string_arg_const_val(cobj["A"]));
            assert(string_arg_ref(json(cobj["A"])));
            assert(string_arg_const_ref(cobj["A"]));
            assert(string_arg_const_ptr(cobj["A"].c_str()));
        }
        cout << "OK" << endl;
        cout << endl;

        cout << "Testing comments:" << endl;
        {
            json x;
            x.parse("[\"A\" // Element A\n \n//##Kommentarzeile \n , \"B\"]//Schluß");
            assert(x.size() == 2);
            assert(x[0] == "A");
            assert(x[1] == "B");
        }
        cout << "OK" << endl;
        cout << endl;

        cout << "Testing IO:" << endl;
        {
            ifstream ifs;
            json document;
            ifs.open("./test.json");
            document.parse(ifs);
        }

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
