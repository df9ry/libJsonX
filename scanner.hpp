#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <iostream>
#include <cctype>

namespace jsonx {

class scanner
{
public:
    scanner(std::istream &_is): is{_is}
    {
        get_ch();
    }

    bool eof() const { return cur_ch == EOF; }

    void get_ch()
    {
        if (eof())
            return;
        cur_ch = is.get();
        if (eof())
            return;
        if (cur_ch == '\n') {
            cur_line += 1;
            cur_col = 0;
        } else {
            cur_col += 1;
        }
    }

    void skip_whitespace()
    {
        while (!eof()) {
            if (in_comment) {
                if (cur_ch == '\n')
                    in_comment = false;
            } else if (!std::isspace(static_cast<unsigned char>(cur_ch))) {
                if (cur_ch == '#')
                    in_comment = true;
                else
                    return;
            }
            get_ch();
        } // end while //
    }

    int cur_ch{0x00};
    int cur_line{1};
    int cur_col{0};

private:
    std::istream &is;
    bool in_comment{false};
};

} // end namespace jsonx //

#endif // SCANNER_HPP
