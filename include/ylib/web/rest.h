
#pragma once

#include <string>
#include <codecvt>
#include <ylib/core/lang.h>
#include <cpprest/http_client.h>
#include <cpprest/uri.h>

using namespace std;

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;


namespace ylib {
namespace web {

class Response {
private:
    http_response _delegator;
    uri _url;

public:
    Response(){

    }
    Response(http_response& res):_delegator(res){

    }


    Response(http_response& res, uri& url):_delegator(res), _url(url){

    }

    unsigned short status_code(){
        return _delegator.status_code();
    }

    std::string body_utf8(){
        return _delegator.extract_utf8string(true).get();
    }

    std::u16string body_utf16(){
        return _delegator.extract_utf16string(true).get();
    }

    json::value body_json(){
        return _delegator.extract_json().get();
    }


    vector<unsigned char> body_binary(){
        return _delegator.extract_vector().get();
    }

    uri url(){
        return _url;
    }

    void print(){
        std::u16string body = body_utf16();

        std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cv;
        std::cout << cv.to_bytes(body) << endl;
        fflush(stdout);
    }
};


class Headers{
private:
    http_headers _headers;

public:
    Headers(){
    }

    void put(const char* name, const char* val){
        _headers[name] = val;
    }

    void put(const char* name, const string& val){
        put(name, val.c_str());
    }

    http_headers delegate()const{
        return _headers;
    }
};


class Query {
private:
    uri_builder _builder;

public:
    Query() {

    }

    void put(const char* name, const char* val) {
        _builder.append_query(name, val);
    }

    void put(const char* name, string val) {
        _builder.append_query(name, val);
    }

    void put(const char* name, long int val) {
        _builder.append_query(name, val);
    }

    uri_builder& builder() {
        return _builder;
    }

    string to_string() const {
        return _builder.query();
    }

    void print() {
        string str = to_string();
        println(str);
    }
};


class RestClient {
private:
    string root;
    http_client client;

    string fullp(const char* path, const Query& query){
        uri_builder b;
        b.append_path(path);
        string q = query.to_string();
        b.append_query(q);

        string target = b.to_string();
        return target;
    }

    uri fullURI(const char* path_query_fragment){
    
        //construct the url first, before doing any network call.
        uri_builder b{root};
        b.append(path_query_fragment);
        uri url = b.to_uri();

        return url;
    }

    uri fullURI(string& path_query_fragment){
    
        //construct the url first, before doing any network call.
        uri_builder b{root};
        b.append(path_query_fragment);
        uri url = b.to_uri();

        return url;
    }

public:

    RestClient(const char* root) :root{root}, client{root} {
    }

    RestClient(const string& root) :root{root}, client{root} {
    }

    //Copy Constructor
    //No copy constructor allowed
    RestClient(const RestClient&) = delete;

    pplx::task<http_response> getAsync(){
        http_request req{methods::GET};
        return client.request(req);
    }

    pplx::task<http_response> getAsync(const char* path_query_fragment){
        http_request req{methods::GET};
        req.set_request_uri(path_query_fragment);

        return client.request(req);
    }

    pplx::task<http_response> getAsync(const string& path_query_fragment){
        return getAsync(path_query_fragment.c_str());
    }

    vector<pplx::task<http_response>> getAsync(const vector<string>& batch){

        vector<pplx::task<http_response>> ans;

        for(string path_query_fragment:batch){
            ans.push_back(getAsync(path_query_fragment));
        }

        return ans;
    }


    Response get(const Headers& headers, const char* path_query_fragment){
    
        http_request req{methods::GET};
        req.set_request_uri(path_query_fragment);
        http_headers& h = req.headers();

        http_headers send = headers.delegate();

        for(auto a:send){
            h.add(a.first, a.second);
        }

        uri url = fullURI(path_query_fragment);
        http_response res = client.request(req).get();
        Response ans{res, url};

        return ans;

    }

    Response get(const char* path_query_fragment) {

        uri url = fullURI(path_query_fragment);
        http_response res = client.request(methods::GET, path_query_fragment).get();

        Response ans{res, url};

        return ans;
    }

    Response get(std::string& path_query_fragment) {
        const char* str = path_query_fragment.c_str();
        return get(str);
    }

    Response get(const char* path, Query& query) {
        string target = fullp(path, query);
        return get(target);
    }

    Response get2xx(const char* path_query_fragment){
        Response r = get(path_query_fragment);
        unsigned short status = r.status_code();
        if(status < 200 || status >= 300){
            string url = r.url().to_string();
            string msg = "Response is not 2xx. Got: " + std::to_string(status) +", for url: " + url +".";
            throw Exception(msg);
        }
        return r;
    }

    Response get2xx(const string& url){
        return get2xx(url.c_str());
    }

    Response get2xx(const Headers& headers, const char* path_query_fragment){
        Response r = get(headers, path_query_fragment);
        unsigned short status = r.status_code();
        if(status < 200 || status >= 300){
            string url = r.url().to_string();
            string msg = "Response is not 2xx. Got: " + std::to_string(status) +", for url: " + url +".";
            throw Exception(msg);
        }
        return r;
    }

    Response get2xx(const char* path, const Query& q){
        string target = fullp(path, q);
        return get2xx(target.c_str());
    }

    Response get2xx(const Headers& headers, const char* path, const Query& q){
        string target = fullp(path, q);
        return get2xx(headers, target.c_str());
    }

    string wget(const char* path, const Query& q){
        return get2xx(path, q).body_utf8();
    }

    string wget(const Headers& headers, const char* path, const Query& q){
        return get2xx(headers, path, q).body_utf8();
    }

    string wget(const string& path, const Query& q){
        const char* cpath = path.c_str();
        return wget(cpath, q);
    }

    string wget(const char* path){
        return get2xx(path).body_utf8();
    }

    string wget(string& path){
        return wget(path.c_str());
    }

    u16string wget_utf16(const char* path){
        return get2xx(path).body_utf16();
    }

    u16string wget_utf16( const char* path, Query& q){
        return get2xx(path, q).body_utf16();
    }

    u16string wget_utf16( const Headers& headers, const char* path, Query& q){
        return get2xx(headers, path, q).body_utf16();
    }

    json::value getJson(const Headers& headers, const char* path, Query& q){
        return get2xx(headers, path, q).body_json();
    }
};

pplx::task<http_response> request(const string& url){

    RestClient client{url};

    return client.getAsync();
}

} // namespace web
} // namespace ylib