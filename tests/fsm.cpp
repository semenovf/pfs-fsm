////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2020 Vladislav Trifochkin
//
// This file is part of [pfs-fsm](https://github.com/semenovf/pfs-fsm) library.
//
// Changelog:
//      2020.03.26 Initial version
////////////////////////////////////////////////////////////////////////////////
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <pfs/fsm.hpp>
#include <pfs/fsm_traits.hpp>
#include <iterator>
#include <string>

using string_type = std::string;
using fsm_type = pfs::fsm::fsm<string_type::const_iterator>;
using char_type = fsm_type::char_type;

/* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
static string_type const _DIGITS("0123456789");
static string_type const _HEXDIGITS("ABCDEFabcdef");

static fsm_type::transition_type hexdig_tr[] = {
      {-1, 1, fsm_type::one_of(_DIGITS.begin()   , _DIGITS.end())   , fsm_type::accept, 0, 0 }
    , {-1,-1, fsm_type::one_of(_HEXDIGITS.begin(), _HEXDIGITS.end()), fsm_type::accept, 0, 0 }
};

TEST_CASE("Alternatives") {
    string_type const hexdig("F");
    string_type const digit("9");
    string_type const notdigit("w");

    fsm_type fsm(hexdig_tr);
    fsm_type::result_type r = fsm.exec(0, hexdig.begin(), hexdig.begin());
    CHECK(r.first == false);

    string_type::const_iterator it_end;

    it_end = hexdig.begin();
    std::advance(it_end, 1);
    r = fsm.exec(0, hexdig.begin(), it_end);
    CHECK((r.first && std::distance(hexdig.begin(), r.second) == 1));

    it_end = digit.begin();
    std::advance(it_end, 1);
    r = fsm.exec(0, digit.begin(), it_end);
    CHECK((r.first && std::distance(digit.begin(), r.second) == 1));

    it_end = notdigit.begin();
    std::advance(it_end, 1);
    r = fsm.exec(0, notdigit.begin(), it_end);
    CHECK((!r.first && r.second == it_end));
}

static string_type const alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz");

static fsm_type::transition_type alphabet_length_tr[] = {
      { 1,-1, fsm_type::length(1), fsm_type::normal, 0, 0 }
    , { 2,-1, fsm_type::length(2), fsm_type::normal, 0, 0 }
    , { 3,-1, fsm_type::length(3), fsm_type::normal, 0, 0 }
    , { 4,-1, fsm_type::length(4), fsm_type::normal, 0, 0 }
    , { 5,-1, fsm_type::length(5), fsm_type::normal, 0, 0 }
    , { 6,-1, fsm_type::length(6), fsm_type::normal, 0, 0 }
    , { 7,-1, fsm_type::length(7), fsm_type::normal, 0, 0 }
    , { 8,-1, fsm_type::length(8), fsm_type::normal, 0, 0 }
    , { 9,-1, fsm_type::length(9), fsm_type::normal, 0, 0 }
    , {-1,-1, fsm_type::length(7), fsm_type::accept, 0, 0 }
};

static string_type const Z("Z");
static string_type const z("z");

static fsm_type::transition_type z_pos_tr[] = {
      { 1,-1, fsm_type::length(25)                , fsm_type::normal, 0, 0 }
    , { 2,-1, fsm_type::seq(Z.begin(), Z.end())   , fsm_type::normal, 0, 0 }
    , { 3,-1, fsm_type::length(25)                , fsm_type::normal, 0, 0 }
    , {-1,-1, fsm_type::one_of(z.begin(), z.end()), fsm_type::accept, 0, 0 }
};

TEST_CASE("Length") {
    fsm_type fsm1(alphabet_length_tr);
    fsm_type fsm2(z_pos_tr);

    CHECK(fsm1.exec(0, alphabet.begin(), alphabet.end())
            == fsm_type::result_type(true, alphabet.end()));

    CHECK(fsm2.exec(0, alphabet.begin(), alphabet.end())
            == fsm_type::result_type(true, alphabet.end()));
}

static string_type const ABCD("ABCD");
static string_type const EFGH("EFGH");
static string_type const IJKL("IJKL");
static string_type const MNOP("MNOP");
static string_type const QRST("QRST");
static string_type const UVWX("UVWX");
static string_type const YZ("YZ");
static string_type const a_z("abcdefghijklmnopqrstuvwxyz");

static fsm_type::transition_type subseq_tr[] = {
      { 1,-1, fsm_type::seq(ABCD.begin(), ABCD.end()), fsm_type::normal, 0, 0 }
    , { 2,-1, fsm_type::seq(EFGH.begin(), EFGH.end()), fsm_type::normal, 0, 0 }
    , { 3,-1, fsm_type::seq(IJKL.begin(), IJKL.end()), fsm_type::normal, 0, 0 }
    , { 4,-1, fsm_type::seq(MNOP.begin(), MNOP.end()), fsm_type::normal, 0, 0 }
    , { 5,-1, fsm_type::seq(QRST.begin(), QRST.end()), fsm_type::normal, 0, 0 }
    , { 6,-1, fsm_type::seq(UVWX.begin(), UVWX.end()), fsm_type::normal, 0, 0 }
    , { 7,-1, fsm_type::seq(YZ.begin(), YZ.end())    , fsm_type::normal, 0, 0 }
    , {-1,-1, fsm_type::seq(a_z.begin(), a_z.end())  , fsm_type::accept, 0, 0 }
};

TEST_CASE("Subsequence") {
    fsm_type fsm(subseq_tr);

    CHECK(fsm.exec(0, alphabet.begin(), alphabet.end())
            == fsm_type::result_type(true, alphabet.end()));

    CHECK(fsm.exec(0, _DIGITS.begin(), _DIGITS.begin()).first == false);
    CHECK(fsm.exec(0, _DIGITS.begin(), _DIGITS.end()).first == false);
}

static fsm_type::transition_type range_tr[] = {
      { 1,-1, fsm_type::range(char_type('A'), char_type('B')), fsm_type::normal, 0, 0 }
    , {-1,-1, fsm_type::range(char_type('B'), char_type('C')), fsm_type::accept, 0, 0 }
};

TEST_CASE("Range") {
    fsm_type fsm(range_tr);

    CHECK(fsm.exec(0, alphabet.begin(), alphabet.end()).first == true);
    CHECK(fsm.exec(0, _DIGITS.begin(), _DIGITS.end()).first == false);
}

static fsm_type::result_type is_alphabet (
                          fsm_type::iterator begin
                        , fsm_type::iterator end
                        , void * /*parse_context*/
                        , void * /*fn_context*/)
{
    string_type sample(begin, end);

    if (sample == alphabet)
        return fsm_type::result_type(true, begin + alphabet.size());

    return fsm_type::result_type(false, end);
}

