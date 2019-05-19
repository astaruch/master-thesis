#ifndef PHISHING_DETECTION_URL_TEST_H
#define PHISHING_DETECTION_URL_TEST_H

#include <Poco/URI.h>

namespace phishapp {

class Url_test {
public:
    /**
      * \brief Constructor. Assign raw URL and parse it into helper members,
      *  which aren't present in Poco::URI class. If raw_url is e.g.
      *  www.inf.ed.ac.uk, then m_tld = uk, m_sld = ac, m_components[0] = ac,
      *  m_components[1] = ed, ...
      * \param raw_url whole URL
      */
    explicit Url_test(const std::string& raw_url);

    int
    test_length() const;

    int
    test_depth() const;

    int
    test_special_chars() const { throw std::exception(); }

    int
    test_special_keywords() const { throw std::exception(); }

    int
    test_hostname_ip() const { throw std::exception(); }

    int
    test_non_std_port() const { throw std::exception(); }

    int
    test_non_std_tld() const { throw std::exception(); }

    int
    test_query_script() const { throw std::exception(); }

    int
    test_query_html() const { throw std::exception(); }

    int
    test_chars_freq() const { throw std::exception(); }

    int
    test_in_phishtank() const { throw std::exception(); }

    int
    do_tests();

private:
    //! Raw URL string
    std::string m_raw_url;

    //! Parsed URL object
    Poco::URI m_url;

    //! Flag whether parsing was successful
    bool m_initialized;

    //! Top level domain
    std::string m_tld;

    //! Second level domain
    std::string m_sld;

    /**
     * Components (or labels) of a hostname
     */
    std::vector<std::string> m_components;
};

} // namespace phishapp

#endif //PHISHING_DETECTION_URL_TEST_H
