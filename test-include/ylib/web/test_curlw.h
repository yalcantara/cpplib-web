#pragma once

#include <ylib/core/lang.h>
#include <ylib/test.h>

#include <ylib/web/curlw.h>

using namespace ylib::test;
using namespace std;


namespace ylib::web::curlw {

    void httpGetTest() {
        TEST("HTTP GET Test", [] {

            auto res = http::get("https://jsonplaceholder.typicode.com");
            assertFalse(isStrBlank(res));
        });

        TEST("HTTP GET Error test", [] {
            assertThrows([&] { http::get("https://hub.docker.com/v2/user/"); }); //401
        });
    }
}