static fsm_type::transition_type alphabet_func_tr[] = {
    {-1,-1, fsm_type::func(is_alphabet, 0), fsm_type::accept, 0, 0 }
};

TEST_CASE("Function") {
    fsm_type fsm(alphabet_func_tr);

    CHECK(fsm.exec(0, alphabet.begin(), alphabet.end()).first == true);
    CHECK(fsm.exec(0, _DIGITS.begin(), _DIGITS.end()).first == false);
}

/* 0*DIGIT */
static fsm_type::transition_type decimal0more_tr[] = {
      { 0, 1, fsm_type::one_of(_DIGITS.begin(), _DIGITS.end()), fsm_type::accept, 0, 0 }
    , {-1,-1, fsm_type::nothing()                             , fsm_type::accept, 0, 0 }
};

TEST_CASE("Repetition zero or more times") {
    string_type const dec("1972");
    string_type const notdec("x1972");

    fsm_type fsm(decimal0more_tr);

    string_type::const_iterator it_end;
    fsm_type::result_type r;

    it_end = dec.begin();
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((r.first && std::distance(dec.begin(), r.second) == 0));

    std::advance(it_end, 1);
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((r.first && std::distance(dec.begin(), r.second) == 1));

    std::advance(it_end, 1);
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((r.first && std::distance(dec.begin(), r.second) == 2));

    std::advance(it_end, 1);
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((r.first && std::distance(dec.begin(), r.second) == 3));

    std::advance(it_end, 1);
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((r.first && std::distance(dec.begin(), r.second) == 4));

    it_end = notdec.begin();
    r = fsm.exec(0, notdec.begin(), it_end);
    CHECK((r.first && std::distance(notdec.begin(), r.second) == 0));

    std::advance(it_end, 1);
    r = fsm.exec(0, notdec.begin(), it_end);
    CHECK((r.first && std::distance(notdec.begin(), r.second) == 0));

    std::advance(it_end, 1);
    r = fsm.exec(0, notdec.begin(), it_end);
    CHECK((r.first && std::distance(notdec.begin(), r.second) == 0));

    std::advance(it_end, 1);
    r = fsm.exec(0, notdec.begin(), it_end);
    CHECK((r.first && std::distance(notdec.begin(), r.second) == 0));

    std::advance(it_end, 1);
    r = fsm.exec(0, notdec.begin(), it_end);
    CHECK((r.first && std::distance(notdec.begin(), r.second) == 0));

    std::advance(it_end, 1);
    r = fsm.exec(0, notdec.begin(), it_end);
    CHECK((r.first && std::distance(notdec.begin(), r.second) == 0));
}

