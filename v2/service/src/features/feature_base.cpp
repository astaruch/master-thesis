#include "feature_base.h"

#include <Poco/Exception.h>
#include <fmt/format.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>


namespace feature {

void base::set_url(const std::string& url)
{
    _url = url;
    try {
        _url_obj = Poco::URI(url);
        _url_is_ok = true;
    } catch (const Poco::SyntaxException& ex) {
        fmt::print(stderr, "Error. URL '{}' is malformed.\n", url);
        _url_obj = Poco::URI();
        _value = 1.0; // we say that URL is phishing if we can't parse it
    }
    get_page();
}

void base::get_page()
{
    if(!_url_is_ok) {
        fmt::print(stdout, "Skipping downloading page because URL is malformed\n");
        return;
    }
    try	{
        curlpp::Cleanup my_cleanup;
        curlpp::options::Url my_url(_url);
        curlpp::Easy my_request;
        my_request.setOpt(my_url);
        my_request.setOpt(curlpp::options::HttpHeader({"Content-type: text/html; charset=UTF-8"}));
        my_request.setOpt(curlpp::options::FollowLocation(true));
        my_request.perform();
        std::cout << my_request << std::endl;
    } catch(curlpp::RuntimeError& e) {
		std::cout << e.what() << std::endl;
	} catch(curlpp::LogicError& e){
		std::cout << e.what() << std::endl;
	}
}

double base::normalize_value(int min, int value, int max)
{
    value = std::max(min, std::min(value, max));
    return static_cast<double>(value - min) / static_cast<double>(max - min);
}

} // namespace feature

