
#include <ylib/web/rest.h>

using namespace ylib::web;

int main() {
    const char* base = "https://jsonplaceholder.typicode.com";
    RestClient client{base};

    Query q;
    q.put("postId", "1");

    string txt = client.wget("/comments", q);

    json::array comments = json::value::parse(txt).as_array();

    json::value email = comments.at(0).at("email");

    println(sfput("Got email of the first comment: ${}", email.as_string()));
    return EXIT_SUCCESS;
}