/* 1*DIGIT */
static fsm_type::transition_type decimal1more_tr[] = {
    { 0,-1, fsm_type::one_of(_DIGITS.begin(), _DIGITS.end()), fsm_type::accept, 0, 0 }
};

/* 2*DIGIT */
static fsm_type::transition_type decimal2more_tr[] = {
      { 1,-1, fsm_type::one_of(_DIGITS.begin(), _DIGITS.end()), fsm_type::normal, 0, 0 }
    , { 1,-1, fsm_type::one_of(_DIGITS.begin(), _DIGITS.end()), fsm_type::accept, 0, 0 }
};

/* 1*HEXDIG_FSM */
static fsm_type::transition_type hex_tr[] = {
    { 0,-1, fsm_type::tr(hexdig_tr), fsm_type::accept, 0, 0 }
};

TEST_CASE("Repetition one or more times") {
    string_type const dec("1972");
    string_type const notdec("x1972");
    string_type const hex("BEAF");
    string_type const nothex("BEAR");

    fsm_type fsm(decimal1more_tr, 0);

    string_type::const_iterator it_end;
    fsm_type::result_type r;

    it_end = dec.begin();
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((!r.first && r.second == it_end));

    std::advance(it_end, 1);
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((r.first && std::distance(dec.begin(), r.second) == 1));

    std::advance(it_end, 1);
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((r.first && std::distance(dec.begin(), r.second) == 2));

    std::advance(it_end, 1);
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((r.first && std::distance(dec.begin(), r.second) == 3));

    std::advance(it_end, 1);
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((r.first && std::distance(dec.begin(), r.second) == 4));

    it_end = notdec.begin();
    r = fsm.exec(0, notdec.begin(), it_end);
    CHECK(!r.first);

    std::advance(it_end, 1);
    r = fsm.exec(0, notdec.begin(), it_end);
    CHECK(!r.first);

    std::advance(it_end, 1);
    r = fsm.exec(0, notdec.begin(), it_end);
    CHECK(!r.first);

    fsm.set_transition_table(decimal2more_tr);

    it_end = dec.begin();
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK(!r.first);

    std::advance(it_end, 1);
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK(!r.first);

    std::advance(it_end, 1);
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((r.first && std::distance(dec.begin(), r.second) == 2));

    std::advance(it_end, 1);
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((r.first && std::distance(dec.begin(), r.second) == 3));

    std::advance(it_end, 1);
    r = fsm.exec(0, dec.begin(), it_end);
    CHECK((r.first && std::distance(dec.begin(), r.second) == 4));

    fsm.set_transition_table(hex_tr);

    it_end = hex.begin();
    r = fsm.exec(0, hex.begin(), it_end);
    CHECK(!r.first);

    std::advance(it_end, 1);
    r = fsm.exec(0, hex.begin(), it_end);
    CHECK((r.first && std::distance(hex.begin(), r.second) == 1));

    std::advance(it_end, 1);
    r = fsm.exec(0, hex.begin(), it_end);
    CHECK((r.first && std::distance(hex.begin(), r.second) == 2));

    std::advance(it_end, 1);
    r = fsm.exec(0, hex.begin(), it_end);
    CHECK((r.first && std::distance(hex.begin(), r.second) == 3));

    std::advance(it_end, 1);
    r = fsm.exec(0, hex.begin(), it_end);
    CHECK((r.first && std::distance(hex.begin(), r.second) == 4));

    it_end = nothex.begin();
    r = fsm.exec(0, nothex.begin(), it_end);
    CHECK(!r.first);

    std::advance(it_end, 1);
    r = fsm.exec(0, nothex.begin(), it_end);
    CHECK((r.first && std::distance(nothex.begin(), r.second) == 1));

    std::advance(it_end, 1);
    r = fsm.exec(0, nothex.begin(), it_end);
    CHECK((r.first && std::distance(nothex.begin(), r.second) == 2));

    std::advance(it_end, 1);
    r = fsm.exec(0, nothex.begin(), it_end);
    CHECK((r.first && std::distance(nothex.begin(), r.second) == 3));

    std::advance(it_end, 1);
    r = fsm.exec(0, nothex.begin(), it_end);
    CHECK((r.first && std::distance(nothex.begin(), r.second) == 3));
}

static string_type const _NONZERODIGITS("123456789");

