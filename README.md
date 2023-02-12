# C++ | cpplib-web
A header-only web library for C++, that contains simple-to-use functions and classes that simplify development.


## Features
- Easy to use
- Based on cpplib-core
- Header-only!
- Simple to use REST Client

## Development
- C++ 17
- Docker
- Ubuntu Linux (18.04)
- clang
- cmake
- Boost
- OpenSSL

## To compile & run 🔥

The compile and run process is automated using **Docker** and some simple scripts.

First, build the Docker:
```shell
./build-image.sh
```
Then, compile & run:
```shell
./compile.sh
./run.sh
```

This will run the default `main` program which does an REST API call to `https://jsonplaceholder.typicode.com` and parses the response in JSON. Then extract, the email of the first comment.

```console
Docker container created. About to run program main.
Got email of the first comment: Eliseo@gardner.biz
```

## Usage
Start by cloning the repo in a path that is easy to search from your desired project. You can install the dependencies in Ubuntu 18 by running:

```shell
apt-get install -y libboost-all-dev libcpprest-dev libaio-dev
```

### As a header
This is the easiest way to add to your project. Make sure `cpplib-core/include` is added to the list of includes. Then, include as a header the path `cpplib-web/include` and compile normally.

### CMake
You can add cpplib-web to your project by using `cmake`. In your `CMakeLists.txt` file, add the following instructions (assuming `main` is the name of your program):
```
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(../cpplib-core/include)

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL 1.0.0 REQUIRED)

target_link_libraries(main
        cpprest
        -lstdc++fs
        Boost::boost
        Boost::system
        OpenSSL::SSL
        ${CMAKE_DL_LIBS})
```

## Examples
### RestClient
The `RestClient` class is a wrapper of the `web::http::client::rest_client` in **cpprestsdk**. The method `wget` accepts the path of the root URL, and a `Query` object that contains the query params. It returns the response UTF-8 formatted in a `std::string`.

```cpp
#include <ylib/web/rest.h>

using namespace ylib::web;

int main() {
    const char* base = "https://jsonplaceholder.typicode.com";
    RestClient client{base};

    Query q;
    q.put("postId", "1");

    string txt = client.wget("/comments", q);

    json::array comments = json::value::parse(txt).as_array();
    // Getting the first comment, then the field "email"
    json::value email = comments.at(0).at("email");

    println(sfput("Got email of the first comment: ${}", email.as_string()));
    return EXIT_SUCCESS;
}
```
Output:
```console
Docker container created. About to run program main.
Got email of the first comment: Eliseo@gardner.biz
```




## Contributing
 Contributions are welcome to the cpplib-web library! If you have a bugfix or new feature, please create a pull request. If you have any questions, feel free to open an issue.