/* NON-ZERO_DIGIT *DIGIT */
static fsm_type::transition_type non_zero_decimal_tr[] = {
      { 1,-1, fsm_type::one_of(_NONZERODIGITS.begin(), _NONZERODIGITS.end()), fsm_type::accept, 0, 0 }
    , { 1,-1, fsm_type::one_of(_DIGITS.begin(), _DIGITS.end()), fsm_type::accept, 0, 0 }
};

static string_type const _ZERO("0");
static string_type const _xX("xX");

/* (non-zero-dec dec)  / ( "0" ("x" / "X") hex ) */
static fsm_type::transition_type number_tr[] = {
      {-1, 1, fsm_type::tr(non_zero_decimal_tr)         , fsm_type::accept, 0, 0 }
    , { 2,-1, fsm_type::seq(_ZERO.begin(), _ZERO.end()) , fsm_type::normal, 0, 0 }
    , { 3,-1, fsm_type::one_of(_xX.begin(), _xX.end())  , fsm_type::normal, 0, 0 }
    , {-1,-1, fsm_type::tr(hex_tr)                      , fsm_type::accept, 0, 0 }
};

TEST_CASE("Complex alternatives") {
    string_type const hex("0xDEAD");
    string_type const decimal("1972");
    string_type const notnumber("[number]");

    fsm_type fsm(number_tr, 0);

    string_type::const_iterator it_end;
    fsm_type::result_type r;

    it_end = hex.begin();
    CHECK(fsm.exec(0, hex.begin(), it_end)
            == fsm_type::result_type(false, it_end));

    std::advance(it_end, 1);
    CHECK(fsm.exec(0, hex.begin(), it_end)
            == fsm_type::result_type(false, it_end));

    std::advance(it_end, 1);
    CHECK(fsm.exec(0, hex.begin(), it_end)
            == fsm_type::result_type(false, it_end));

    std::advance(it_end, 1);
    r = fsm.exec(0, hex.begin(), it_end);
    CHECK((r.first && std::distance(hex.begin(), r.second) == 3));

    std::advance(it_end, 1);
    r = fsm.exec(0, hex.begin(), it_end);
    CHECK((r.first && std::distance(hex.begin(), r.second) == 4));

    std::advance(it_end, 1);
    r = fsm.exec(0, hex.begin(), it_end);
    CHECK((r.first && std::distance(hex.begin(), r.second) == 5));

    std::advance(it_end, 1);
    r = fsm.exec(0, hex.begin(), it_end);
    CHECK((r.first && std::distance(hex.begin(), r.second) == 6));

    it_end = decimal.begin();
    CHECK(fsm.exec(0, decimal.begin(), it_end)
            == fsm_type::result_type(false, it_end));

    std::advance(it_end, 1);
    r = fsm.exec(0, decimal.begin(), it_end);
    CHECK((r.first && std::distance(decimal.begin(), r.second) == 1));

    std::advance(it_end, 1);
    r = fsm.exec(0, decimal.begin(), it_end);
    CHECK((r.first && std::distance(decimal.begin(), r.second) == 2));

    std::advance(it_end, 1);
    r = fsm.exec(0, decimal.begin(), it_end);
    CHECK((r.first && std::distance(decimal.begin(), r.second) == 3));

    std::advance(it_end, 1);
    r = fsm.exec(0, decimal.begin(), it_end);
    CHECK((r.first && std::distance(decimal.begin(), r.second) == 4));

    it_end = notnumber.begin();
    std::advance(it_end, 1);
    CHECK(fsm.exec(0, notnumber.begin(), it_end)
            == fsm_type::result_type(false, it_end));

    std::advance(it_end, 1);
    CHECK(fsm.exec(0, notnumber.begin(), it_end)
            == fsm_type::result_type(false, it_end));

    std::advance(it_end, 1);
    CHECK(fsm.exec(0, notnumber.begin(), it_end)
            == fsm_type::result_type(false, it_end));

    std::advance(it_end, 5);
    CHECK(fsm.exec(0, notnumber.begin(), it_end)
            == fsm_type::result_type(false, it_end));
}

static string_type const _ABC("_ABC");

static fsm_type::transition_type rpt_tr[] = {
    {-1,-1, fsm_type::rpt_seq(_ABC.begin(), _ABC.end(), 0, 10), fsm_type::accept, 0, 0 }
};

TEST_CASE("Sequence repetitions") {
    static string_type const rpt_chars("_ABC_ABC_ABC_ABC");
    fsm_type fsm(rpt_tr, 0);

    CHECK(fsm.exec(0, rpt_chars.begin(), rpt_chars.end())
            == fsm_type::result_type(true, rpt_chars.end()));
}
